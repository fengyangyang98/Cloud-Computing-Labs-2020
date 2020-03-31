#ifndef SOLVERMANAGER_HPP
#define SOLVERMANAGER_HPP

#include "global.hpp"
#include "lock.hpp"
#include "solver.hpp"

// extern problemID;
struct solverID
{
    unsigned int id;
};


void * solveManager(void * arg);
void * answerPrinter(void * arg);

#endif