/* An example of pthread mutex attr using PTHREAD_MUTEX_RECURSIVE */



// clock_nanosleep - high-resolution sleep with specifiable clock
//
// int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *request, struct timespec *remain);
//
// clock_nanosleep() allows the calling thread to sleep for an interval specified with nanosecond
// precision. It differs in allowing the caller to select the clock against which the sleep interval
// is to be measured, and in allowing the sleep interval to be specified as either an absolute or a
// relative value.
//
// struct timespec {
// 	time_t tv_sec;        /* seconds */
// 	long   tv_nsec;       /* nanoseconds */
// };
//
// clock_id flag:
//
// CLOCK_REALTIME, A settable system-wide real-time clock.


#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

#define SEC_TO_NSEC 1000000000			// seconds to nanoseconds

int make_thread(void *(*) (void *), void *);

struct to_info
{
	void (* to_fn) (void *);		// function
	void *to_arg;				// argument
	struct timespec to_wait;		// time to wait
};

void timeout_helper(void *arg)
{
	struct to_info *tip;

	tip = (struct to_info *) arg;
	clock_nanosleep(CLOCK_REALTIME, 0, &tip -> to_wait, NULL);
	(*tip -> to_fn) (tip -> to_arg);

	free(arg);

	return 0;
}

void timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
	int err;
	struct timespec now;
	struct to_info *tip;

	clock_gettime(CLOCK_REALTIME, &now);
	if ( (when -> tv_sev > now.tv_sec) || (when -> tv_sev == now.tv_sec) ){

		tip = mallock(sizeof(struct to_info));
		if (tip != NULL){
			tip -> to_fu =func;
			tip -> to_arg = arg;
			tip -> to_wait.tv_sec = when -> tv_sec - now.tv_sec;

			if( when -> tv_nsec >= now.tv_nsec ){
				tip -> to_wait.tv_nsec = when -> tv_nsec - now.tv_nsec;
			}else{
				tip -> to_wait.tv_nsec --;
				tip -> to_wait.tv_nsec = SEC_TO_NSEC - now.tv_nsec + when -> tv_nsec;
			}
		}
		err = make_thread(timeout_helper, (void *)tip);
		if ( err == 0 ){
			return 0;
		}else{
			free(tip);
		}

	}
}


pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void retry(void *arg)
{
	pthread_mutex_lock(&mutex);

	pthread_mutex_unlock(&mutex);

}

int main()
{

}
