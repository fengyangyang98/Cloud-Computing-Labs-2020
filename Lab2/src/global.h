#pragma once

// Linux
#include <arpa/inet.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>

#include <errno.h>

#include <assert.h>

// CPP
#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using std::cout;
using std::future;
using std::queue;
using std::string;
using std::vector;

// rc code
#define SE_OK 0
#define SE_TIMEOUT -1
#define SE_EINTR -2
#define SE_EAGAIN -3

// debug util
#define PD_DEBUG(cond)                                                         \
  do {                                                                         \
    if (cond) {                                                                \
      printf("%s(%d): %s\n", __FILE__, __LINE__, __func__);                    \
    }                                                                          \
  } while (0)

#define PD_DEBUG_PRINTF(format, ...)                                           \
  do {                                                                         \
    if (0) {                                                                   \
      printf(format, ##__VA_ARGS__);                                           \
    }                                                                          \
  } while (0)

extern char *ip;
extern int *port;
extern char *upstream_url;
extern int *upstream_port;
extern int *thread_num;
extern bool proxy_mode;