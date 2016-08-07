/* Reading all the bytes */

#include <unistd.h>

int fd;
char buf[256];
int len;

ssize_t ret;

while ( len != 0 && (ret = read(fd, buf, len) ) != 0 ){

        if( ret == -1){

                // This indicates that a signal was received before
                // any bytes were read.
                if (errno == EINTR){
                        continue;
                }

                perror("read");
                break;
        }

        // len is reduced by the amount read.
        len -= ret;

        // buf is increased by the amount read.
        buf += ret;
}
