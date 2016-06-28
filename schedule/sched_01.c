/* An example of sched_setscheduler() */

// How to compile this code:
//
// $ gcc sched_01.c
//
// $ gcc sched_01.c -DSCHED_BATCH
//
// $ gcc sched_01.c -DSCHED_BATCH -DSCHED_IDLE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sched.h>


// strchr - locate character in string
//
// char *strchr(const char *s, int c);
//
// The strchr() function returns a pointer to the first occurrence of the character
// c in the string s.



// int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param);
//
// The sched_setscheduler() system call sets both the scheduling policy and parameters
// for the thread whose ID is specified in pid. If pid equals zero, the scheduling policy
// and parameters of the calling thread will be set.
//
// param:
// 	struct sched_param {
// 	    ...
// 	    int sched_priority;
// 	    ...
// 	};
//
//
// policy:
// SCHED_OTHER   the standard round-robin time-sharing policy;
// SCHED_BATCH   for "batch" style execution of processes; and
// SCHED_IDLE    for running very low priority background jobs.
// SCHED_FIFO    a first-in, first-out policy; and
// SCHED_RR      a round-robin policy.


int main(int argc, char *argv[])
{

	int i;
	int policy;
	int ret_setscheduler;
	struct sched_param parameter;

	if (argc < 3 || strchr("rfobi", argv[1][0]) == NULL){
	        printf("%s policy priority [pid...]\n"
	                "    policy is 'r' (RR), 'f' (FIFO), "

#ifdef SCHED_BATCH
	                "'b' (BATCH), "
#endif

#ifdef SCHED_IDLE
			"'i' (IDLE), "
#endif
			"or 'o' (OTHER)\n",
			argv[0]);

			exit(EXIT_SUCCESS);
	}

	policy = (argv[1][0] == 'r') ? SCHED_RR : (argv[1][0] == 'f') ? SCHED_FIFO :

#ifdef SCHED_BATCH
	(argv[1][0] == 'b') ? SCHED_BATCH :
#endif

#ifdef SCHED_IDLE
        (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
		SCHED_OTHER;


	parameter.sched_priority = atoi(argv[2]);

	for (i = 3; i < argc; i++){

		ret_setscheduler = sched_setscheduler(atoi(argv[i]), policy, &parameter);
		if (ret_setscheduler == -1){
			perror("sched_setscheduler");
		}
	}


	exit(EXIT_SUCCESS);
}
