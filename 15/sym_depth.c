#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH 4096
#define PREFIX "/home/niten/dev/os-course/"

int main() {
  umask(0);
  if (mkdir("temp", 0777) < 0) {
    perror("can't mkdir\n");
    exit(1);
  }

  int fd;
  int i = 1;
  char name[MAX_PATH] = PREFIX "temp/a0";
  char symname[MAX_PATH] = PREFIX "temp/a1";
  if ((fd = creat(name, 0666) < 0)) {
    perror("can't create\n");
    exit(1);
  }
  if (close(fd) < 0) {
    perror("can't close\n");
    exit(1);
  }

  while (1) {
    if (symlink(name, symname) < 0) {
      perror("can't link\n");
      exit(1);
    }

    if ((fd = open(symname, O_RDONLY) < 0)) {
      perror("can't open\n");
      exit(1);
    }
    if (close(fd) < 0) {
      perror("can't close\n");
      exit(1);
    }

    printf("%d\n", i);
    i++;
    strncpy(name, symname, MAX_PATH);
    sprintf(symname, PREFIX "temp/a%d", i);
  }
  return 0;
}
