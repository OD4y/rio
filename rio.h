#ifndef __LRIO__
#define __LRIO__

#include <unistd.h>
#include <errno.h>
#include <string.h>

extern ssize_t readn(int fd , void *buf, size_t nbyte);
extern ssize_t writen(int fd , void *buf, size_t nbyte);

#endif
