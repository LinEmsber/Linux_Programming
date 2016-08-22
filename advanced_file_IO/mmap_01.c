/* An example of mmap() */

// The program creates a memory mapping of the required pages of the file and then uses write()
// to output the desired bytes.
//
// The first argument: The specified file want to open.
// The second argument: The offset of open.
// The third argument(optional): The length of open. If it is without this argument display to end of file.

// void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
//
// mmap() creates a new mapping in the virtual address space of the calling process. The
// starting address for the new mapping is specified in addr.  The length argument specifies
// the length of the mapping.
//
// If addr is NULL, then the kernel chooses the address at which to create the mapping;
// this is the most portable method of creating a new mapping.  If addr is not NULL, then
// the kernel takes it as a hint about where to place the mapping; on Linux, the mapping will
// be created at a nearby page boundary.  The address of the new mapping is returned as the
// result of the call.
//
// The contents of a file mapping are initialized using length bytes starting at offset, offset,
// in the file  referred to by the file descriptor fd.
//
// On success, mmap() returns a pointer to the mapped area.  On error, the value MAP_FAILED (that
// is, (void *) -1) is returned.


// int munmap(void *addr, size_t length);
//
// The munmap() system call deletes the mappings for the specified address range, and causes
// further references to addresses within the range to generate invalid memory references.
// The region is also automatically unmapped when the process is terminated.  On the other
// hand, closing the file descriptor does not unmap the region.
//
// On success, munmap() returns 0.  On failure, it returns -1.


// int fstat(int fd, struct stat *buf);
//
// These functions return information about a file, in the buffer pointed to by buf.
//
// On success, zero is returned.  On error, -1 is returned,

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>


// error massage handler
#define handle_error(msg)               \
        do {                            \
                perror(msg);            \
                exit(EXIT_FAILURE);     \
        } while (0)


int main(int argc, char *argv[])
{
        int fd;                                 // file descriptor for open
        ssize_t nw;                             // n bytes for read
        char *addr;                             // return addr from mmap()
        off_t offset, page_aligned_offset;      // offset
        size_t length;                          // length to read

        struct stat sb;

        // argument check
        if (argc < 3 || argc > 4) {
                fprintf(stderr, "%s file offset [length]\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        // file open
        fd = open(argv[1], O_RDONLY);
        if (fd == -1){
                handle_error("open");
        }

        // To obtain file size
        if (fstat(fd, &sb) == -1){
                handle_error("fstat");
        }


        // offset for mmap() must be page aligned
        offset = atoi(argv[2]);
        page_aligned_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);

        // print out info
        printf("\ntotal size in byte: %zu\n", sb.st_size);
        printf("offset: %zu\n", offset);
        printf("_SC_PAGE_SIZE: %zu\n", sysconf(_SC_PAGE_SIZE));
        printf("page_aligned_offset: %zu\n", page_aligned_offset);

        // prevent offset exceed the end of file.
        if (offset >= sb.st_size) {
                fprintf(stderr, "offset is past end of file\n");
                exit(EXIT_FAILURE);
        }

	printf("\n");
        // Do we set the length argument for displaying?
        if (argc == 4) {

                length = atoi(argv[3]);

                // Can't display bytes past end of file
                if (offset + length > sb.st_size){
                        length = sb.st_size - offset;
                }

        // No length arg. So we display to end of file.
        } else {

                length = sb.st_size - offset;
        }

        // mmap() returns a pointer to the mapped area.
        addr = mmap(NULL, length + offset - page_aligned_offset, PROT_READ, MAP_PRIVATE, fd, page_aligned_offset);
        if (addr == MAP_FAILED){
                handle_error("mmap");
        }

        // write to STDOUT
        nw = write(STDOUT_FILENO, addr + offset - page_aligned_offset, length);
        if (nw != length) {
                if (nw == -1){
                        handle_error("write");
                }

                fprintf(stderr, "partial write");
                exit(EXIT_FAILURE);
        }

        printf("\n");

        // munmap removes any memory maps from (addr) to (addr + length).
        munmap(addr, length + offset - page_aligned_offset);
        close(fd);

        exit(EXIT_SUCCESS);
}
