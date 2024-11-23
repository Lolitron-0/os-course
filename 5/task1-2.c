#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd;
  size_t size;
  char resstring[14];
  char name[] = "aaa.fifo";

  if (mkfifo(name, 0666) < 0) {
    if (errno == EEXIST) {
      printf("already exists, opening\n");
    } else {
      perror("Can\'t create FIFO\n");
      exit(-1);
    }
  }

  if ((fd = open(name, O_RDONLY)) < 0) {
    printf("Can\'t open FIFO for reading\n");
    exit(-1);
  }

  size = read(fd, resstring, 14);

  if (size < 0) {
    printf("Can\'t read string from FIFO\n");
    exit(-1);
  }

  printf("Child exit, resstring: %s\n", resstring);

  if (close(fd) < 0) {
    printf("child: Can\'t close FIFO\n");
    exit(-1);
  }
}
