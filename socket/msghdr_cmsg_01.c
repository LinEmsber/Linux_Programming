/* An example of socket programming */

// recvmsg()
// The recvmsg() call uses a msghdr structure to minimize the number of
// directly supplied arguments.
//
// msg_iov is an array of input/output buffers with length msg_iovlen.
// Each member of this array contains a pointer to a data buffer and the
// size of the buffer.
//
// msg_control points to a buffer of size msg_controllen that contains
// additional information about the packet. To read this field, you first
// need to declare a struct cmsghdr * (let's call it cmhdr).

// access ancillary data
//
// IP_TTL (since Linux 1.0)
// Set or retrieve the current time-to-live field that is used in every packet sent
// from this socket.
//
// IP_TOS (since Linux 1.0)
// Set or receive the Type-Of-Service (TOS) field that is sent with every IP packet
// originating from this socket.  It is used to prioritize packets on the network.


// struct cmsghdr *CMSG_FIRSTHDR(struct msghdr *msgh);
// CMSG_FIRSTHDR() returns a pointer to the first cmsghdr in the ancillary data
// buffer associated with the passed msghdr.
//
// struct cmsghdr *CMSG_NXTHDR(struct msghdr *msgh, struct cmsghdr *cmsg);
// CMSG_NXTHDR() returns the next valid cmsghdr after the passed cmsghdr. It returns
// NULL when there isn't enough space left in the buffer.
//
// unsigned char *CMSG_DATA(struct cmsghdr *cmsg);
// CMSG_DATA() returns a pointer to the data portion of a cmsghdr.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

int main()
{
	int sockfd;
	int ret_sendmsg;
	char control[1000];
	char buf[1000];
	unsigned char tos;

	struct sockaddr_in addr_in;
	struct msghdr header;
	struct iovec io[1];
	struct cmsghdr *cmsg;

	memset(&addr, 0, sizeof (struct sockaddr_in) );
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_in.sin_port = htons(PORT);


	header.msg_name 		= &addr_in;
	header.msg_namelen 		= sizeof(struct sockaddr_in);
	haeder.msg_iov			= &io;
	header.msg_iovlen		= 1;
	header.msg_control		= control;
	header.msg_controllen		= sizeof(control)

	// iovec
	io[0] -> iov_base		= buf;
	io[0].iov_len			= sizeof(buf);


	// recv msg
	// ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
	ret_recvmsg = recvmsg(sockfd, &haeder, 0);
	if (ret_recvmsg < 0){
		perror("recvmsg");
	}

	c_header = CMSG_FIRSTHDR(&header);

	while( c != NULL){

		if ( cmsg -> cmsg_level == IPPROTO_IP && cmsg -> cmsg_type == IP_T){

			tos = ( (unsigned char *) CMSG_DATA(cmsg) )[0];
		}

		cmsg = CMSG_NXTHDR(&header, cmsg)
	}

	printf("read: %s, tos byte = %02X\n", buf, tos);

	return 0;
}
