#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_ready_enter = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_entered = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_ready_leave = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_left = PTHREAD_COND_INITIALIZER;
int ready_to_enter = 0;
int entered = 0;
int ready_to_leave = 0;
int left = 0;
int N, K;

void *passenger(void *arg) {
  int i = *(int *)arg;

  pthread_mutex_lock(&lock);
  while (ready_to_enter == 0) {
    pthread_cond_wait(&cond_ready_enter, &lock);
  }
  ready_to_enter--;
  entered = 1;
  printf("%i is on the boat\n", i);
  pthread_mutex_unlock(&lock);

  pthread_cond_signal(&cond_entered);

  pthread_mutex_lock(&lock);
  while (ready_to_leave == 0) {
    pthread_cond_wait(&cond_ready_leave, &lock);
  }
  ready_to_leave--;
  left = 1;
  printf("%i got off\n", i);
  pthread_mutex_unlock(&lock);

  pthread_cond_signal(&cond_left);

  return NULL;
}

int main() {
  scanf("%d %d", &N, &K);
  int pass_nums[N * K];

  pthread_t threads[K * N];
  for (int i = 0; i < K * N; i++) {
    pass_nums[i] = i;
    if (pthread_create(&threads[i], NULL, passenger, &pass_nums[i]) != 0) {
      printf("Can\'t create thread\n");
      exit(-1);
    }
  }

  for (int i = 0; i < K; i++) {

    printf("boat: Got %d free seats\n", N);
    for (int j = 0; j < N; j++) {
      ready_to_enter = 1;

      pthread_cond_signal(&cond_ready_enter);

      pthread_mutex_lock(&lock);
      while (entered == 0) {
        pthread_cond_wait(&cond_entered, &lock);
      }
      entered = 0;
      pthread_mutex_unlock(&lock);
    }

    printf("boat: sailing...\n");
    sleep(1);

    printf("boat: getting people out\n");
    for (int j = 0; j < N; j++) {
      ready_to_leave = 1;

      pthread_cond_signal(&cond_ready_leave);

      pthread_mutex_lock(&lock);
      while (left == 0) {
        pthread_cond_wait(&cond_left, &lock);
      }
      left = 0;
      pthread_mutex_unlock(&lock);
    }
  }

  for (int i = 0; i < K * N; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      printf("Can\'t join thread\n");
      exit(-1);
    }
  }

  return 0;
}
