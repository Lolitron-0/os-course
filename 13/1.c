#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int gvar = 0;
int sync_flags[3] = {0};
pthread_mutex_t mutex;

void *job(void *arg) {
  // int n = *(int *)arg;
  // int other_n = (n + 1) % 2;
  for (int i = 0; i < 10000000; i++) {
    if (pthread_mutex_lock(&mutex) != 0) {
      perror("Error on mutex lock\n");
      exit(-1);
    }
    // sync_flags[n] = 1;
    // sync_flags[2] = n;
    // while (sync_flags[other_n] && sync_flags[2] == n) {
    // }
    gvar++;
    // sync_flags[n] = 0;
    if (pthread_mutex_unlock(&mutex) != 0) {
      perror("Error on mutex unlock\n");
      exit(-1);
    }
  }
  return NULL;
}

int main() {
  int res;
  pthread_t t1, t2;
  int n1 = 0;

  res = pthread_mutex_init(&mutex, NULL);
  if (res != 0) {
    perror("Error on mutex init\n");
    exit(-1);
  }

  res = pthread_create(&t1, NULL, &job, &n1);
  if (res != 0) {
    perror("Error on thread create\n");
    exit(-1);
  }

  int n2 = 1;
  res = pthread_create(&t2, NULL, &job, &n2);
  if (res != 0) {
    perror("Error on thread create\n");
    exit(-1);
  }

  sleep(1);
  res = pthread_join(t1, NULL);
  if (res != 0) {
    perror("Error on thread join\n");
    exit(-1);
  }
  res = pthread_join(t2, NULL);
  if (res != 0) {
    perror("Error on thread join\n");
    exit(-1);
  }

  res = pthread_mutex_destroy(&mutex);
  if (res != 0) {
    perror("Error on mutex destroy\n");
    exit(-1);
  }

  printf("%i\n", gvar);
  return 0;
}
