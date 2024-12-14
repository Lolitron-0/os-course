#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

uint32_t number;
uint32_t mask = 1U << 31;
pid_t other_pid;

void my_handler(int nsig) {
  if (number & mask) {
    if (kill(other_pid, SIGUSR1) < 0) {
      perror("can't kill");
      exit(1);
    }
  } else {
    if (kill(other_pid, SIGUSR2) < 0) {
      perror("can't kill");
      exit(1);
    }
  }
  mask >>= 1;
}

int main() {
  pid_t pid = getpid();
  printf("my pid: %d\n", pid);
  printf("other pid: ");
  scanf("%d", &other_pid);
  printf("number: ");
  scanf("%u", &number);

  signal(SIGUSR1, my_handler);

  my_handler(SIGUSR1);

  while (mask) {
    sleep(1);
  }

  return 0;
}
