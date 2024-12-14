#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void my_handler(int nsig) {
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, &status, 0)) > 0) {
    if ((status & 0xff) == 0) {
      printf("Process %d was exited with status %d\n", pid, status >> 8);
    } else if ((status & 0xff00) == 0) {
      printf("Process %d killed by signal %d %s\n", pid, status & 0x7f,
             (status & 0x80) ? "with core file" : "without core file");
    }
  }
  if (pid < 0) {
    if (errno == ECHILD)
      return;
    printf("cant waitpid %i\n", errno);
    exit(1);
  }
}

int main(void) {
  pid_t pid;
  int i, j;

  (void)signal(SIGCHLD, my_handler);

  for (i = 0; i < 10; i++) {
    if ((pid = fork()) < 0) {
      printf("Can\'t fork child 1\n");
      exit(1);
    } else if (pid == 0) {
      /* Child  */
      for (j = 1; j < 1000000; j++)
        ;
      sleep(2);
      exit(200 + i);
    }
  }

  while (1)
    ;
  return 0;
}
