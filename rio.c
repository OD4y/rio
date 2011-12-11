#include "rio.h"

ssize_t
readn(int fd , void *buf, size_t nbyte) {
  ssize_t nread;  // number of bytes read
  size_t  nleft = nbyte;  // number of bytes left
  char* bufp = buf; // convert to a byte pointer

  while (nleft > 0)
    if ((nread = read(fd, bufp, nleft)) < 0) {  // an error occurred
      if (ERRNO != EINTR) return -1;
    } else if (nread == 0) {  // EOF
      break;
    } else {
      nleft -= nread;
      bufp  += nread;
    }

  return (nbyte - nleft);
}

ssize_t
writen(int fd , void *buf, size_t nbyte) {
  ssize_t nwritten; // number of bytes written
  size_t  nleft = nbyte;  // number of bytes left
  char* bufp = buf; // convert to a byte pointer

  while (nleft > 0)
    if ((nwritten = write(fd, bufp, nleft)) < 0) {  // an error occurred
      if (ERRNO != EINTR) return -1;
    } else {  // write never encounters an EOF condition
      nleft -= nwritten;
      bufp  += nwritten;
    }

  return nbyte; // write never 'short-counts'
}
// vim: et:ai:sw=2:ts=2
