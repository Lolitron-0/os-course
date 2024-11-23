#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define LAST_MESSAGE 1 // Message type for termination of program 11-1b.c

struct mymsgbuf // Custom structure for the message
{
  long mtype;
  struct {
    int i;
    char msg;
  } data;
} mybuf;

int main(void) {
  int msqid; // IPC descriptor for the message queue
  char pathname[] =
      "Makefile"; // The file name used to generate the key.
                  // A file with this name must exist in the current directory.
  key_t key;      // IPC key
  int i, len;     // Cycle counter and the length of the informative part of the
                  // message

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  /* Send information */

  for (i = 1; i <= 5; i++) {
    //
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 2;
    mybuf.data.i = i;
    mybuf.data.msg = 'a';
    len = sizeof(mybuf.data);
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0) {
      perror("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(-1);

    }
    printf("sent a\n");

    if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, len, 3, 0)) < 0) {
      perror("Can\'t receive message from queue\n");
      exit(-1);
    }

    printf("got msg = %c\n", mybuf.data.msg);
  }

  /* Send the last message */

  mybuf.mtype = LAST_MESSAGE;
  len = 0;

  if (msgsnd(msqid, (struct msgbuf *)&mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    exit(-1);
  }

  return 0;
}
