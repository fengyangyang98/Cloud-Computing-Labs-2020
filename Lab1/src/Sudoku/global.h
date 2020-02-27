#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string>
#include <stack>
#include <semaphore.h>
#include <thread>

using std::string;
using std::stack;
using std::thread;


// the length of the board
#define BOARD_LEN 9
#define BOARD_SCALE 81
#define SUB_BOARD_LEN 3


#define PROBLEM_BUFFER_LEN 3
#define RESULT_BUFFER_LEN 3



// store the file content in buffer for the solver to read them one by one
extern stack<string> problemBuffer;

// store the solution in order
extern stack<string> resultBuffer;

// semi
extern sem_t empty;
extern sem_t full;
extern sem_t mutex;



#endif