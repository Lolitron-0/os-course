#include "buf.h"
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

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  mybuf.mtype = 1;
  len = sizeof(mybuf.data);
  if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    exit(-1);
  }

  return 0;
}
