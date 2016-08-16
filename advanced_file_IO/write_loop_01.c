/* readv() or writev() could be implemented as read() or write() in simple loop */

#include <unistd.h>
#include <errno.h>
#include <sys/uio.h>

ssize_t write_loop(int fd, const struct iovec *iov, int count)
{
        int i;                  // iteration
        ssize_t ret = 0;        // return for total string length

        for ( i = 0; i < count; i++){

                ssize_t nr;
                errno = 0;

                nr = write(fd, iov[i].iov_base, iov[i].iov_len);
                if ( nr == -1 ){
                        if ( errno == EINTR ){
                                continue;
                        }

                        ret = -1;
                        break;
                }

                // sum the total read char
                ret =+ nr;
        }

        return ret;
}
