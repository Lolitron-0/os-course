#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

// boat: a_21 a_1N z
// chel: d_2 d_1

int main() {
  key_t key;
  int semid;

  if ((key = ftok("sem1.txt", 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 2, 0666 | IPC_CREAT)) < 0) {
    perror("Can\'t create semaphore set\n");
    exit(-1);
  }

  struct sembuf ship_sem;
  ship_sem.sem_num = 0;
  ship_sem.sem_flg = 0;
  struct sembuf person_sem;
  person_sem.sem_num = 1;
  person_sem.sem_flg = 0;

  int N, K;
  scanf("%d %d", &N, &K);

  // init ship
  ship_sem.sem_op = N;
  if (semop(semid, &ship_sem, 1) < 0) {
    perror("Can\'t semop\n");
    exit(-1);
  }

  for (int i = 0; i < K; i++) {
    printf("boat: Got %d free seats\n", N);

    // wait until full
    ship_sem.sem_op = 0;
    if (semop(semid, &ship_sem, 1) < 0) {
      perror("Can\'t semop\n");
      exit(-1);
    }
    printf("boat: full\n");

    // wait for people to stop boarding
    person_sem.sem_op = -N;
    if (semop(semid, &person_sem, 1) < 0) {
      perror("Can\'t semop\n");
      exit(-1);
    }

    printf("boat: sailing...\n");
    sleep(2);
    printf("boat: getting people out\n");

    // free up seats
    ship_sem.sem_op = N;
    if (semop(semid, &ship_sem, 1) < 0) {
      perror("Can\'t semop\n");
      exit(-1);
    }
  }

  // sailing away :) (sems should be 0 at the end)
  ship_sem.sem_op = -N;
  if (semop(semid, &ship_sem, 1) < 0) {
    perror("Can\'t semop\n");
    exit(-1);
  }

  return 0;
}
