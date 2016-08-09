/* An example of synchronized I/O */

// The system calls fsync() and fdatasync() ensures that all data associated with the
// file mapped by the file descriptor fd are written back to disk.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

const char *write_string = "This is test string for synchonized I/O.";


// The call writes back both data and metadata, such as creation timestamps and other
// attributes contained in the inode.
void write_log_fsync(char *fd_path, char *string)
{
	int fd;
	int nw;
	int ret;

	fd = open(fd_path, O_WRONLY | O_CREAT, 0664 );
	if (fd == -1){
		perror("open");
		return -1;
	}

	nw = write(fd, &string, strlen(string));
	if (nw < 0){
		perror("write");
		return -1;
	}

	ret = fsync(fd);
	if (ret == -1){

		// EINVAL: The given file descriptor is mapped to an object that does not
		// support synchronization.
		// In some version of Linux, a call to fsync() may fail because fsync() is
		// not implemented by the backing filesystem.
		if ( errno == EINVAL){

			ret = fdatasync(fd);
			if (ret == -1){
				perror("fdatasync");
				return -1;
			}

		} else {
			perror("fsync");
			return -1;

		}

	}
}

// The system call, fdatasync(), does the same thing as fsync(), except that it only flushes
// data and metadata required to properly access the file in the future.
void write_log_fdatasync(char *fd_path, char *string)
{
	int fd;
	int nw;
	int ret;

	fd = open(fd_path, O_WRONLY | O_CREAT, 0664 )
	if (fd == -1){
		perror("open");
		return -1;
	}

	nw = write(fd, &string, strlen(string));
	if (nw < 0){
		perror("write");
		return -1;
	}

	ret = fdatasync(fd);
	if (ret == -1){
		perror("fdatasync");
		return -1;
	}
}
