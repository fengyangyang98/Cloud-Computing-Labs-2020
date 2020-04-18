#ifndef GLOBAL_HPP
#define GLOBAL_HPP

// Linux
#include <unistd.h>
#include <linux/limits.h>
#include <sys/time.h>
#include <syscall.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/mman.h>
#include <pthread.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>

#include <errno.h>

// CPP
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>

#define SE_OK       0
#define SE_TIMEOUT  -1
#define SE_EINTR    -2
#define SE_EAGAIN   -3


extern char *ip;
extern int *port;
extern char *upstream_url;
extern int *upstream_port;
extern int *thread_num;
extern bool proxy_mode;

#endif // GLOBAL_HPP