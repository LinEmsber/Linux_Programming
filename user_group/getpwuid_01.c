/* An example of getpwuid() */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <pwd.h>

int main( void )
{
	struct passwd *pwd;

	pwd = getpwuid( getuid() );
	if( pwd == NULL ) {
		fprintf( stderr, "getpwuid: no password entry\n" );
		return( EXIT_FAILURE );
	}

	printf( "login name  %s\n", pwd -> pw_name );
	printf( "user id     %d\n", pwd -> pw_uid );
	printf( "group id    %d\n", pwd -> pw_gid );
	printf( "home dir    %s\n", pwd -> pw_dir );
	printf( "login shell %s\n", pwd -> pw_shell );

	return( EXIT_SUCCESS );
}
