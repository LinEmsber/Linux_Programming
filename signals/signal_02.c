/* Simple program to catch SIGUSR1 and SIGUSR2 */

#include <stdio.h>
#include <signal.h>

// argument is signal number
static void sig_usr(int signo)
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		printf("received signal %d\n", signo);
}

int main(void)
{
	int i;
	int times = 5;
	pid_t pid;
	pid = getpid();
	printf("%d\n", pid);

	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR1\n");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		printf("can't catch SIGUSR2\n");

	for (i=0; i<times; i++)
		kill(pid, SIGUSR2);

	return 0;
}
