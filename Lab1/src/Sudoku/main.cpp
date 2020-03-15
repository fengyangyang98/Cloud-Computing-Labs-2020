#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "solverManager.hpp"
#include "fileManager.hpp"
#include "global.hpp"

int PROBLEM_BUFFER_LEN=10;
int FILENAME_BUFFER_LEN=10;
int RESULT_BUFFER_LEN=10;
bool OUTPUT = false;

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'p':
                sscanf(&argv[i][2], "%d", &PROBLEM_BUFFER_LEN);
                break;
            case 'f':
                sscanf(&argv[i][2], "%d", &FILENAME_BUFFER_LEN);
                break;
            case 'r':
                sscanf(&argv[i][2], "%d", &RESULT_BUFFER_LEN);
                break;
            case 'i':
                OUTPUT = true;
                break;
            default:
                break;
            }
        }
    }

    printf("PROBLEM_BUFFER_LEN = %d\nFILENAME_BUFFER_LEN = %d\nRESULT_BUFFER_LEN = %d\n", 
                                PROBLEM_BUFFER_LEN, FILENAME_BUFFER_LEN, RESULT_BUFFER_LEN);
    printf("OUTPUT = %s\n", (OUTPUT ? "TURE" : "FALSE"));

    pthread_t filenameReader;
    pthread_t problemReader;
    pthread_t problemSolver;
    pthread_t resultPrinter;

    problemSemInit();
    resultSemInit();

    pthread_create(&filenameReader, NULL, filenameReadThread, NULL);
    pthread_create(&problemReader, NULL, problemReadThread, NULL);
    pthread_create(&problemSolver, NULL, solveManager, NULL);

    if(OUTPUT)
        pthread_create(&resultPrinter, NULL, answerPrinter, NULL);

    pthread_join(filenameReader, NULL);
    pthread_join(problemReader, NULL);
    pthread_join(problemSolver, NULL);
    if(OUTPUT)
        pthread_join(resultPrinter, NULL);
}