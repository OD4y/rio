#ifndef __LRIO__
#define __LRIO__

#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef struct {
  int fd; // input source
  char *next; // next unread byte
  char buf[0x1000];  // internal buffer, 4KB
  ssize_t cnt;
} rio_t;

extern ssize_t rio_readn(int fd , void *buf, size_t nbyte);
extern ssize_t rio_writen(int fd , void *buf, size_t nbyte);

extern void rio_init(rio_t *rp, int fd);
extern ssize_t rio_readnb(rio_t *rp, void *buf, size_t nbyte);
extern ssize_t rio_readlineb(rio_t *rp, void *buf, size_t maxlen);

#endif
