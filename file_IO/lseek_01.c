/* An example of lseek() */

// The off_t data type is a signed integer data type specified by POSIX.1.
// grep -r off_t /usr/include/sys/types.h

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	// SEEK_SET
	off_t ret;
	ret = lseek(fd, (off_t) 1825, SEEK_SET);
	if (ret == (off_t) -1){
		perror("lseek");
		return -1;
	}

	// SEEK_END
	off_t ret;
	ret = lseek(fd, 0, SEEK_END);
	if (ret == (off_t) -1){
		perror("lseek");
		return -1;
	}

	// SEEK_CUR
	off_t ret;
	ret = lseek(fd, 0, SEEK_CUR);
	if (ret == (off_t) -1){
		perror("lseek");
		return -1;
	}

}
