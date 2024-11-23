#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd[2], result;

  size_t size;

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  fcntl(fd[1], fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK));

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

    /* Parent process */

    if (close(fd[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n");
      exit(-1);
    }

    unsigned long long cnt = 0;
    while (1) {
      size = write(fd[1], "Hello, world!", 1);
      printf("%li\n", size);
      if (size != 1) {
        printf("%lli\n", cnt);
        perror(NULL);
        exit(0);
      }
      cnt++;
    }

    if (size != 14) {
      printf("Can\'t write all string to pipe\n");
      exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n");
      exit(-1);
    }

    printf("Parent exit\n");

  } else {

    /* Child process */

    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n");
      exit(-1);
    }

    // printf("read...\n");
    // size = read(fd[0], resstring, 14);
    //
    // if (size < 0) {
    //   printf("Can\'t read string from pipe\n");
    //   exit(-1);
    // }
    //
    // printf("Child exit, resstring:%s\n", resstring);

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n");
      exit(-1);
    }
  }

  return 0;
}
