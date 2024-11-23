#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  char *buf=0;                      // Pointer to shared memory
  int shmid;                      // IPC descriptor for the shared memory
  char pathname[] = ".gitignore"; // The file name used to generate the key.

  key_t key;

  if ((key = ftok(pathname, 0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  int fd;
  if ((fd = open("./6/07-1a.c", O_RDONLY)) < 0) {
    perror("Can\'t open file\n");
    exit(-1);
  }

  int sz;
  if ((sz = lseek(fd, (size_t)0, SEEK_END)) == -1) {
    perror("lseek failed");
    exit(-1);
  }
	lseek(fd, 0, SEEK_SET);

  if ((shmid = shmget(key, sz, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, sz, 0)) < 0) {
        perror("Can\'t find shared memory\n");
        exit(-1);
      }
    }
  }
  //
  // Try to map shared memory to the address space of the current process.
  // Note that for proper comparison we explicitly convert the value -1 to a
  // pointer to an integer.
  //
  if ((buf = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

	errno = 0;
  sz = read(fd, buf, sz);
  if (sz <= 0) {
    perror("Can't read");
    exit(-1);
  }

  if (shmdt(buf) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
