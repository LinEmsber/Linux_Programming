/* partial writes */


// For regular files, you do not need to perform writes in a loop. However, for some type
// files, such like sockets, a loop may be required to guarantee that you really write out
// all of the requeseted bytes.
// Another benefit of using a loop is that a second call to write() may return an error
// revealing what caused the first call to perform only a partial write.

ssize_t ret, nr;

while ( len != 0 && (ret = write(fd, buf, len)) != 0 ){

	if (ret == -1){
		if (errno == EINTR){
			continue;
		}

		perror("wreite");
		break;
	}

	len -= ret;
	buf += ret;
}
