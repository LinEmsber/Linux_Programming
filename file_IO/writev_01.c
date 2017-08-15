/* In computing, vectored I/O, also known as scatter/gather I/O, is a method of input
 * and output by which a single procedure call sequentially reads data from multiple 
 * buffers and writes it to a single data stream, or reads data from a data stream and 
 * writes it to multiple buffers, as defined in a vector of buffers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/uio.h>

#define NUMBUFS 3

int main(int argc, char *argv[])
{
	const char *buf1 = "Hello ";
	const char *buf2 = "Wikipedia ";
	const char *buf3 = "Community!\n";

	struct iovec bufs[NUMBUFS];

	bufs[0].iov_base = (void*) buf1;
	bufs[0].iov_len = strlen(buf1);

	bufs[1].iov_base = (void*) buf2;
	bufs[1].iov_len = strlen(buf2);

	bufs[2].iov_base = (void*) buf3;
	bufs[2].iov_len = strlen(buf3);

	if (-1 == writev(STDOUT_FILENO, bufs, NUMBUFS)) {
		perror("writev()");
		exit(EXIT_FAILURE);
	}

	return 0;
}
