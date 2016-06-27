/* An example of getpwnam_r() */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <pwd.h>

// getpwnam, getpwnam_r, getpwuid, getpwuid_r - get password file entry
//
// int getpwnam_r(const char *name, struct passwd *pwd, char *buf, size_t buflen, struct passwd **result);
//
// The getpwnam_r() functions obtain the same information as getpwnam(), but store
// the retrieved passwd structure in the space pointed to by pwd.

int main(int argc, char *argv[])
{
	int s;

	char *buffer;
	size_t buffer_size;

	struct passwd pwd;
	struct passwd *result;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s username\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	buffer_size = sysconf(_SC_GETPW_R_SIZE_MAX);
	printf("_SC_GETPW_R_SIZE_MAX: %ld\n", buffer_size);

	// If value was indeterminate, it should be more than enough
	if (buffer_size == -1){
		buffer_size = 16384;
	}


	buffer = malloc(buffer_size);
	if (buffer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	s =  (argv[1], &pwd, buffer, buffer_size, &result);
	
	if (result == NULL) {

		if (s == 0){
			printf("Not found\n");
		}else {
			errno = s;
			perror("getpwnam_r");
		}
		exit(EXIT_FAILURE);
	}

	printf("Name: %s; UID: %ld\n", pwd.pw_gecos, (long) pwd.pw_uid);
	exit(EXIT_SUCCESS);
}
