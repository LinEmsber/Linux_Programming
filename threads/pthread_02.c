/* An example of pthread API */

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

// compute successive prime numbers
void *compute_prime (void* arg)
{

	int candidate = 2;
	int n = *((int*) arg);

	while (1) {
		int factor;
		int is_prime = 1;

		// test primality by successive division.
		for (factor = 2; factor < candidate; ++factor){

			if (candidate % factor == 0) {
				is_prime = 0;
				break;
			}
		}
		// is the prime number or not
		if (is_prime) {
			if (--n == 0)
				// printf("%d\n", n);
				return (void *)candidate;
		}
		++candidate;
	}
	return NULL;
}

int main ()
{

	int prime;
	int which_prime = 1000;
	int which_prime_origianl = which_prime;
	int ret_pthread_create, ret_pthread_join;
	pthread_t thread;

	// make a new thread
	ret_pthread_create = pthread_create (&thread, NULL, &compute_prime, &which_prime);
	if (ret_pthread_create != 0)
		perror("pthread_create");

	// wait for the prime number thread to complete
	ret_pthread_join = pthread_join (thread, (void**) &prime);
	if (ret_pthread_join != 0)
		perror("pthread_join");


	// print the largest prime
	printf("The %d-th prime number is %d.\n", which_prime_origianl, prime);

	return 0;
}
