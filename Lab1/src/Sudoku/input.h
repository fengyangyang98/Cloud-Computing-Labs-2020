#ifndef _INPUT_H_
#define _INPUT_H_


#include "global.h"



// a dedicated thread hold the program for reading content from files
void ReadController();

// each file read by a thread 
void ReadThread(FILE filename);

#endif