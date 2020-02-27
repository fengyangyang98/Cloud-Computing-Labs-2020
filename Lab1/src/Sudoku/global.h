#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string>
using std::string;

// the length of the board
#define BOARD_LEN 9
#define BOARD_SCALE 81
#define SUB_BOARD_LEN 3


#define PROBLEM_BUFFER_LEN 100
#define RESULT_BUFFER_LEN 100

extern string resultBuffer[RESULT_BUFFER_LEN];

// store the file content in buffer for the solver to read them one by one
extern string problemBuffer[PROBLEM_BUFFER_LEN];


#endif