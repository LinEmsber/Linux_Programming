/* The example of brk() and sbrk().
 * We can use GDB and valgrind to check the result, as follows:
 * > gcc -Wall -g brk_sbrk_01.c && valgrind ./a.out
 */
#define _GNU_SOURCE
#include <unistd.h>

int main(void)
{
	void * b_original;
	void * b_new;
	int * p;

	/* Get the first address beyond the end of the heap. */
	b_original = sbrk(0);

	/* Move it 2 ints forward */
	// brk( b_original + (2 * sizeof(int)) );
	sbrk(2 * sizeof(int));

	/* Get the new end of the heap. */
	b_new = sbrk(0);

	/* Use a pointer point to the old end of the heap. */
	p = (int *) b_original;

	/* Use the memory to assign the value for it. */
	*p = 1;
	*(p + 1) = 2;

	/* deallocate memory */
	brk(b_original);

	return 0;
}
