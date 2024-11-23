#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define LAST_MESSAGE 1

struct mymsgbuf // Custom structure for the message
{
  long mtype;
  struct {
    int i;
    char msg;
  } data;
} mybuf;

int main(void) {
  int msqid;
  char pathname[] = "Makefile";
  key_t key;
  int len, maxlen;

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  while (1) {
    //
    // In an infinite loop, accept messages of any type in FIFO order
    // with a maximum length of 81 characters
    // until a message of type LAST_MESSAGE is received.
    //
    maxlen = 81;

    if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, -2, 0)) < 0) {
      perror("Can\'t receive message from queue\n");
      exit(-1);
    }

    if (mybuf.mtype == LAST_MESSAGE) {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(0);
    }

    //
    // If the received message is of type LAST_MESSAGE,
    // then terminate and remove the message queue from the system.
    // Otherwise, print the text of the received message.
    //
    mybuf.mtype = 3;
    mybuf.data.i = -1;
    mybuf.data.msg = 'b';
    len = sizeof(mybuf.data);
    if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(-1);
    }

    printf("message type = %ld, i = %d, msg = %c\n", mybuf.mtype, mybuf.data.i,
           mybuf.data.msg);
  }

  return 0;
}
