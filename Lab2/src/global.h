#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

extern char *ip;
extern int *port;
extern char *upstream_url;
extern int *upstream_port;
extern int *thread_num;
extern bool proxy_mode;

#endif // GLOBAL_HPP