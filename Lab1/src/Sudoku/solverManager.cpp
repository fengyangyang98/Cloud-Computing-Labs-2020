#include "solverManager.hpp"
#include "fileManager.hpp"
#include "solver.hpp"
#include "global.hpp"

#include <string.h>
#include <pthread.h>


threadCB tcb[MAX_THREAD_NUMBER];
board bcb[MAX_THREAD_NUMBER];
pthread_t * prevThread;

int initCB()
{
    int rc = 0;
    for(int i = 0; i < MAX_THREAD_NUMBER; i++)
    {
        bcb[i].tcb = & tcb[i];
    }
    return rc;
}

void * solveManager(void * arg)
{
    while(1)
    {
        // get the answer from the problem buffer
        std::string problem = problemReader();

        // get a new thread to solve the problem
        int threadCBIndex = 0;
        while(1)
        {
            if(tcb[threadCBIndex].status == idle) {
                bcb[threadCBIndex].prevThread = prevThread;
                bcb[threadCBIndex].problem = problem;
                bcb[threadCBIndex].prevThread = &(tcb[threadCBIndex].t);
                tcb[threadCBIndex].status = running;
                
                pthread_create(&tcb[threadCBIndex].t, NULL, solverThread, bcb);
                break;
            }

            threadCBIndex = (threadCBIndex + 1) % MAX_THREAD_NUMBER;
        }
    }

}