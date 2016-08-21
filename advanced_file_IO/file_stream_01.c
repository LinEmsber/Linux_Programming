/* An example of fopen(), fread(), fwrite(), fseek() */


// size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
//
// The function fread() reads nmemb items of data, each size bytes long, from the stream
// pointed to by stream, storing them at the location given by ptr.


// size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
//
// The function fwrite() writes nmemb items of data, each size bytes long, to the stream
// pointed to by stream, obtaining them from the location given by ptr.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITEM_SIZE 10
#define NUM_ITEM 5

// error massage handler
#define handle_error(msg)               \
        do {                            \
                perror(msg);            \
                exit(EXIT_FAILURE);     \
        } while (0)

int main(void)
{

        FILE* fd = NULL;
        int nr;
        int ret;
        char buff[100];

        // memory initialization
        memset(buff, 0, sizeof(buff));

        // file open through fopen()
        fd = fopen("test.txt","rw+");
        if(fd == NULL) {
                handle_error("fopen");
        }

        // read fd
        ret = fread(buff, ITEM_SIZE, NUM_ITEM, fd);
        if(ret == -1) {
                handle_error("fread");
        }

        printf("\nThe bytes read are:\n%s\n", buff);

        // set fd seek
        ret = fseek(fd, 0, SEEK_END);
        if(ret == -1) {
                handle_error("fseek");
        }

        // file write through fwrite
        ret = fwrite(buff, ITEM_SIZE, strlen(buff), fd);
        if(ret == -1) {
                handle_error("fwrite");
        }

        // close fd
        fclose(fd);

        return 0;
}
