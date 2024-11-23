#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2], child_pid;

  size_t size;
  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  child_pid = fork();

  if (child_pid < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (child_pid > 0) {

    /* Parent process */

    unsigned int version = 0xbadf00d;
    size = write(fd[1], &version, sizeof version);

    if (size != sizeof version) {
      printf("Can\'t write greeting to pipe\n");
      exit(-1);
    }

    int child_status;
    if (waitpid(child_pid, &child_status, 0) < 0) {
      perror("Waitpid failed");
      exit(-1);
    }

    unsigned int response = 0;
    size = read(fd[0], &response, sizeof response);
    if (size != sizeof response || response != (version ^ 0xffffffff)) {
      printf("Child misbehaving. %u\n", child_pid);
      exit(-1);
    }

    if (close(fd[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n");
      exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n");
      exit(-1);
    }

    printf("Parent exit\n");

  } else {

    /* Child process */

    unsigned int greeting = 0;
    size = read(fd[0], &greeting, sizeof greeting);

    if (size != sizeof greeting) {
      printf("Can\'t read greeting from pipe\n");
      exit(-1);
    }

    greeting ^= 0xffffffff;
    size = write(fd[1], &greeting, sizeof greeting);

    if (size != sizeof greeting) {
      printf("Can\'t write response\n");
      exit(-1);
    }

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n");
      exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n");
      exit(-1);
    }
  }

  return 0;
}
