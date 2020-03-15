#ifndef SOLVERMANAGER_HPP
#define SOLVERMANAGER_HPP

#include <pthread.h>

void * solveManager(void * arg);
void resultSemInit();
void * answerPrinter(void * arg);

#endif