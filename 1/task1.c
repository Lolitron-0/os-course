#include <stdio.h>
#include <unistd.h>

int main(void) {
  uid_t uid = getuid();
  gid_t gid = getgid();
  printf("UID: %u\nGID: %u\n", uid, gid);
	
	return 0;
}
