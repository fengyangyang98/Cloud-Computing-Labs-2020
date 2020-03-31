#ifndef GLOBAL_HPP
#define GLOBAL_HPP

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
#include <sys/sysinfo.h>
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
#include <queue>
#include <string>
#include <atomic>
#include <iostream>
#include <assert.h>
#include <algorithm>

// some basic number
#define BOARD_HEIGHT    9
#define BOARD_WIDTH     9
#define BOARD_SCALE     BOARD_HEIGHT * BOARD_WIDTH

#define SOLVER_NUMBER_MAX 1000
extern int BATCH_NUMBER;

// some buffer
// extern std::queue<std::string> problemBuffer;
struct problemID
{
    std::string problem;
    size_t id;
    problemID(size_t n, std::string p) 
    {
        problem = p;
        id = n; 
    }
    problemID(){}
};

#endif