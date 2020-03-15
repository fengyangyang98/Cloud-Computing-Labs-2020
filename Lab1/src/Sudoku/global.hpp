#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>


// some basic number
#define BOARD_HEIGHT    9
#define BOARD_WIDTH     9
#define BOARD_SCALE     BOARD_HEIGHT * BOARD_WIDTH

#define PROBLEM_BUFFER_LEN 10
#define FILENAME_BUFFER_LEN 10
#define MAX_THREAD_NUMBER 10


// sem_t for the read & write problem
extern sem_t problemBufferEmpty;
extern sem_t problemBufferFull;
extern sem_t problemBufferMutex;

extern sem_t filenameBufferEmpty;
extern sem_t filenameBufferFull;
extern sem_t filenameBufferMutex;



// some structure
struct board
{
    std::string problem;
    pthread_t * prevThread;
    struct threadCB * tcb;
};

enum threadStatus
{
    idle,
    running
};

struct threadCB
{
    threadStatus status = idle;
    pthread_t t;
};

// some buffer
extern std::queue<std::string> problemBuffer;


#endif