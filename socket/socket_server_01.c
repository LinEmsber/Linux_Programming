/* An example of socket server */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// struct sockaddr_in {
// 	sa_family_t    sin_family; /* address family: AF_INET */
// 	in_port_t      sin_port;   /* port in network byte order */
// 	struct in_addr sin_addr;   /* internet address */
// };
//
// socket, creates an endpoint for communication and returns a file descriptor
// that refers to that endpoint.
// int socket(int domain, int type, int protocol);
//
//
// htonl, htons, ntohl, ntohs, convert values between host and network
// byte order
// uint16_t htons(uint16_t hostshort);
//
//
// bind, bind a name to a socket
// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
//
//
// listen - listen for connections on a socket
// int listen(int sockfd, int backlog);
//
//
// accept, accept a connection on a socket
// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int main(int argc, char *argv[])
{

	int listen_fd = 0, conn_fd = 0;
	int ret_bind, ret_listen;
	struct sockaddr_in serv_addr;

	char sendBuff[1025];
	time_t ticks;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	memset(sendBuff, 0, sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	ret_bind = bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
	if (ret_bind != 0){
		perror("bind");
	}

	listen(listen_fd, 10);
	if (ret_listen != 0){
		perror("listen");
	}

	while(1){
		conn_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);

		ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		write(conn_fd, sendBuff, strlen(sendBuff));

		close(conn_fd);
		sleep(1);
	}

	return 0;
}
