#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include "global.h"
#include "transport.hpp"
// #include "httpParse.hpp"
#include "lock.hpp"

void *listen(void* arg) ;

void* get_socket_func(void* arg);

void thread_manager();

 /**
   * @Author : Feng Yangyang
   */

void * socket_worker(void * arg);

void * thread_scheduling(void * arg);

#endif