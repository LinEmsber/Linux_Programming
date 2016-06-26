/* An example of atexit() */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// sysconf - get configuration information at run time
//
// long sysconf(int name);
//
// POSIX allows an application to test at compile or run time whether certain options
// are supported, or what the value is of certain configurable constants or limits.



// atexit - register return_stexit_max function to be called at normal process termination
//
// int atexit(void (*function)(void));
//
// The atexit() function registers the given function to be called at normal process
// termination, either via exit(3) or via return from the program's main().
//
// Functions so registered are called in the reverse order of their registration; no
// arguments are passed.
//
// The atexit() function returns the value 0 if successful.

void bye(void)
{
	printf("hello, this is bye function.\n");
}

int main(void)
{

	int i;
	long return_stexit_max;
	long return_host_name_max;
	long return_clk_tck;



	// POSIX.1 requires that an implementation allow at least ATEXIT_MAX (32) such
	// functions to be registered.
	return_stexit_max = sysconf(_SC_ATEXIT_MAX);
	printf("ATEXIT_MAX = %ld\n", return_stexit_max);



	// Maximum length of a hostname, not including the terminating null byte.
	return_host_name_max = sysconf(_SC_HOST_NAME_MAX);
	printf("_SC_HOST_NAME_MAX = %ld\n", return_host_name_max);



	// The number of clock ticks per second.
	return_clk_tck = sysconf(_SC_CLK_TCK);
	printf("_SC_CLK_TCK = %ld\n", return_clk_tck);


	i = atexit(bye);
	if (i != 0) {
		perror("atexit");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
