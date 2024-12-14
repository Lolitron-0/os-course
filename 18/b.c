#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

uint32_t number = 0;
int recieved_bits = 0;
bool done = false;
pid_t other_pid;

void my_handler(int nsig) {
  number <<= 1;
  if (nsig == SIGUSR1) {
    number += 1;
  } else if (nsig == SIGUSR2) {
    number += 0;
  }
  recieved_bits++;
  if (recieved_bits == 32) {
    done = true;
  }

  if (kill(other_pid, SIGUSR1) < 0) {
    perror("can't kill");
    exit(1);
  }
}

int main() {
  pid_t pid = getpid();
  printf("my pid: %d\n", pid);
  printf("other pid: ");
  scanf("%d", &other_pid);

  signal(SIGUSR1, my_handler);
  signal(SIGUSR2, my_handler);

  while (!done)
    sleep(1);

  printf("number: %u\n", number);

  return 0;
}
