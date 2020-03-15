#include <semaphore.h>
#include <string>
#include <queue>
#include <unistd.h>

#include "global.hpp"

sem_t problemBufferEmpty;
sem_t problemBufferFull;
sem_t problemBufferMutex;

sem_t filenameBufferEmpty;
sem_t filenameBufferFull;
sem_t filenameBufferMutex;

std::queue<std::string> problemBuffer;
std::queue<std::string> filenameBuffer;

int problemSemInit()
{
    int rc = 0;
    sem_init(&problemBufferEmpty,0,PROBLEM_BUFFER_LEN);
    sem_init(&problemBufferFull,0,0);
    sem_init(&problemBufferMutex,0,1);

    sem_init(&filenameBufferEmpty,0,FILENAME_BUFFER_LEN);
    sem_init(&filenameBufferFull,0,0);
    sem_init(&filenameBufferMutex,0,1);

    return rc;
}

std::string problemReader() {
    sem_wait(&problemBufferFull);
    sem_wait(&problemBufferMutex);

    std::string tem = problemBuffer.front();
    problemBuffer.pop();

    // printf("Read: %s\n", tem.c_str());
    // printf("%d\n", problemBuffer.size());

    sem_post(&problemBufferMutex);
    sem_post(&problemBufferEmpty);

    return tem;
}

void problemWriter(std::string problem) {
    sem_wait(&problemBufferEmpty);
    sem_wait(&problemBufferMutex);

    // std::cout<<"Write:"<<problem<<std::endl;
    problemBuffer.push(problem);

    sem_post(&problemBufferMutex);
    sem_post(&problemBufferFull);
}


void * problemReadThread(void * arg)
{
    while(1)
    {
        sem_wait(&filenameBufferFull);
        sem_wait(&filenameBufferMutex);

        std::string file_path = filenameBuffer.front();
        filenameBuffer.pop();

        sem_post(&filenameBufferMutex);
        sem_post(&filenameBufferEmpty);

        std::ifstream file(file_path,std::ios::in);

        if (!file.good()) {
            std::cout <<"failed to open file"<< "\n";
            continue;
        }

        else if(file.peek() == EOF)
        {
            std::cout << "file is empty."<<std::endl;
            continue;
        }

        std::string problem;
        int pos=0;

        while(file.peek() != EOF){
            file.seekg(pos, std::ios::beg);
            std::getline(file, problem);
            problemWriter(problem);    
            pos += BOARD_SCALE + 1;
        }
    }
}

void * filenameReadThread(void * arg){
    std::string file_path;
	// std::cout<<"filename tid:"<<pthread_self()<<std::endl;

    while(std::cin >> file_path)
    {
        sem_wait(&filenameBufferEmpty);
        sem_wait(&filenameBufferMutex);

        filenameBuffer.push(file_path);

        sem_post(&filenameBufferMutex);
        sem_post(&filenameBufferFull);
    }
}

