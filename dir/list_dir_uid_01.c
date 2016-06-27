/* list dir uid */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>

int main(void)
{
	int i ;
	DIR *dir_entry;

	// path of dir
	static const char *dir_path = "/dev/";

	// length of dir
	size_t dir_len = strlen(dir_path) + 1;

	// open dir
	dir_entry = opendir(dir_path);
	if (dir_entry == 0){

		exit(EXIT_FAILURE);

	}else if (dir_entry != 0){

		struct dirent *dirp;

		// read dir
		for (i = 0; dirp = readdir (dir_entry); i++){

			char *str = malloc(dir_len + strlen(dirp->d_name));

			if (str != 0){

				struct stat buf;
				strcpy(str, dir_path);
				strcat(str, dirp->d_name);

				// get file status
				if (stat(str, &buf) == 0){

					// The getpwuid() function returns a pointer to a structure containing
				        // the broken-out fields of the record in the password database that
				        // matches the user ID uid.

					// get file user ID of owner
					struct passwd *pwd = getpwuid(buf.st_uid);

					if (pwd != 0){
						printf("\t%s (%s)\n", str, pwd->pw_name);
					}

				}
				free(str);
			}
		}
		closedir(dir_entry);
	}

	return 0;
}
