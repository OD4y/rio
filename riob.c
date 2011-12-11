#include "rio.h"


// initializing a rio_t
void
rio_init(rio_t *rp, int fd) {
	rp->fd	= fd;
	rp->cnt	= 0;
	rp->next = rp->buf;
}

// buffered version of read system call
// the semantic of rio_read is the same as read
#define	MIN(a, b)	((a) < (b) ? (a) : (b))
static
ssize_t rio_read(rio_t *rp, char *buf, size_t nbyte) {
	while (rp->cnt <= 0) {	// refill if buf is empty
		rp->cnt = read(rp->fd, rp->buf, sizeof(rp->buf));
		if (rp->cnt < 0) {
			if (errno != EINTR) return -1;
		} else if (rp->cnt == 0) {	// EOF
			return 0;
		} else {
			rp->next = rp->buf;	// reset next unread pointer
			break;
		}
	}

	size_t cnt = MIN(nbyte, rp->cnt);
	memcpy(buf, rp->next, cnt);
	
	rp->next	+= cnt;
	rp->cnt		-= cnt;

	return cnt;
}
#undef	MIN


// read at most (maxlen -1)bytes sized textline from rp to buf
ssize_t
rio_readlineb(rio_t *rp, void *buf, size_t maxlen) {
	int n, rc;
	char c, *bufp = (char*)buf;

	for (n = 1; n < maxlen; n++)
		if ((rc = rio_read(rp, &c, 1)) == 1) {
			*bufp++ = c;
			if (c == '\n') break;
		} else if (rc == 0) {
			if (n == 1) return 0;	// EOF, no data read
			else break;	// EOF, some data was read
		} else {
			return -1;
		}

	*bufp = 0;	// append NULL to the string
	return n;
}

// read nbyte from rp, automatically restart interrupted syscall
ssize_t
rio_readnb(rio_t *rp, void *buf, size_t nbyte) {
	ssize_t nread;
	size_t	nleft = nbyte;
	char *bufp = (char*)buf;

	while (nleft > 0)
		if ((nread = rio_read(rp, bufp, nleft)) < 0) {
			if (errno != EINTR) return -1;
		} else if (nread == 0) {	// EOF
			break;
		} else {
			nleft	-= nread;
			bufp	+= nread;
		}

	return (nbyte - nleft);
}
