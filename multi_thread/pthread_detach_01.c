/* An example of pthread_join() and pthread_detach().
 *
 * pthread_join():
 * To suspend execution of the calling thread until the target thread terminates, unless the target
 * thread has already terminated.
 *
 * pthread_detach():
 * When a detached thread terminates, its resources are automatically relclaimed back to the system.
 *
 * MACRO:
 * 	_L: use pthread_mutex_lock()
 *	_J: use ptrhead_join()
 *	_D: use pthread_detach()
 *	_S: Add sleep(2) to check whether the mutex_lock() works normally when we use pthread_detach().
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 2000
#define ADD_COUNT 100

int global = 0;

pthread_mutex_t mutex;
pthread_t tids[THREAD_NUM];

void *compute(void *arg)
{
        int i;

        for (i = 0; i < ADD_COUNT * ADD_COUNT; i++) {
#ifdef _L
		pthread_mutex_lock(&mutex);
#endif
		global++;
#ifdef _L
		pthread_mutex_unlock(&mutex);
#endif
        }

        return NULL;
}

void multi_thread_test()
{
        int i;

        global = 0;

#ifdef _L
        pthread_mutex_init(&mutex, NULL);
#endif

        for (i = 0; i < THREAD_NUM; i++)
                pthread_create(&tids[i], NULL, compute, NULL);

#ifdef _J
        for (i = 0; i < THREAD_NUM; i++){
                pthread_join(tids[i], NULL);
	}
#elif _D
        for (i = 0; i < THREAD_NUM; i++){
                pthread_detach(tids[i]);
	}
#endif


#ifdef _S
        /* set the sleep time for waiting threads to finish. */
	sleep(2);
#endif

        pthread_mutex_destroy(&mutex);

}

double time_diff_in_second(struct timespec t1, struct timespec t2)
{
	struct timespec diff;

	if (t2.tv_nsec - t1.tv_nsec < 0) {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
	} else {
		diff.tv_sec  = t2.tv_sec - t1.tv_sec;
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
	}

	return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}



int main()
{
        double time_diff;
        struct timespec start, end;

        clock_gettime(CLOCK_REALTIME, &start);
        multi_thread_test();
        clock_gettime(CLOCK_REALTIME, &end);

	printf("calculated global: %d\n", global);
	printf("expected global: %d\n", ADD_COUNT * ADD_COUNT * THREAD_NUM);

        time_diff = time_diff_in_second(start, end);
        printf("time difference in sencond: %lf\n", time_diff);

        return 0;
}
