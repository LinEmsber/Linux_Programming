/* nonblocking read */

// When performing nonblocking reads, you mest check for EAGAIN or risk confusing a
// serious error with the mere lack of data.

#define BUF_SIZE 256

char buf[BUF_SIZE];
ssize_t nr;

start:

nr = read(fd, buf, BUF_SIZE);
if (nr == -1){

        if (errno == EINTR){
                goto start;
        }

        if (errno == EAGAIN){

                /* resubmit later */

        }else{

                /* other error */
        }

}
