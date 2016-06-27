/* An example of getrlimit() */

#include <stdio.h>
#include <stdint.h>

#include <sys/resource.h>

void print_out_rlimit(struct rlimit *r, const char *name)
{

	// Soft limit
	int64_t current;
	// Hard limit
	int64_t maximum;

	current = r->rlim_cur;
	maximum = r->rlim_max;

	printf("RLIMIT_%s :rlim_cur => %llx, :rlim_max => %llx\n", name, current, maximum);
}

int main(int argc, char *argv[])
{

	int i;
	struct rlimit rlim;

	int resources[] = {RLIMIT_CORE, RLIMIT_CPU, RLIMIT_DATA,
				RLIMIT_FSIZE, RLIMIT_MEMLOCK, RLIMIT_NOFILE,
				RLIMIT_NPROC, RLIMIT_RSS, RLIMIT_STACK};

	const char *names[] = {"CORE", "CPU", "DATA",
				"FSIZE", "MEMLOCK", "NOFILE",
				"NPROC", "RSS", "STACK"};

	int n = sizeof(resources) / sizeof(resources[0]);

	for (i = 0; i < n; i++){

		getrlimit(resources[i], &rlim);
		print_out_rlimit(&rlim, names[i]);
		
	}

	return 0;
}
