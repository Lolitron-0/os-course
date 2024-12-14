#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid = getpid();
  pid_t other_pid;
  uint32_t number;
  printf("my pid: %d\n", pid);
  printf("other pid: ");
  scanf("%d", &other_pid);
  printf("number: ");
  scanf("%u", &number);

  uint32_t mask = 1U << 31;
  for (int i = 0; i < 32; i++) {
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
    usleep(1000);
  }

  return 0;
}
