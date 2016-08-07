/* An example of async IO */

// Use dmesg to check which /dev/ttyS* is serial port
// > dmesg | grep tty


// Usage:
//
// terminal 1:
// > sudo a.out
//
// terminal 2:
// > sudo screen /dev/ttyS* 115200
// > (enter any key)



// F_SETOWN:
//
// Set the process ID or process group ID that will receive SIGIO and SIGURG signals
// for events on the file descriptor fd. The target process or process group ID is
// specified in arg. A process ID is specified as a positive value; a process group
// ID is specified as a negative value.

// FAYNC:
//
// This is necessary for the kernel to know just whom to notify. In order to actually
// enable asynchronous notification, the user programs must set the FASYNC flag in the
// device by means of the F_SETFL fcntl command.

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

// STOP flag for while loop
volatile int STOP = FALSE;

// TRUE while no signal received
int wait_flag = TRUE;

// definition of signal handler
void signal_handler_IO (int status)
{
	printf("received SIGIO signal.\n");
	wait_flag = FALSE;
}



int main()
{
	int fd;
	int rn;
	char buf[255];

	struct termios old_tio,new_tio;
	struct sigaction sa_io;


	// open the device to be non-blocking (read will return immediatly)
	fd = open(MODEM_DEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) {
		perror(MODEM_DEVICE);
		exit(-1);
	}

	// install the signal handler before making the device asynchronous
	sa_io.sa_handler = signal_handler_IO;
	sa_io.sa_flags = 0;
	sigaction(SIGIO, &sa_io, NULL);

	// allow the process to receive SIGIO
	fcntl(fd, F_SETOWN, getpid());

	// Make the file descriptor asynchronous
        fcntl(fd, F_SETFL, FASYNC);

	// get  and set terminal attributes

	// save current port settings
	tcgetattr(fd, &old_tio);
	// set new port settings for canonical input processing
	new_tio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	new_tio.c_iflag = IGNPAR | ICRNL;
	new_tio.c_oflag = 0;
	new_tio.c_lflag = ICANON;
	new_tio.c_cc[VMIN]=1;
	new_tio.c_cc[VTIME]=0;

	// flush and set the new setting
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &new_tio);


	// loop while waiting for input. normally we would do something useful here.
	while (STOP == FALSE) {

		printf(".\n");
		sleep(1);

		// After receiving SIGIO, wait_flag = FALSE, input is available and can be read
		if (wait_flag == FALSE) {

			rn = read(fd, buf, 255);
			buf[rn] = '\0';
			printf("buf: %s, rn: %d\n", buf, rn);

			if (rn == 1){
				STOP = TRUE;
			}

			// wait for new input
			wait_flag = TRUE;
		}
	}


	// restore old port settings
	tcsetattr(fd,TCSANOW, &old_tio);

	return 0;
}
