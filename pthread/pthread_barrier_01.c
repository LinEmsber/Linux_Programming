/* The example of the pthread_barrier_t */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define THREAD_NUMBER 10

pthread_barrier_t barrier;

void * thread(void * arg)
{
	int i = *( (int *) arg);

	printf("Time: %lu, [thread] ready to work!\n", time(NULL));

	usleep( 1000000 * i);

	pthread_barrier_wait(&barrier);
	printf("Time: %lu, [thread]: init completed.\n", time(NULL));

	printf("Time: %lu, [thread]: worke completed.\n", time(NULL));

	return NULL;
}

int main(int argc, char **argv)
{
	int i;
	int arg;

	pthread_t pid[THREAD_NUMBER];

	pthread_barrier_init( &barrier, NULL, THREAD_NUMBER );
	printf("Time: %lu, [main]: pthread_barrier_init done.\n", time(NULL));

	for (i = 0; i < THREAD_NUMBER; i++){
		arg = i;
		pthread_create( &pid[i], NULL, &thread, (void *) &arg );
	}

	sleep(3);

	for (i = 0; i < THREAD_NUMBER; i++){
		pthread_join(pid[i], NULL);
	}

	printf("Time: %lu, [main]: pthread well done.\n", time(NULL));
	pthread_barrier_destroy(&barrier);

	return 0;
}
