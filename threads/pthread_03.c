/*
 * Shows the problem with using an automatic variable(allocated on the stack)
 * as the argument to pthread_exit()
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>

struct entry
{
	int a, b, c, d;
};

void print_entry ( const char *s, const struct entry *fe)
{
	printf("%s", s);
	printf("\tstructure at 0x%lx\n", (unsigned long) fe);
	printf("\tentry.a = %d\n", fe -> a);
	printf("\tentry.b = %d\n", fe -> b);
	printf("\tentry.c = %d\n", fe -> c);
	printf("\tentry.d = %d\n", fe -> d);
}

struct entry test_entry;

void *thread_fun_1(void *arg)
{
	struct entry e = {1, 2, 3, 4};

	test_entry = e;

	print_entry("thread 1: \n", &test_entry);
	pthread_exit( (void *) &test_entry  );
}

void *thread_fun_2(void *arg)
{
	struct entry e = {100, 200, 300, 400};
	test_entry = e;

	print_entry("thread 2: \n", &test_entry);
	printf("thread 2: ID is %lu\n", pthread_self() );
	pthread_exit( (void *) &test_entry );
}

int main()
{
	int err;
	pthread_t tid_1, tid_2;
	struct entry *fe;

	// thread 1
	err = pthread_create(&tid_1, NULL, thread_fun_1, NULL);
	if (err != 0){
		perror("pthread_create");
	}

	err = pthread_join(tid_1, (void *) &fe);
	if (err != 0){
		perror("pthread_join");
	}

	sleep(1);
	printf("\n");
	// printf("parent starting second thread\n");


	// thread 2
	err = pthread_create(&tid_2, NULL, thread_fun_2, NULL);
	if (err != 0){
		perror("pthread_create");
	}

	err = pthread_join(tid_2, (void *) &fe);
	if (err != 0){
		perror("pthread_join");
	}
	sleep(1);

	return 0;
}
