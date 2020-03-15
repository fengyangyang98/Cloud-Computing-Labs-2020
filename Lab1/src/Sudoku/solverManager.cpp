#include "solverManager.hpp"
#include "fileManager.hpp"
#include "solver.hpp"
#include "global.hpp"

#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <future>
#include <queue>
#include <semaphore.h>

extern int PROBLEM_BUFFER_LEN;
extern int FILENAME_BUFFER_LEN;
extern int RESULT_BUFFER_LEN;
extern bool OUTPUT;

sem_t resultBufferEmpty;
sem_t resultBufferFull;
sem_t resultBufferMutex;

std::queue<std::future<std::string>> resultBuffer; 


void resultSemInit()
{
    sem_init(&resultBufferEmpty,0, RESULT_BUFFER_LEN);
    sem_init(&resultBufferFull,0,0);
    sem_init(&resultBufferMutex,0,1);
}

void * solveManager(void * arg)
{
    while(1)
    {
        // get the answer from the problem buffer
        std::string problem = problemReader();
        sem_wait(&resultBufferEmpty);
        sem_wait(&resultBufferMutex);

        if(OUTPUT)
            resultBuffer.push(std::async(std::launch::async, solverThread, problem));
        else
            std::async(std::launch::async, solverThread, problem);

        sem_post(&resultBufferMutex);
        sem_post(&resultBufferFull);
    }

}

void * answerPrinter(void * arg)
{
    while(1)
    {
        sem_wait(&resultBufferFull);
        sem_wait(&resultBufferMutex);

        resultBuffer.front().wait();
        printf("%s\n", resultBuffer.front().get().c_str());
        resultBuffer.pop();

        sem_post(&resultBufferMutex);
        sem_post(&resultBufferEmpty);
    }
}
