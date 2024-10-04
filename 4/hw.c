#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd[2], child_pid;
	int result;

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

    if (close(fd[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n");
      exit(-1);
    }

    if (dup2(fd[0], 0) == -1) {
      perror("dup2 failed");
      exit(-1);
    }

    if (close(fd[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n");
      exit(-1);
    }

    result = execlp("cat", "cat", NULL);
    if (result == -1) {
      perror("exec failed");
      exit(-1);
    }

    printf("Parent exit\n");

  } else {

    /* Child process */

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n");
      exit(-1);
    }

    if (dup2(fd[1], 1) == -1) {
      perror("dup2 failed");
      exit(-1);
    }
    if (dup2(fd[1], 2) == -1) {
      perror("dup2 failed");
      exit(-1);
    }
    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n");
      exit(-1);
    }

    result = execlp("ls", "ls", NULL);
    if (result == -1) {
      perror("exec failed");
      exit(-1);
    }
  }

  return 0;
}
