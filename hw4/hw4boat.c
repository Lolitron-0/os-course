#include "buf.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  key_t key;
  mymsgbuf mybuf;
  int mqid;
  int len;

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

  pid_t *pids = malloc(N * sizeof(pid_t));

  for (int i = 0; i < K; i++) {
    printf("boat: Got %d free seats\n", N);

    for (int j = 0; j < N; j++) {
      if ((len = msgrcv(mqid, (struct msgbuf *)&mybuf, DATALEN, 1, 0)) < 0) {
        printf("Can\'t receive\n");
        exit(-1);
      }

      pids[j] = mybuf.data.pid;
    }

    printf("boat: sailing...\n");
    sleep(2);
    printf("boat: getting people out\n");

    for (int j = 0; j < N; j++) {
      mybuf.mtype = pids[j];
      mybuf.data.msg = 1;
      if (msgsnd(mqid, (struct msgbuf *)&mybuf, DATALEN, pids[j]) < 0) {
        printf("Can\'t send mq\n");
        exit(-1);
      }
    }
  }

  return 0;
}
