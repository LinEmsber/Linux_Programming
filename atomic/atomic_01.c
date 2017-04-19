/* Using the GCC builtin function to do atomic arithmetic operation.
 *
 * References:
 * https://gcc.gnu.org/onlinedocs/gcc-5.4.0/gcc/_005f_005fatomic-Builtins.html
 * https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync
 * https://gcc.gnu.org/onlinedocs/gcc-4.3.4/gcc/Atomic-Builtins.html
 */

#include <stdio.h>

#include <pthread.h>

#define THREAD_NUMS 10000

int global_x;

void *func (void *x)
{
	int i;

	for (i = 0; i < THREAD_NUMS; i++){

#ifdef __GCC_4
		__sync_add_and_fetch (&global_x, 1);
#elif __GCC_5
		__atomic_add_fetch(&global_x, 1, __ATOMIC_RELAXED);
#else
		global_x ++;
#endif

	}

}

int main (void)
{
	int i;
	pthread_t tid[THREAD_NUMS];

#ifdef __GCC_4
	printf("Use GCC 4 built-in functions\n");
#elif __GCC_5
	printf("Use GCC 5 built-in functions\n");
#else
	printf("Default\n");
#endif
	printf("expection of global_x: %d\n", THREAD_NUMS * THREAD_NUMS);

	for (i = 0; i < THREAD_NUMS; i++){
		pthread_create (&tid[i], NULL, func, NULL);
	}

	for (i = 0; i < THREAD_NUMS; i++){
		pthread_join (tid[i], NULL);
	}
	
	printf ("result of global_x: %d\n", global_x);

	return 0;
}

