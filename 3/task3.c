#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  pid_t pid, ppid;
  pid_t child_pid;

  child_pid = fork();
  if (child_pid < 0) {
    perror("Fork failed");
    return -1;
  } else if (child_pid == 0) {
    char prog[] = "/usr/bin/lsd";
    char *args[] = {prog, "-la", "/", NULL};
    int result = execv(prog, args);
    if (result == -1) {
      perror("execl failed");
      exit(-1);
    }
  }

  pid = getpid();
  ppid = getppid();

  printf("My pid = %d, my ppid = %d\n", (int)pid, (int)ppid);

  int status;
  printf("waiting\n");
  if (waitpid(child_pid, &status, 0) < 0) {
    perror("Waitpid failed");
    return -1;
  }
  printf("exited\n");

  return 0;
}
