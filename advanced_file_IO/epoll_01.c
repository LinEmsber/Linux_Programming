/* An example of epoll() */


// int epoll_create1(int flags);
//
// epoll_create() returns a file descriptor referring to the new epoll instance.
// This file descriptor is used for all the subsequent calls to the epoll interface.
// If flags is 0, then, other than the fact that the obsolete size argument is dropped.
//
// On success, these system calls return a nonnegative file descriptor. On error, -1 is returned.


// int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
//
// This system call performs control operations on the epoll(7) instance referred to by the file descriptor epfd.
// It requests that the operation op be performed for the target file descriptor, fd.
//
// Valid values for the op argument are:
// EPOLL_CTL_ADD, EPOLL_CTL_MOD, and EPOLL_CTL_DEL.
//
// When successful, epoll_ctl() returns zero.  When an error occurs, epoll_ctl() returns -1.


// int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
//
// The epoll_wait() system call waits for events on the epoll(7) instance referred to by the file descriptor
// epfd. The memory area pointed to by events will contain the events that will be available for the caller.
// Up to maxevents are returned by epoll_wait(). The maxevents argument must be greater than zero.
//
// When successful, epoll_wait() returns the number of file descriptors ready for the requested I/O, or zero
// if no file descriptor became ready.

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE		10
#define OPEN_MAX	100
#define LISTENQ		20
#define SERV_PORT	5555
#define INFTIM		1000

// error massage handler
#define handle_error(msg)               \
        do {                            \
                perror(msg);            \
                exit(EXIT_FAILURE);     \
        } while (0)

// set fd as nonblocking
void set_nonblocking(int sock)
{
        int opts;

        // get flags from fd.
        opts = fcntl(sock, F_GETFL);
        if(opts < 0) {
                handle_error("fcntl(sock, GETFL)");
        }

        // set flags with nonblocking.
        opts = opts | O_NONBLOCK;
        if(fcntl(sock, F_SETFL, opts) < 0) {
                handle_error("fcntl(sock, SETFL, opts)");
        }
}

int main(int argc, char *argv[])
{
        printf("epoll socket begins.\n");

        int i;                                  // iteration
        int ret;                                // return for function
        int listenfd, connfd, sockfd;           // fd
        int epfd;                               // fd for epoll
        int nfds;                               // number of fd for polling
        ssize_t nr;                             // n byte read
        char line[MAXLINE];                     // char line for reading
        socklen_t cli_len;

	struct epoll_event ev, events[20];

	// epfd = epoll_create(256);
        epfd = epoll_create1(0);
        if ( epfd == -1 ){
                handle_error("epoll_create1");
        }

        // socket addr structure for client and server
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
        if (listenfd == -1){
                handle_error("socket");
        }

        // set fd as nonblocking
	set_nonblocking(listenfd);

	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;

        // epoll_ctl to add fd
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
        if (ret == -1){
                handle_error("epoll_ctl");
        }

        // initialize and set struct sockaddr_in
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	// char *local_addr = "192.168.199.8";
        char *local_addr = "127.0.0.1";

	inet_aton(local_addr, &(server_addr.sin_addr));
	server_addr.sin_port = htons(SERV_PORT);

        // bind
	ret = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret == -1){
                handle_error("bind");
        }

        // listen
	ret = listen(listenfd, LISTENQ);
        if (ret == -1){
                handle_error("bind");
        }


	for(;;) {

                // wait for event fds become
		nfds = epoll_wait(epfd, events, 20, 5);

                // poll for each fds
		for(i = 0; i < nfds; ++i) {

                        // listen fd
			if(events[i].data.fd == listenfd) {

				printf("accept connection, fd is %d\n", listenfd);
				connfd = accept(listenfd, (struct sockaddr *)&client_addr, &cli_len);
				if(connfd < 0) {
					handle_error("connfd < 0");
					exit(1);
				}

				set_nonblocking(connfd);

				char *str = inet_ntoa(client_addr.sin_addr);
				printf("connect from %s\n", str);

				ev.data.fd = connfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);

                        // poll in
			} else if(events[i].events & EPOLLIN) {

				if((sockfd = events[i].data.fd) < 0)
                                        continue;

				if((nr = read(sockfd, line, MAXLINE)) < 0) {

					if(errno == ECONNRESET) {
						close(sockfd);
						events[i].data.fd = -1;
					} else {
						printf("readline error");
					}

				} else if(nr == 0) {
					close(sockfd);
					events[i].data.fd = -1;
				}

				printf("received data: %s\n", line);

				ev.data.fd = sockfd;
				ev.events = EPOLLOUT | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);

                        // poll out
			} else if(events[i].events & EPOLLOUT) {

				sockfd = events[i].data.fd;
				write(sockfd, line, nr);

				printf("written data: %s\n", line);

				ev.data.fd = sockfd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
			}

                // for(i = 0; i < nfds; ++i) loop
		}

                printf("timeout\n");
        // for(;;) loop
	}

        return 0;
}
