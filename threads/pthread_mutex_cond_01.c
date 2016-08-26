/* An example of pthread_cond_wait() */

#include <pthread.h>

struct msg
{
	struct msg *next;
}


struct msg *work;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void process_msg(void)
{
	struct msg *m;

	for(;;){

		pthread_mutex_lock(&lock);

		while(work == NULL){
			pthread_cond_wait(&cond, &lock);
		}

		m = work;
		work = m -> next;

		pthread_mutex_unlock(&lock);

	}
}

void enquue_msg(struct msg *m)
{
	pthread_mutex_lock(&lock);

	m -> next = work;
	work = m;

	pthread_mutex_unlock(&lock);

	pthread_cond_signal(&cond);
}
