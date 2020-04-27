#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include "global.h"
#include "transport.hpp"
#include "lock.hpp"

void error_respose(string &dest, string &src);

 /**
   * @Author : Feng Yangyang
   */

void * socket_worker(void * arg);

void * thread_scheduling(void * arg);

#endif