#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int *array;
  int shmid;
  int new = 1;
  char pathname[] = "launch.json";
  char pathname2[] = "Makefile";
  key_t key;
  key_t key2;
  long i;
  int semid;
  struct sembuf sem;

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  if ((key2 = ftok(pathname2, 0)) < 0) {
    printf("Can\'t generate key2\n");
    exit(-1);
  }

  if ((semid = semget(key2, 1, 0666 | IPC_CREAT)) < 0) {
    perror("Can\'t create semaphore set\n");
    exit(-1);
  }

  printf("SEM: %i\n", semctl(semid, 0, GETVAL, 0));

  if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
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
    sem.sem_flg = 0;
    sem.sem_num = 0;
    sem.sem_op = 1;

    if (semop(semid, &sem, 1) < 0) {
      perror("Can\'t semop\n");
      exit(-1);
    }
    printf("SEM: %i\n", semctl(semid, 0, GETVAL, 0));
    array[0] = 1;
    array[1] = 0;
    array[2] = 1;
  } else {
    sem.sem_flg = 0;
    sem.sem_num = 0;
    sem.sem_op = -1;

    if (semop(semid, &sem, 1) < 0) {
      perror("Can\'t semop\n");
      exit(-1);
    }

    array[0] += 1;
    for (i = 0; i < 2000000000L; i++)
      ;
    array[2] += 1;
  }

  printf(
      "Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
      array[0], array[1], array[2]);

  if (!new) {
    sem.sem_flg = 0;
    sem.sem_num = 0;
    sem.sem_op = 1;
    if (semop(semid, &sem, 1) < 0) {
      perror("Can\'t semop\n");
      exit(-1);
    }
  }

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
