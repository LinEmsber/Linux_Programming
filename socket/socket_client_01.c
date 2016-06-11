/* An example of socket cilent */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// inet_pton, convert IPv4 and IPv6 addresses from text to binary form
// int inet_pton(int af, const char *src, void *dst);
//
// This function converts the character string src into a network address structure
// in the af address family, then copies the network address structure to dst.
// The af argument must be either AF_INET or AF_INET6.
//
//
// fputs, output of characters and strings
// int fputs(const char *s, FILE *stream);
//
// fputs() writes the string s to stream, without its terminating null byte ('\0').


int main(int argc, char *argv[])
{

	int sockfd = 0;
	int listen_fd = 0, ret_read = 0, ret_inet_pton = 0, ret_connect = 0;
	struct sockaddr_in serv_addr;

	char recv_buf[1024];
	memset(recv_buf, 0, sizeof(struct sockaddr_in));

	if(argc != 2) {
		printf("Usage: %s <ip of server> \n", argv[0]);
		perror("argc");
	}

	// create a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_fd < 0){
		perror("socket");
	}

	// set domian
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);


	ret_inet_pton = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	if(ret_inet_pton <= 0){
		perror("inet_pton");
	}

	// connect to the server
	ret_connect = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) );
	if( ret_connect < 0){
		perror("connect");
	}

	while ( 1 ){

		ret_read = read(sockfd, recv_buf, sizeof(recv_buf) );
		if ( ret_read > 0){

			recv_buf[ret_read] = 0;
			if(fputs(recv_buf, stdout) == EOF){
				printf("\n Error : Fputs error\n");
			}
			break;

		} else if(ret_read < 0)	{

			printf("\n Read error \n");

		}
	// while loop
	}
	return 0;
}
