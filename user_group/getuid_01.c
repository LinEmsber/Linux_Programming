/* An example of getuid () */

// $ gcc setuid.c -o a.out
// $ sudo chown root a.out
// $ sudo chmod 4755 a.out

// http://theurbanpenguin.com/wp/index.php/using-a-simple-c-program-to-explain-the-suid-permission/


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int real = getuid();
        int euid = geteuid();

        printf("The REAL UID =: %d\n", real);
        printf("The EFFECTIVE UID =: %d\n", euid);

	return 0;
}
