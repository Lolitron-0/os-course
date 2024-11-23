#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int mqid;
struct sembuf ship_sem;
struct sembuf person_sem;

void *passenger(void *idx_ptr) {
  int idx = *(int *)idx_ptr;

  printf("person %d: waiting...\n", idx);
  person_sem.sem_op = 1;
  if (semop(mqid, &person_sem, 1) < 0) {
    perror("Can\'t semop\n");
    exit(-1);
  }

  ship_sem.sem_op = -1;
  if (semop(mqid, &ship_sem, 1) < 0) {
    perror("Can\'t semop\n");
    exit(-1);
  }
  printf("person %d: on board\n", idx);
  return NULL;
}

int main() {
  int result;
  key_t key;
  ship_sem.sem_num = 0;
  ship_sem.sem_flg = 0;
  person_sem.sem_num = 1;
  person_sem.sem_flg = 0;

  if ((key = ftok("sem1.txt", 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((mqid = semget(key, 2, 0666 | IPC_CREAT)) < 0) {
    perror("Can\'t create semaphore set\n");
    exit(-1);
  }

  int N, K;
  scanf("%d %d", &N, &K);
  pthread_t *threads = (pthread_t *)malloc(N * K * sizeof(pthread_t));
  int indexes[N * K];

  for (int i = 0; i < N * K; i++) {
    indexes[i] = i;
    result = pthread_create(&threads[i], NULL, passenger, &indexes[i]);

    if (result != 0) {
      printf("Error on thread create, return value = %d\n", result);
      exit(-1);
    }
  }

  for (int i = 0; i < N * K; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("No more people in queue");

  free(threads);
}
