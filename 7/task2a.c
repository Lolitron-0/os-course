#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main() {
  int *array;
  int shmid;
  int new = 1;
  char pathname[] = ".gitignore";
  key_t key;
  long i;

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 6 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 6 * sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  if (new) {
    array[0] = 1;
    array[1] = 0;
    array[2] = 1;
    array[3] = 0; // lock 1
    array[4] = 0; // lock 2
    array[5] = 0; // ready_to_wait
  } else {
    array[3] = 1;
    array[5] = 0;
    printf("%i\n", array[4] && array[5] == 0);
    while (array[4] && array[5] == 0) {
    }

    array[0] += 1;
    for (i = 0; i < 2000000000L; i++)
      ;
    array[2] += 1;
  }

  printf(
      "Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
      array[0], array[1], array[2]);

  array[3] = 0;

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
