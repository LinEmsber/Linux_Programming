/*
 * An example of passing file descriptors between processes using
 * sendmsg() and recvmsg()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>

// struct msghdr {
// 	void         *msg_name;
// 	socklen_t    msg_namelen;
// 	struct iovec *msg_iov;
// 	size_t       msg_iovlen;
// 	void         *msg_control;
// 	size_t       msg_controllen;
// 	int          msg_flags;
// };


// CMSG_SPACE()
//
// The macro CMSG_SPACE() computes the total necessary space required for one
// ancillary data object.


// struct iovec {			/* Scatter/gather array items */
// 	void  *iov_base;		/* Starting address */
// 	size_t iov_len;			/* Number of bytes to transfer */
// };


// struct cmsghdr {
// 	socklen_t     cmsg_len;     /* data byte count, including hdr */
// 	int           cmsg_level;   /* originating protocol */
// 	int           cmsg_type;    /* protocol-specific type */
// 	/* followed by unsigned char cmsg_data[]; */
// };
//
// struct cmsghdr *CMSG_FIRSTHDR(struct msghdr *mp);
// pointer to first cmsghdr structure associated with the msghdr structure, or
// NULL if none exists



// struct cmsg
//
// unsigned int CMSG_LEN(unsigned int nbytes);
// size to allocate for data object nbytes large
//
// For historical reasons the ancillary message types listed below are
// specified with a SOL_SOCKET type even though they are AF_UNIX
// specific.
//
// SCM_RIGHTS
// Send or receive a set of open file descriptors from another
// process. The data portion contains an integer array of the
// file descriptors
//
// We use the CMSG_LEN macro to calculate the amount of space needed.


// unsigned char *CMSG_DATA(struct cmsghdr *cp);
// Returns: pointer to data associated with cmsghdr structure


// void *memmove(void *dest, const void *src, size_t n);
//
// The memmove() function copies n bytes from memory area src to memory area dest.
// The memory areas may overlap: copying takes place as though the bytes in src
// are first copied into a temporary array that does not overlap src or dest, and
// the bytes are then copied from the temporary array to dest.


// send a file descriptor from parent.
static void parent_send(int socket, int fd)
{
	int ret_sendmsg;
	char buf[ CMSG_SPACE( sizeof(fd) ) ];
	memset(buf, 0, sizeof(buf) );

	// struct iovec
	struct iovec io[1];
	char *str0 = " ";
	io[0].iov_base = str0;
	io[0].iov_len = strlen(str0);


	struct msghdr msg = {0};
	// struct msghdr
	// To finish the setting of "struct msghdr" before the setting of "struct cmsghdr".
	msg.msg_iov = io;
	msg.msg_iovlen = 2;
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);


	// The cmsg_level field is set to SOL_SOCKET, and cmsg_type is set to SCM_RIGHTS,
	// to indicate that we are passing access rights.
	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
	cmsg -> cmsg_len = CMSG_LEN(sizeof(fd));
	cmsg -> cmsg_level = SOL_SOCKET;
	cmsg -> cmsg_type = SCM_RIGHTS;

	// the length for one file descriptor
	msg.msg_controllen = cmsg->cmsg_len;


	// fetch the data which want to send, file descriptor. There are two methods.

	// method 1:
	// *(int *)CMSG_DATA(cmsg) = fd;

	// method 2:
	memmove(CMSG_DATA(cmsg), &fd, sizeof(fd));

	ret_sendmsg = sendmsg(socket, &msg, 0);
	if (ret_sendmsg < 0){
		perror("sendmsg");
	}
}


// receive fd from socket
static int child_recv(int socket)
{
	int fd;
	int ret_recvmsg;

	// Since we dont use msg_iov, so just set a small memory for m_buffer.
	char m_buffer[1];
	char c_buffer[256];

	struct iovec io[1];
	io[0].iov_base = m_buffer;
	io[0].iov_len = sizeof(m_buffer);

	struct msghdr msg = {0};
	msg.msg_iov = io;
	msg.msg_iovlen = 1;
	msg.msg_control = c_buffer;
	msg.msg_controllen = sizeof(c_buffer);


	// ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
	ret_recvmsg = recvmsg(socket, &msg, 0);
	if (ret_recvmsg < 0){
		perror("recvmsg");
	}

	// fetch the data which want to receive, file descriptor. There are two methods.
	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);

	// method 1:
	// fd = *(int *)CMSG_DATA(cmsg);

	// method 2:
	memmove(&fd, CMSG_DATA(cmsg), sizeof(fd));

	return fd;
}

int main(int argc, char **argv)
{

	int pid;
	int sv[2];
	int ret_socketpair;
	const char *filename = "./cmsg_file.txt";

	// int socketpair(int domain, int type, int protocol, int sv[2]);
	// On success, zero is returned.
	ret_socketpair = socketpair(AF_UNIX, SOCK_DGRAM, 0, sv);
	if (ret_socketpair != 0){
		perror("socketpair");
	}


	pid = fork();

	// parent process
	if (pid > 0){

		int fd;					// fd for file to open
		int parent_socket;			// socket for parent
		int status;				// the status of child process


		// leave one socket for parent process, and close another
		close(sv[1]);
		parent_socket = sv[0];


		fd = open(filename, O_RDONLY);
		if (fd < 0){
			perror("open");
		}

		printf("========parent at work========\n");
		printf("Extracted fd: %d\n", fd);

		parent_send(parent_socket, fd);



		wait(&status);
		printf("========waiting the child========\n");

		printf("========parent exits========\n");

		close(fd);

	// child process
	}else if (pid == 0){

		int fd;						// fd for file to open
		int child_socket;				// socket for child
		char buffer[256];
		int nbytes;

		// leave one socket for child process, and close another
		close(sv[0]);
		child_socket = sv[1];

		printf("========child at work========\n");

		// fetch the file descriptor from the parent process
		fd = child_recv(child_socket);
		printf("Extracted fd: %d\n", fd);


		// read from the file descriptor until end of file.
		for (;;){
			nbytes = read(fd, buffer, sizeof(buffer) );
			if (nbytes == 0){
				break;
			}else{
				write(STDOUT_FILENO, buffer, nbytes);
			}
		}

		printf("========child done========\n");
		close(fd);


	// error happen
	}else if ( pid <0){
		perror("fork");
	}


	return 0;
}
