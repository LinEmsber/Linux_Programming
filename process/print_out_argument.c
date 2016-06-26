/* A print out program for execv() */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{

	printf("ppid: %d\tpid: %d\targv: %s\n", getppid(), getpid(), argv[1]);

	return 0;
}
