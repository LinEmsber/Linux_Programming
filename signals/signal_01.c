/* Installing a handleer for SIGINT */

#include <stdio.h>
#include <signal.h>
#include <errno.h>

static void handler(int sig)
{
	printf("got signal, go into the handler!\n");
}

int main(){

	int i;

	if (signal(SIGINT, handler) == SIG_ERR){
		perror("signal");
	}


	for (i = 0; ; i++) {

		printf("%d\n", i);
		sleep(3);
	}

	return 0;
}
