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
	printf("structure at 0x%lx\n", (unsigned long) fe);
	printf("entry.a = %d\n", fe -> a);
	printf("entry.b = %d\n", fe -> b);
	printf("entry.c = %d\n", fe -> c);
	printf("entry.d = %d\n", fe -> d);
}

void thread_fun_1(void *arg)
{
	struct entry e = {1, 2, 3, 4};

	print_entry("thread 1: \n", &e);
	pthread_exit( (void *) &e  );
}

void thread_fun_2(void *arg)
{
	struct entry e = {1, 2, 3, 4};

	print_entry("thread 2: ID is %lu", (unsigned long)pthread_self() );
	pthread_exit( (void *) 0 );
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
	printf("parent starting second thread\n");


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

	print_entry("parent: \n", fe);

	return 0;
}
