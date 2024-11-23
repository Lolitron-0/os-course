#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

// 2
// Parent: w z r a2
// Child:  d r w d

int main() {
  int fd[2], result;
  size_t size;
  int N;
  char pathname[] = "launch.json";
  key_t key;
  int semid;
  struct sembuf sem;
  sem.sem_num = 0;
  sem.sem_flg = 0;

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    perror("Can\'t create semaphore set\n");
    exit(-1);
  }
  semctl(semid, 0, SETVAL, 2);
  scanf("%d", &N);

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

    /* Parent process */

    for (int i = 0; i < N; i++) {
      size = write(fd[1], &i, 4);
      if (size != 4) {
        perror("Can\'t write\n");
        exit(-1);
      }

      sem.sem_op = 0;

      if (semop(semid, &sem, 1) < 0) {
        printf("Can\'t semop\n");
        exit(-1);
      }

      int data;
      size = read(fd[0], &data, 4);
      if (size != 4) {
        perror("Can\'t read\n");
        exit(-1);
      }

      printf("PARENT: %i\n", data);

      sem.sem_op = 2;

      if (semop(semid, &sem, 1) < 0) {
        printf("Can\'t semop\n");
        exit(-1);
      }

    }

    if (close(fd[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n");
      exit(-1);
    }

    if (close(fd[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n");
      exit(-1);
    }

  } else {

    /* Child process */

    for (int i = 0; i < N; i++) {

      sem.sem_op = -1;

      if (semop(semid, &sem, 1) < 0) {
        printf("Can\'t semop\n");
        exit(-1);
      }

      int data;
      size = read(fd[0], &data, 4);
      if (size != 4) {
        perror("Can\'t read\n");
        exit(-1);
      }

      printf("CHILD: %i\n", data);

      size = write(fd[1], &i, 4);
      if (size != 4) {
        perror("Can\'t write\n");
        exit(-1);
      }

      sem.sem_op = -1;

      if (semop(semid, &sem, 1) < 0) {
        printf("Can\'t semop\n");
        exit(-1);
      }
    }

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n");
      exit(-1);
    }
    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n");
      exit(-1);
    }
  }

  return 0;
}
