/* An example of ftruncate() and truncate() */

// truncate, ftruncate - truncate a file to a specified length
//
// int truncate(const char *path, off_t length);
// int ftruncate(int fd, off_t length);
//
// The truncate() system call operates on the filenmae given by path, which
// must be writable.


#include <stdio.h>
#include <unistd.h>

int main()
{
	int ret;

	ret = truncate("./pirate.txt", 45);
	if(ret == -1){
		perror("truncate");
		return -1;
	}

	return 0;
}
