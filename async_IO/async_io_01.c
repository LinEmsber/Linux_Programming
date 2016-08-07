/* An example of async IO */

// Use dmesg to check which /dev/ttyS* is serial port
// > dmesg | grep tty

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/signal.h>
#include <sys/types.h>

#include <termios.h>

#define FALSE 0
#define TRUE 1

#define BAUDRATE B115200
#define MODEM_DEVICE "/dev/ttyS0"

volatile int STOP = FALSE;

// definition of signal handler
void signal_handler_IO (int status)
{
	printf("received SIGIO signal.\n");
	wait_flag = FALSE;
}


// TRUE while no signal received
int wait_flag = TRUE;

int main()
{
	int fd;
	char buf[255];

	struct termios old_tio,new_tio;
	struct sigaction sa_io;


	// open the device to be non-blocking (read will return immediatly)
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) {
		perror(MODEMDEVICE); exit(-1);
	}
}
