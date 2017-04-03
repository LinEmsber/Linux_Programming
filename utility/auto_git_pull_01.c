/* Automatically git pull for each folder of current directory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

int dir_dir_list (char * list[], char *dir_path)
{
        int i;
        DIR *dp;
        struct dirent *ep;
        
	dp = opendir (dir_path);

        for (i = 0; ( ep = readdir (dp) ); i++){

                // except main, current dir, previous dir.
                if ( strcmp(".", ep->d_name) == 0 || strcmp("..", ep->d_name) == 0 || strcmp("./a.out", ep->d_name) ){
                        i--;
                        continue;
                }

                // limit the file name length.
                list[i] = (char*) malloc (sizeof(ep->d_name));
                strcpy( list[i], ep->d_name);
        }

        closedir (dp);
        return i;
}


int main( int argc, char *argv[] )
{
	int i;
	FILE *fp;

	char * dir_list[20];
	int dir_n;

	char command[64];

	dir_n = dir_dir_list(dir_list, "./");
	printf("dir number: %d\n", dir_n);
	for (i = 0; i < dir_n; i++){

		if ( dir_list[i] == NULL )
			break;
		else{
			printf("address of dir_list[%d]: %p, file name: %s\n", i, dir_list[i], dir_list[i]);
			sprintf( command, "%s%s", "cd ", dir_list[i]);
			printf("%s\n", command);
		}
		system(command);
		system("pwd");
		system("cd ");
	}


	return 0;
}
