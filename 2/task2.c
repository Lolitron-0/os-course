#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t pid, ppid;
  pid_t child_pid;
  int a = 0;

  child_pid = fork();
  if (child_pid < 0) {
    perror("Fork failed");
    return -1;
  } else if (child_pid == 0) {
    printf("Hello papa!\n");
    int sum = 0;
    for (int i = 0; i < 1e9; i++) {
      sum++;
    }
    printf("I can count to %i\n", sum);
    exit(0);
  }

  a = a + 1;

  pid = getpid();
  ppid = getppid();

  printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);

  int status;
  printf("waiting\n");
  if (waitpid(child_pid, &status, 0) < 0) {
    perror("Waitpid failed");
    return -1;
  }
  printf("exited\n");

  return 0;
}
