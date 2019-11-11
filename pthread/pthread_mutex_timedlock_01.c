/* An example of Using pthread_mutex_timedlock */

// case 1, unlock the mutex:
// $gcc pthread_mutex_timedlock_01.c -lpthread -D_UNLOCK
//
// case 2, do not unlock the mutex:
// $gcc pthread_mutex_timedlock_01.c -lpthread


#include <stdio.h>
#include <errno.h>

#include <time.h>

#include <pthread.h>


// struct timespec {
//     time_t   tv_sec;        /* seconds */
//     long     tv_nsec;       /* nanoseconds */
// };
//
// struct tm {
//     int tm_sec;    /* Seconds (0-60) */
//     int tm_min;    /* Minutes (0-59) */
//     int tm_hour;   /* Hours (0-23) */
//     int tm_mday;   /* Day of the month (1-31) */
//     int tm_mon;    /* Month (0-11) */
//     int tm_year;   /* Year - 1900 */
//     int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
//     int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
//     int tm_isdst;  /* Daylight saving time */
// };
//
// struct tm *localtime(const time_t *timep);
//
// The localtime() function converts the calendar time timep to broken-down time
// representation, expressed relative to the user's specified timezone.


// strftime - format date and time
//
// size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);
//
// The strftime() function formats the broken-down time tm according to the format specification
// format and places the result in the character array s of size max.
//
// %r     The time in a.m. or p.m. notation.  In the POSIX locale this
//        is equivalent to %I:%M:%S %p.  (SU)


// pthread_mutex_timedlock — lock a mutex
//
// int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
//
// The pthread_mutex_timedlock() function shall lock the mutex object referenced by mutex.
//
// If the mutex is already locked, the calling thread shall block until the mutex becomes available
// as in the pthread_mutex_lock() function. If the mutex cannot be locked without waiting for another
// thread to unlock the mutex, this wait shall be terminated when the specified timeout expires.


int main()
{

	int err;
	char buf[64];

	struct timespec time;
	struct tm *tmp;

	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


	// lock
	pthread_mutex_lock(&lock);
	printf("mutex is locked\n");

	// get the time and convert it.
	clock_gettime(CLOCK_REALTIME, &time);
	tmp = localtime(&time.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);

	// current time
	printf("current time: %s\n", buf);


#ifdef _UNLOCK
	pthread_mutex_unlock(&lock);
#else
	// do nothing
#endif

	// add 3 sec
	time.tv_sec += 3;

	// get the time again
	err = pthread_mutex_timedlock(&lock, &time);
	clock_gettime(CLOCK_REALTIME, &time);
	tmp = localtime(&time.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);

	// current time
	printf("current time: %s\n", buf);


	if(err == 0){
		printf("mutex locked again\n");
	}else{
		printf("cant lock mutex again: %d\n", strerror(err));
	}

	return 0;
}
