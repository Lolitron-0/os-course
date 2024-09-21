#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) { 
	pid_t pid = getpid();
	pid_t ppid = getppid();
	printf("pid: %i\n", pid);
	printf("ppid: %i\n", ppid);
	return 0; 
}
