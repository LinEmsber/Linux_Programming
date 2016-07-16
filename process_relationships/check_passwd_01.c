/* Authenticating a user against the shadow password file */

// Enter the user name, and then enter the corresponding password.
// If both of them is correct, print out the user ID.


#define _BSD_SOURCE     /* Get getpass() declaration from <unistd.h> */
#define _XOPEN_SOURCE   /* Get crypt() declaration from <unistd.h> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <limits.h>
#include <pwd.h>			// struct passwd
#include <shadow.h>			// struct spwd

// struct passwd {
//     char   *pw_name;       /* username */
//     char   *pw_passwd;     /* user password */
//     uid_t   pw_uid;        /* user ID */
//     gid_t   pw_gid;        /* group ID */
//     char   *pw_gecos;      /* user information */
//     char   *pw_dir;        /* home directory */
//     char   *pw_shell;      /* shell program */
// };
//
// struct spwd {
//     char *sp_namp;     /* Login name */
//     char *sp_pwdp;     /* Encrypted password */
//     long  sp_lstchg;   /* Date of last change
// 			  (measured in days since
// 			  1970-01-01 00:00:00 +0000 (UTC)) */
//     long  sp_min;      /* Min # of days between changes */
//     long  sp_max;      /* Max # of days between changes */
//     long  sp_warn;     /* # of days before password expires
// 			  to warn user to change it */
//     long  sp_inact;    /* # of days after password expires
// 			  until account is disabled */
//     long  sp_expire;   /* Date when account expires
// 			  (measured in days since
// 			  1970-01-01 00:00:00 +0000 (UTC)) */
//     unsigned long sp_flag;  /* Reserved */
// };


// crypt, password and data encryption
//
// char *crypt(const char *key, const char *salt);
//
// crypt() is the password encryption function. It is based on the Data Encryption
// Standard algorithm with variations intended to discourage use of hardware
// implementations of a key search.



int main(int argc, char *argv[])
{

	char *username, *password, *encrypted, *p;
	int is_auth_ok;
	size_t len;
	int login_name_max;

	struct passwd *pwd;
	struct spwd *spwd;

	// If limit is indeterminate, make a guess.
	login_name_max = sysconf(_SC_LOGIN_NAME_MAX);
	if (login_name_max == -1){
		login_name_max = 256;
	}

	username = malloc(login_name_max);
	if (username == NULL){
		perror("malloc");
	}


	printf("Username: ");
	fflush(stdout);

	if (fgets(username, login_name_max, stdin) == NULL){
		exit(EXIT_FAILURE);
	}


	len = strlen(username);
	if (username[len - 1] == '\n'){
		username[len - 1] = '\0';       /* Remove trailing '\n' */
	}


	pwd = getpwnam(username);
	if (pwd == NULL){
		perror("couldn't get password record");
	}


	spwd = getspnam(username);

	if (spwd == NULL && errno == EACCES){
		perror("no permission to read shadow password file");
	}

	// If there is a shadow password record, using the shadow password
	if (spwd != NULL){
		pwd->pw_passwd = spwd->sp_pwdp;
	}


	// Encrypt password and erase cleartext version immediately
	password = getpass("Password: ");


	encrypted = crypt(password, pwd->pw_passwd);
	for (p = password; *p != '\0'; ){
		*p++ = '\0';
	}

	if (encrypted == NULL){
		perror("crypt");
	}


	// check the password.
	is_auth_ok = strcmp(encrypted, pwd->pw_passwd) == 0;
	if (!is_auth_ok) {

		printf("Incorrect password\n");
		exit(EXIT_FAILURE);
	}else{

		printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);
	}

	return 0;
}
