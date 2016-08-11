/* An example of select() */

#include <stdio.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main()
{
	int ret;
	struct timeval tv;
	fd_set read_fds;

	// Initialize fd_set
	FD_ZERO(&read_fds);
	FD_SET(STDIN_FILENO, &read_fds);

	// timeout
	tv.tv_sec = TIMEOUT;
	tv.tv_usec = 0;

	// select block STDIN_FILENO, wait for any input
	ret = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &tv);
	if (ret == -1){
		perror("select");
		return 1;
	}else if (!ret){
		printf("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}

	if ( FD_ISSET(STDIN_FILENO, &read_fds) ){

		int len;
		char buf[BUF_LEN + 1];

		len = read(STDIN_FILENO, buf, BUF_LEN);
		if (len == -1){
			perror("read");
			return 1;
		}

		if(len){
			buf[len] = '\0';
			printf("read: %s\n", buf);
		}

		return 0;
	}
}
