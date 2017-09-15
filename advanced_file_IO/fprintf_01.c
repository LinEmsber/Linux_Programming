#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main()
{
	unsigned int i;
	char buffer[20];
	FILE * fp;

	struct tm* tm_info;
	struct timeval tv;

	memset(&buffer, 0, sizeof(char)*20);

	fp = fopen ("./fprintf_file.txt", "w+");

	for (i = 0; i < 10; i++){
		gettimeofday(&tv, NULL);
		tm_info = localtime(&tv.tv_sec);
		strftime(buffer, 26, "%Y:%m:%d %H:%M:%S\n", tm_info);

		fprintf(fp, "%s", buffer);
//		printf("%s\n", buffer);

		usleep(100000);
	}
	fclose(fp);

	return(0);
}
