/* A sample exmple of prthread_cond_signal() */



// The common use of condition variable is something like:
//
// int condition = 0;
//
// void thread()
// {
// 	pthread_mutex_lock(&mutex);
//
//	// wait for the condition becoming true.
// 	while (condition){
// 		pthread_cond_wait(&cond, &mutex);
// 	}
//
// 	pthread_mutex_unlock(&mutex);
// }
//
//
//
// void wakeup_thread()
// {
// 	pthread_mutex_lock(&mutex);
//
// 	// do something that might make condition true.
// 	condition = 1;
// 	if (condition == 1){
// 		pthread_cond_signal(&cond);
// 	}
//
// 	pthread_mutex_unlock(&mutex);
// }
//

#include <stdio.h>

#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int counter = 0;

void *compute(void *arg)
{
	pthread_mutex_lock(&mutex);

	pthread_cond_wait(&cond, &mutex);

	pthread_mutex_unlock(&mutex);

	return NULL;
}

void wakeup_thread()
{
	pthread_mutex_lock(&mutex);

	counter ++;
	pthread_cond_signal(&cond);

	pthread_mutex_unlock(&mutex);
}


int main(int argc, char *argv[])
{
	int i;
	int number = atoi(argv[1]);
	pthread_t tids[number];

	// multi thread
	for (i = 0; i < number; i++){
		pthread_create(&tids[i], NULL, compute, NULL);
	}

	// sleep
	sleep(1);

	// wake up
	printf("Start to wakeup the thread which with pthread_cond_wait()\n");
	printf("Using wakeup_thread\n");

	// wakeup thread
	for(i = 0; i< number; i++){

		wakeup_thread();

	}

	printf("counter: %d\n", counter);

	return 0;
}
