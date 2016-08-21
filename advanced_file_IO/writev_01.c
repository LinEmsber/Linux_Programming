/* An example of writev() */

#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/uio.h>


char *buf[] =
{
        "Japan’s economic growth ground to a halt in the second quarter as\n",
        "weak exports and shaky domestic demand prompted companies to cut spending,\n",
        "putting fresh pressure on Japanese Prime Minister Shinzo Abe to come up with\n",
        "policies to produce more sustainable growth.\n",
	"\n"
};

int main()
{
        int i;
        int fd;
        ssize_t nr;
        struct iovec iov[5];

        // open
        fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC);
        if (fd == -1){
                perror("open");
                return 1;
        }

	// chmod
	fchmod(fd, 00644);


        // fill out iovec structure
        for ( i = 0; i < 5; i++){
                iov[i].iov_base = buf[i];
                iov[i].iov_len = strlen(buf[i] + 1);
        }

        // write
        nr = writev(fd, iov, 5);
        if (nr == -1){
                perror("writev");
                return 1;
        }

        if ( close(fd) ){
                perror("close");
                return 1;
        }

        return 0;
}
