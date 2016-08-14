/* reading an entire line */

// read an entire line from a file, and save this string into a string buffer.

#include <stdio.h>
#define BUFF_SIZE 256

int main()
{
        int c;                          // one byte char to read
        int n;                          // integer for byte count
	int d;				// stop reading at a dilimiter, given by the integer d.
        char *s;                        // pointer for string
        char str[BUFF_SIZE];            // a string for saving char which is read from file
        FILE *stream;                   // read a file as stream

        // the maximum char we can read
        n = BUFF_SIZE;

	// stop at dilimiter, d
	d = 10;

        // open a file as a stream
        stream = fopen("./pirate.txt", "r");

        // s pointer represent the first position for string
        s = str;
        while ( --n > 0 && ( (c = fgetc(stream))  != EOF) &&  (*s++ = c) != d ){
                ;
        }

        if (c == d)
                *--s = '\0';
        else
                *s = '\0';


	printf("str: %s\n", str);

        return 0;
}
