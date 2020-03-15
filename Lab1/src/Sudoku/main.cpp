#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "solverManager.hpp"
#include "fileManager.hpp"
#include "global.hpp"

extern pthread_t * prevThread;

int main()
{
    pthread_t filenameReader;
    pthread_t problemReader;
    pthread_t problemSolver;
    pthread_t resultPrinter;

    problemSemInit();
    resultSemInit();

    pthread_create(&filenameReader, NULL, filenameReadThread, NULL);
    pthread_create(&problemReader, NULL, problemReadThread, NULL);
    pthread_create(&problemSolver, NULL, solveManager, NULL);
    pthread_create(&resultPrinter, NULL, answerPrinter, NULL);

    pthread_join(filenameReader, NULL);
    pthread_join(problemReader, NULL);
    pthread_join(problemSolver, NULL);
}