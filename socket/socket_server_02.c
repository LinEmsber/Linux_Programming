/* An example of socket server, file transfer server */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define SERVERPORT 8888
#define MAXBUF 1024

int main()
{
	int return_status;
	int socket1, socket2;
	int addrlen;
	struct sockaddr_in file_trans_server, file_trans_client;

	// create a socket
	socket1 = socket(AF_INET, SOCK_STREAM, 0);
	if (socket1 == -1)	{
		fprintf(stderr, "Could not create socket!\n");
		exit(1);
	}

	// bind to a socket, use INADDR_ANY for all local addresses	file_trans_server.sin_family = AF_INET;
	file_trans_server.sin_addr.s_addr = INADDR_ANY;
	file_trans_server.sin_port = htons(SERVERPORT);

	return_status = bind(socket1, (struct sockaddr*)&file_trans_server, sizeof(file_trans_server));
	if (return_status == -1)	{
		fprintf(stderr, "Could not bind to socket!\n");
		exit(1);
	}

	return_status = listen(socket1, 5);
	if (return_status == -1)	{
		fprintf(stderr, "Could not listen on socket!\n");
		exit(1);
	}

	for(;;)	{

		int fd;
		int i, read_counter, write_counter;
		char* bufptr;
		char buf[MAXBUF];
		char filename[MAXBUF];

		// wait for an incoming connection
		addrlen = sizeof(file_trans_client);

		// use accept() to handle incoming connection requests, and free up
		// the original socket for other requests.
		socket2 = accept(socket1, (struct sockaddr*)&file_trans_client, &addrlen);
		if (socket2 == -1){
			fprintf(stderr, "Could not accept connection!\n");
			exit(1);
		}

		// get the filename from the client over the socket
		i = 0;
		if ((read_counter = read(socket2, filename + i, MAXBUF)) > 0){
			i += read_counter;
		}

		if (read_counter == -1){
			fprintf(stderr, "Could not read filename from socket!\n");
			close(socket2);
			continue;
		}

		filename[i+1] = '\0';
		printf("Reading file %s\n", filename);

		// open the file for reading
		fd = open(filename, O_RDONLY);
		if (fd == -1){
			fprintf(stderr, "Could not open file for reading!\n");
			close(socket2);
			continue;
		}


		// reset the read counter
		read_counter = 0;
		// read the file, and send it to the client in chunks of size MAXBUF
		while((read_counter = read(fd, buf, MAXBUF)) > 0){

			write_counter = 0;
			bufptr = buf;

			while (write_counter < read_counter){

				read_counter -= write_counter;
				bufptr += write_counter;
				write_counter = write(socket2, bufptr, read_counter);

				if (write_counter == -1){
					fprintf(stderr, "Could not write file to client!\n");
					close(socket2);
					continue;
				}
			}
		}
	// for loop
	}

	close(fd);
	close(socket2);
	close(socket1);

	return 0;
}
