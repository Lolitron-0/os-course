#pragma once
#include <sys/types.h>

typedef struct mymsgbuf {
  long mtype;
  struct {
    float num;
    pid_t pid;
  } data;
} mymsgbuf;
