#include "io.h"

sem_t empty;
sem_t full;
sem_t mutex;

stack<string> problemBuffer;
stack<string> resultBuffer;

string ProblemReader() {
    sem_wait(&full);
    sem_wait(&mutex);

    string tem = problemBuffer.top();
    problemBuffer.pop();

    sem_post(&mutex);
    sem_post(&empty);

    return tem;
}

void ProblemWriter(string problem) {
    sem_wait(&empty);
    sem_wait(&mutex);

    problemBuffer.push(problem);

    sem_post(&mutex);
    sem_post(&full);
}