#include "buf.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int msqid;
  char pathname[] = "Makefile";
  key_t key;
  mymsgbuf mybuf;
  int len = sizeof(mybuf.data);

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      perror("Can\'t get msqid\n");
    } else {
      printf("tried to launch second server\n");
    }
    exit(-1);
  }

  while (1) {
    if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, len, -2, 0)) < 0) {
      perror("Can\'t receive message from queue\n");
      exit(-1);
    }

    if (mybuf.mtype == 1) {
      printf("ouch\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      break;
    }

    printf("got %f\n", mybuf.data.num);
    mybuf.mtype = mybuf.data.pid;
    mybuf.data.num = mybuf.data.num * mybuf.data.num;
    len = sizeof(mybuf.data);
    if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(-1);
    }
  }

  return 0;
}
