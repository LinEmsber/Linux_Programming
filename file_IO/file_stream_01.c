/* Sample program using buffered I/O */

#include <stdio.h>
#include <string.h>


struct pirate
{
        char 		name[64];
        unsigned long   booty;
        unsigned int    beard_len;
};

int main()
{
	FILE *in, *out;
        struct pirate p, black_bread;

        strcpy(black_bread.name, "Edward");
	black_bread.booty = 950;
	black_bread.beard_len = 10;

        // open a stream, out, for write.
        out = fopen("./data", "w");
        if( !out ){
                perror("out");
                return 1;
        }

        // write to stream, out, to 1 element, each sizeof(struct pirate) bytes in length
        // from the data pointed at by buf, black_bread.
        if ( !fwrite (&black_bread, sizeof(struct pirate), 1, out) ){
                perror("fwrite");
                return 1;
        }

        // close a stream, out.
        if ( fclose(out) ){
                perror("fclose");
                return 1;
        }

        // open a stream, in, for read.
        in = fopen("./data", "r");
        if ( !in ){
                perror("fclose");
                return 1;
        }

        if ( !fread( &p, sizeof(struct pirate), 1, in) ){
                perror("fclose");
                return 1;
        }

        if ( fclose(in) ){
                perror("fclose");
                return 1;
        }

        printf("name: %s, boot: %d, beard_len: %d\n", p.name, p.booty, p.beard_len);

        return 0;
}
