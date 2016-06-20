/* An example of understanding and using setbuf() */


// setbuf, setvbuf, stream buffering operations
//
// void setbuf(FILE *stream, char *buf);
//
//
// The setvbuf() function may be used on any open stream to change its
// buffer.  The mode argument must be one of the following three macros:
//
// 	_IONBF unbuffered
//
// 	_IOLBF line buffered
//
// 	_IOFBF fully buffered


#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 128

int main()
{

	char buf[BUFSIZE];

	setbuf(stdout, buf);
	puts("This is tutorialspoint.");

	sleep(3);

	fflush(stdout);
	return(0);
}
