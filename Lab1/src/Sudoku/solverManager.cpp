#include "global.hpp"
#include "solverManager.hpp"

extern int SOLVER_NUMBER;

cpLock problemBufferLock[SOLVER_NUMBER_MAX];
cpLock resultBufferLock[SOLVER_NUMBER_MAX];
std::queue<char *> problemBuffer[SOLVER_NUMBER_MAX];
std::queue<char *> resultBuffer[SOLVER_NUMBER_MAX];

size_t resultNumber = 0;

void * solveManager(void * arg)
{
    solverID * id = (solverID *) arg;
    unsigned int idno = id->id;
    char * problem;

    while(1)
    {
        problemBufferLock[idno].cGet();
        problem = problemBuffer[idno].front();
        problemBuffer[idno].pop();
        
        problemBufferLock[idno].cRelease();
        
        if(problem) {
            solver(problem);
            resultBufferLock[idno].pGet();
            resultBuffer[idno].push(problem);
            resultBufferLock[idno].pRelease();
        } else {
            resultBufferLock[idno].pGet();
            resultBuffer[idno].push(problem);
            resultBufferLock[idno].pRelease();
            break;
        }
    }
}

void * answerPrinter(void * arg)
{
    size_t bufSize = BATCH_NUMBER * 82;
    char * resultBuf = (char *) malloc(bufSize);
    size_t pos = 0;
    int endNumber = 0;
    char * result;
    while(1) 
    {
        resultNumber++;
        unsigned int id = resultNumber % SOLVER_NUMBER;
        resultBufferLock[id].cGet();
        result = resultBuffer[id].front();
        resultBuffer[id].pop();
        resultBufferLock[id].cRelease();

        if(result) {
            strncpy(&resultBuf[pos], result, 81);
            free(result);
            resultBuf[pos + 81] = '\n';
            pos += 82;
            if(pos >= bufSize) {
                resultBuf[bufSize - 1] = '\0';
                printf("%s\n", resultBuf);
                pos = 0;
            }
        }
        else endNumber++;

        if(endNumber == SOLVER_NUMBER) {
            if(pos != 0) {
                resultBuf[pos - 1] = '\0';
                printf("%s\n", resultBuf);
            }
            break;
        }
    }
    free(resultBuf);   
}

