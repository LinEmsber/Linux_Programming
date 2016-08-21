/* An example of readv() */

#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/uio.h>

int main()
{

        int i;
        int fd;
        ssize_t nr;
        struct iovec iov[5];

        // space for i/o vector
        char str_0[50], str_1[50], str_2[50], str_3[50], str_4[50];

        // open
        fd = open("test.txt", O_RDONLY );
        if (fd == -1){
                perror("open");
                return 1;
        }

        // set up iovec structure
        iov[0].iov_base = str_0;
        iov[0].iov_len = sizeof(str_0);
        iov[1].iov_base = str_1;
        iov[1].iov_len = sizeof(str_1);
        iov[2].iov_base = str_2;
        iov[2].iov_len = sizeof(str_2);
        iov[3].iov_base = str_3;
        iov[3].iov_len = sizeof(str_3);
	iov[4].iov_base = str_3;
        iov[4].iov_len = sizeof(str_3);

        // readv
        nr = readv(fd, iov, 5);
        if (nr == -1){
                perror("writev");
                return 1;
        }

        for (i = 0; i < 5; i++){
                printf("%d: %s", i, (char *) iov[i].iov_base );
        }

        if ( close(fd) ){
                perror("close");
                return 1;
        }

        return 0;

}
