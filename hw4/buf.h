#pragma once
#include <sys/types.h>

typedef struct mymsgbuf {
  long mtype;
  struct {
    char msg;
    pid_t pid;
  } data;
} mymsgbuf;

#define DATALEN sizeof(mymsgbuf) - sizeof(long)
