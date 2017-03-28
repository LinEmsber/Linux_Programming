/* dump content of file to hex */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

const char hex_asc[] = "0123456789abcdef";
#define hex_asc_lo(x) hex_asc[((x) & 0x0f)]
#define hex_asc_hi(x) hex_asc[((x) & 0xf0) >> 4]

/* hex_dump_to_buffer
 *
 * @buf: data
 * @len: The number of bytes in the data
 * @row_size: The number of bytes to print per line
 * @group_size: The number of bytes to print at a time
 * @line_buf: where to save the converted data
 * @line_buf_len: total size of @line_buf, including space for terminating NULL
 * @ascii: whether show the ascii after hex print
 */
void hex_dump_to_buffer(const void *buf, size_t len, int row_size, int group_size, char *line_buf, size_t line_buf_len, int ascii)
{
	const uint8_t *ptr = buf;
	uint8_t ch;
	int j, lx = 0;
	int ascii_column;

	// how many character want to show at on row.
	if (row_size != 16 && row_size != 32)
		row_size = 16;

	if (!len)
		goto nil;
	if (len > row_size)
		len = row_size;
	if ((len % group_size) != 0)
		group_size = 1;

	switch (group_size) {
	case 8: {
		const uint64_t *ptr8 = buf;

		// int ngroups = len >> 3;
		int ngroups = len / group_size;

		for (j = 0; j < ngroups; j++)
			lx += snprintf(line_buf + lx, line_buf_len - lx, "%16.16llx ", (unsigned long long)*(ptr8 + j));

		ascii_column = 17 * ngroups + 2;
		break;
	}

	case 4: {
		const uint32_t *ptr4 = buf;
		int ngroups = len / group_size;

		for (j = 0; j < ngroups; j++)
			lx += snprintf(line_buf + lx, line_buf_len - lx, "%8.8x ", *(ptr4 + j));

		ascii_column = 9 * ngroups + 2;
		break;
	}

	case 2: {
		const uint16_t *ptr2 = buf;
		int ngroups = len / group_size;

		for (j = 0; j < ngroups; j++)
			lx += snprintf(line_buf + lx, line_buf_len - lx, "%4.4x ", *(ptr2 + j));

		ascii_column = 5 * ngroups + 2;
		break;
	}

	default:
		for (j = 0; (j < row_size) && (j < len) && (lx + 4) < line_buf_len; j++) {
			ch = ptr[j];
			line_buf[lx++] = hex_asc_hi(ch);
			line_buf[lx++] = hex_asc_lo(ch);
			line_buf[lx++] = ' ';
		}
		ascii_column = 3 * row_size + 2;
		break;
	}

	if (!ascii)
		goto nil;

	while (lx < (line_buf_len - 1) && lx < (ascii_column - 1))
		line_buf[lx++] = ' ';

	for (j = 0; (j < row_size) && (j < len) && (lx + 2) < line_buf_len; j++)
		line_buf[lx++] = (isascii(ptr[j]) && isprint(ptr[j])) ? ptr[j] : '.';

nil:
	line_buf[lx++] = '\0';

}

/* print_hex_dump
 *
 * @row_size: The number of bytes to print per line
 * @group_size: The number of bytes to print at a time
 * @buf: data
 * @len: The number of bytes in the data
 */
void print_hex_dump(int row_size, int group_size, const void *buf, size_t len, int ascii)
{
	int i, line_len, remaining = len;
	const uint8_t *ptr = buf;
	unsigned char line_buf[200];

	// fix row size to 16 or 32
	if (row_size != 16 && row_size != 32)
		row_size = 16;

	for (i = 0; i < len; i += row_size) {

		line_len = ((remaining) < (row_size) ? (remaining) : (row_size));
		remaining -= row_size;
	
		// hex_dump_to_buffer(const void *buf, size_t len, int row_size, int group_size, char *line_buf, size_t line_buf_len, int ascii)
		hex_dump_to_buffer(ptr + i, line_len, row_size, group_size, line_buf, sizeof(line_buf), ascii);
	
		printf("%s\n", line_buf);
	}
}

int main(int argc, char **argv)
{
	int fd;
	int nr;

	// chunk size determine how much bytes read each time
	char chunk[4096];

	// file open
	fd = open(argv[1], O_RDONLY);
	if ( !fd ){
		perror("open()");
	}

	while(1){

		// memory set zero to chunk
		memset(chunk, 0, sizeof(chunk));

		nr = read(fd, chunk, sizeof(chunk));
		if (nr <= 0) {
			close(fd);
			break;
		}

		print_hex_dump(32, 1, chunk, nr, 1);
	}

	return 0;
}
