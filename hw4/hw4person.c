#include "buf.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int mqid;

void *passenger(void *idx_ptr) {
  int idx = *(int *)idx_ptr;
  mymsgbuf mybuf;
  mybuf.mtype = 1;
  mybuf.data.pid = getpid();
  mybuf.data.msg = 2;

  if (msgsnd(mqid, (struct msgbuf *)&mybuf, DATALEN, 1) < 0) {
    perror("Can\'t send to mq\n");
    exit(-1);
  }

  printf("person %d: waiting...\n", idx);

  if (msgrcv(mqid, (struct msgbuf *)&mybuf, DATALEN, mybuf.data.pid, 0) < 0) {
    perror("Can\'t receive from mq\n");
    exit(-1);
  }

  if (mybuf.data.msg == 1) {
    printf("person %d: got out\n", idx);
  }

  return NULL;
}

int main() {
  int result;
  key_t key;

  if ((key = ftok("mq1.txt", 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((mqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    perror("Can\'t create mq\n");
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

  printf("No more people in queue\n");

  free(threads);
}
