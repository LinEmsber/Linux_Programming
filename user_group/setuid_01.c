/* An example of setuid () */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


// setuid - set user identity
//
// int setuid(uid_t uid);
//
// setuid() sets the effective user ID of the calling process.
//
// On success, zero is returned.


// setgid - set group identity
//
// int setgid(gid_t gid);
//
// setgid() sets the effective group ID of the calling process.
//
// On success, zero is returned.

int main()
{

	// To watch the process in the process list.
	printf("sleep\n");
	usleep(1 * 1000 * 1000);

	uid_t uid = 1000; // user
	gid_t gid = 100;  // users

	printf("setuid: %d\n", uid);
	setuid(uid);

	printf("sleep\n");
	usleep(1 * 1000 * 1000);

	printf("setgid: %d\n", gid);
	setgid(gid);

	printf("exit\n");
}
