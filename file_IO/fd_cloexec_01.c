/* An example fo using FD_CLOEXEC */


#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	pid_t pid;
	int wstatus;

	char *const parament_list_1[] = {"./print_out_argument", argv[1], NULL};

	fd = open("./open_test", O_RDONLY);
	if (fd == -1){
		perror("open");
	}
	fcntl(fd, FD_CLOEXEC);


	pid = fork();


	// error
	if (pid < 0){

		perror("fork()");

	// child
	}else if (pid == 0){

		execv("./print_out_argument", parament_list_1);


	// parent
	}else {

		waitpid(-1, &wstatus, 0);

	}

	return 0;
}
