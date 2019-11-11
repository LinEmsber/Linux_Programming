/* The example of the pthread_barrier_t */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int num_1, num_2, num_3, num_4;

pthread_barrier_t barrier;

void * thread_1()
{
	sleep(2);

	printf("Enter value for thread 1:  ");
	scanf("%d", &num_1);
	pthread_barrier_wait(&barrier);

	printf("\nvalues entered by the threads are  %d %d %d %d \n", num_1, num_2, num_3, num_4);

	return NULL;
}


void * thread_2()
{
	sleep(4);

	printf("Enter value for thread 2:  ");
	scanf("%d", &num_2);
	pthread_barrier_wait(&barrier);

	printf("\nvalues entered by the threads are  %d %d %d %d \n", num_1, num_2, num_3, num_4);

	return NULL;
}

void * thread_3()
{
	sleep(6);

	printf("Enter value for thread 3:  ");
	scanf("%d", &num_3);
	pthread_barrier_wait(&barrier);

	printf("\nvalues entered by the threads are  %d %d %d %d \n", num_1, num_2, num_3, num_4);

	return NULL;
}

void * thread_4()
{
	sleep(8);

	printf("Enter value for thread 4:  ");
	scanf("%d", &num_4);
	pthread_barrier_wait(&barrier);

	printf("\nvalues entered by the threads are  %d %d %d %d \n", num_1, num_2, num_3, num_4);

	return NULL;
}


int main()
{
	int ret;
	pthread_t tid_1, tid_2, tid_3, tid_4;

	pthread_barrier_init(&barrier, NULL, 4);

	ret = pthread_create(&tid_1, NULL, &thread_1, NULL);
	if(ret != 0)
		printf("Unable to create thread1");


	ret = pthread_create(&tid_2, NULL, &thread_2, NULL);
	if(ret != 0)
		printf("Unable to create thread2");


	ret = pthread_create(&tid_3, NULL, &thread_3, NULL);
	if(ret != 0)
		printf("Unable to create thread3");


	ret = pthread_create(&tid_4, NULL, &thread_4, NULL);
	if(ret != 0)
		printf("Unable to create thread4");

	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);
	pthread_join(tid_3, NULL);
	pthread_join(tid_4, NULL);

	pthread_barrier_destroy(&barrier);

	return 0;
}
