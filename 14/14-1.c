#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1

char str[80];
int count = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t snd = PTHREAD_COND_INITIALIZER;
pthread_cond_t rcv = PTHREAD_COND_INITIALIZER;

void *th(void *q) {
  // consumer
  for (int i = 0; i < 20; i++) {
    if (pthread_mutex_lock(&m) != 0) {
      printf("error lock in consumer\n");
      exit(-1);
    }
    while (count == 0) {
      if (pthread_cond_wait(&snd, &m) != 0) {
        printf("error wait in consumer\n");
        exit(-2);
      }
    }
    printf("receive %s\n", str);
    count -= 1;
    if (count == N - 1) {
      if (pthread_cond_signal(&rcv) != 0) {
        printf("error signal in consumer\n");
        exit(-3);
      }
    }
    if (pthread_mutex_unlock(&m) != 0) {
      printf("error unlock in consumer\n");
      exit(-4);
    }
  }
}

int main() {
  int i;
  int result;
  pthread_t thid;

  result = pthread_create(&thid, (pthread_attr_t *)NULL, th, NULL);

  if (result != 0) {
    printf("Error on thread create, return value = %d\n", result);
    exit(-5);
  }

  // producer

  for (i = 0; i < 20; i++) {
    if (pthread_mutex_lock(&m) != 0) {
      printf("error lock in main thread\n");
      exit(-6);
    }

    while (count == N) {
      if (pthread_cond_wait(&rcv, &m) != 0) {
        printf("error wait in producer\n");
        exit(-7);
      }
    }

    sprintf(str, "message %d", i);
    printf("send %s\n", str);

    count += 1;
    if (count == 1) {
      if (pthread_cond_signal(&snd) != 0) {
        printf("error signal in producerr\n");
        exit(-8);
      }
    }

    if (pthread_mutex_unlock(&m) != 0) {
      printf("error unlock in producer\n");
      exit(-9);
    }
  }

  result = pthread_join(thid, (void **)NULL);

  if (result != 0) {
    printf("Error on thread join, return value = %d\n", result);
    exit(-11);
  }

  if (pthread_cond_destroy(&snd) != 0) {
    printf("error destroy snd\n");
    exit(-12);
  }

  if (pthread_cond_destroy(&rcv) != 0) {
    printf("error destroy rcv\n");
    exit(-13);
  }

  if (pthread_mutex_destroy(&m) != 0) {
    printf("error destroy mutex\n");
    exit(-14);
  }

  return 0;
}
