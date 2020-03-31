#include "global.hpp"
#include "solverManager.hpp"
#include "fileManager.hpp"

int SOLVER_NUMBER = get_nprocs_conf(); 
int BATCH_NUMBER = 1000;

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 't':
                    SOLVER_NUMBER = std::min(SOLVER_NUMBER, atoi(&argv[i][2]));
                    break;
                case 'b':
                    BATCH_NUMBER = atoi(&argv[i][2]);
                    break;
                default:
                break;
            }
        }
    }
        

    pthread_t filenameReader;
    pthread_t problemReader;
    pthread_t printer;
    pthread_t solverThread[SOLVER_NUMBER];
    solverID  id[SOLVER_NUMBER];
    for(int i = 0; i < SOLVER_NUMBER; i++)
        id[i].id = i;
    
    pthread_create(&filenameReader, NULL, filenameReadThread, NULL);
    pthread_create(&problemReader, NULL, problemReadThread, NULL);
    pthread_create(&printer, NULL, answerPrinter, NULL);
    for(int i = 0; i < SOLVER_NUMBER; i++)
    {
        pthread_create(&solverThread[i], NULL, solveManager, &id[i]);
    }



    pthread_join(filenameReader, NULL);
    pthread_join(problemReader, NULL);
    pthread_join(printer, NULL);
    for(int i = 0; i < SOLVER_NUMBER; i++)
    {
        pthread_join(solverThread[i], NULL);
    }

}