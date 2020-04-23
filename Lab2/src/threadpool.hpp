#pragma once

#include "global.h"
#include "lock.hpp"
#include <thread>
#include <mutex>
#include <atomic>

class ThreadPool {
  private :

  int thread_number;
  pthread_t* thread;
  queue<int> free_thread;

  public :
  ThreadPool(){}
  ThreadPool(int thread_number) {
    this->thread_number = thread_number;
    thread = new pthread_t[thread_number];
  }
  void set_number(int thread_number) {
    this->thread_number = thread_number;
    if (nullptr != thread) {
      delete[] thread;
    }
    thread = new pthread_t[thread_number];
  }
  ~ThreadPool() {
    delete[] thread;
  }
  void bind_job(void *(*__start_routine)(void *), void *__restrict__ __arg){
    __start_routine(__arg) ;
  }

};