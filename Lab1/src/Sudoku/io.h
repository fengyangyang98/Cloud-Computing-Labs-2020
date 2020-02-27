#ifndef _IO_H_
#define _IO_H_


#include "global.h"

// read the problem from the buffer
string ProblemReader();

// set the problem in the stack buffer
void ProblemWriter(string problem);

// a dedicated thread hold the program for reading content from files
void ReadController();

// each file read by a thread 
void ReadThread(FILE filename);

#endif