/* A simple program save the file's name under a specific directory into a char list */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_FILE_NUMBER 2048
#define MAX_FILE_NAME_SIZE 64

int dir_file_name_list (char *list[], char *dir_path){
	DIR *dp;
	struct dirent *ep;
	int i;

	// open
	dp = opendir (dir_path);

	for (i = 0; ( ep = readdir (dp) ); i++){

		// except main, current dir, previous dir.
		if ( strcmp(".", ep->d_name) == 0|| strcmp("..", ep->d_name) == 0 ){
			i--;
			continue;

		// // To exclude some files.
		// } else if (strcmp("main", ep->d_name) == 0 || strcmp("log.txt", ep->d_name) == 0){
		// 	i--;
		// 	continue;
		//

		}

		// limit the file name length.
		list[i] = (char*) malloc (sizeof(ep->d_name));
		strcpy( list[i], ep->d_name);
	}

	// close
	closedir (dp);
	return i;
}


int main (void){

	int i;
	int file_num;
	char *local = "./";
	char *file_name_list[MAX_FILE_NUMBER];

	file_num = dir_file_name_list(file_name_list, local);

	for (i=0; i<file_num; i++){
		printf("address of file_name_list[%d]: %p, file name: %s\n", i, file_name_list[i], file_name_list[i]);
	}

	// free
	for(i=0; i<file_num; i++){
		free(file_name_list[i]);
	}

	return 0;
}
