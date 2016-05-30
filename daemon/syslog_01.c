/* Here is an example of openlog, syslog, and closelog */

#include <stdio.h>
#include <syslog.h>

int main(){


       // The setlogmask() function sets this logmask for the calling process,
       // and returns the previous mask.  If the mask argument is 0, the
       // current logmask is not modified.

	// The eight priorities are LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR,
	// LOG_WARNING, LOG_NOTICE, LOG_INFO, and LOG_DEBUG.  The bit
	// corresponding to a priority p is LOG_MASK(p).  Some systems also
	// provide a macro LOG_UPTO(p) for the mask of all priorities in the
	// above list up to and including p.
	setlogmask (LOG_UPTO (LOG_NOTICE));

	// void openlog(const char *ident, int option, int facility);

	// openlog() opens a connection to the system logger for a program. The
        // string pointed to by ident is prepended to every message, and is
        // typically set to the program name.  If ident is NULL, the program
        // name is used.

	// option
	// The option argument to openlog() is an OR of any of these:
	//
	// LOG_CONS
	// Write directly to system console if there is an error while sending
	// to system logger.
	//
	// LOG_NDELAY
	// Open the connection immediately (normally, the connection is opened
	// when the first message is logged).
	//
	// LOG_NOWAIT
	// Don't wait for child processes that may have been created while
	// logging the message.  (The GNU C library does not create a child
	// process, so this option has no effect on Linux.)
	//
	// LOG_ODELAY
	// The converse of LOG_NDELAY; opening of the connection is delayed
	// until syslog() is called.  (This is the default, and need not be
	// specified.)
	//
	// LOG_PERROR
	// (Not in POSIX.1-2001 or POSIX.1-2008.)  Print to stderr as well.
	//
	// LOG_PID
	// Include PID with each message.
	openlog ("exampleprog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

	syslog (LOG_NOTICE, "Program started by User %d", getuid ());
	syslog (LOG_INFO, "A tree falls in a forest");


	// closelog() closes the file descriptor being used to write to the
	// system logger.  The use of closelog() is optional.
	closelog ();
}
