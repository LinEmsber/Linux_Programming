/*
 * This is a simple serial program which computes the dot product of two
 * vectors.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define VEC_LEN 1000

// define the struct and the typedef
typedef struct _dot dot;
struct _dot{
	double *a;
	double *b;
	double sum;
	int vec_len;
};

// the global structure for computing of dot product
dot dotstr;

// thread numbers
pthread_t call_threads[NUM_THREADS];

// Mutual exclusion
pthread_mutex_t mutex;

// Define and use local variables for convenience
void dot_product() {

	int start, end, i;
	double mysum, *x, *y;

	start = 0;
	end = dotstr.vec_len;
	x = dotstr.a;
	y = dotstr.b;

	mysum = 0;
	for (i=start; i<end ; i++)	{
		mysum += (x[i] * y[i]);
	}

	pthread_mutex_lock (&mutex);
	dotstr.sum += mysum;
	pthread_mutex_unlock (&mutex);

	pthread_exit((void*) 0);
}


int main (int argc, char *argv[])
{

	int i;				// Iteration
	int len = VEC_LEN;
	double *a, *b;
	void *status;
	pthread_attr_t attr;

	// Assign storage and initialize values
	a = (double*) malloc (NUM_THREADS*VEC_LEN*sizeof(double));
	b = (double*) malloc (NUM_THREADS*VEC_LEN*sizeof(double));

	for (i=0; i<len; i++) {
		a[i]=1;
		b[i]=a[i];
	}

	dotstr.vec_len = len;
	dotstr.a = a;
	dotstr.b = b;
	dotstr.sum = 0;

	pthread_mutex_init(&mutex, NULL);

	// Create threads to perform the dotproduct
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i=0; i<NUM_THREADS; i++)	{
		pthread_create(&call_threads[i], &attr, dot_product, (void *)i);
	}

	pthread_attr_destroy(&attr);

	// Wait on the other threads
	for(i=0; i<NUM_THREADS; i++)	{
		pthread_join(call_threads[i], &status);
	}

	// After joining, print out the results and cleanup
	printf ("Sum =  %f \n", dotstr.sum);

	free (a);
	free (b);
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
}
