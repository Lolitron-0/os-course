#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd[2];
  ssize_t size=0;
  char resstring[14];
  //
  // Try to create a pipe
  //
  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  printf("\n\n%i\n\n", fcntl(fd[1], F_GETPIPE_SZ));

  // fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
  // unsigned long long cnt = 0;
  // while (1) {
  //   size = write(fd[1], "Hello, world!", 1);
  //   if (size != 1) {
  //     printf("%lli\n", cnt);
  //     perror(NULL);
  //     exit(0);
  //   }
  //   cnt++;
  // }

  if (size != 14) {
    printf("Can\'t write all string to pipe\n");
    exit(-1);
  }

  size = read(fd[0], resstring, 14);

  if (size < 0) {
    printf("Can\'t read string from pipe\n");
    exit(-1);
  }

  printf("%s\n", resstring);

  if (close(fd[0]) < 0) {
    printf("Can'\t close reading side of pipe\n");
    exit(-1);
  }
  if (close(fd[1]) < 0) {
    printf("Can'\t close writing side of  pipe\n");
    exit(-1);
  }

  return 0;
}
