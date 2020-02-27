#include "main.h"
#include "thread_solver.h"



int main(){
    // build a process/thread to manage the input
    sem_init(&empty, 0, PROBLEM_BUFFER_LEN);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    ProblemWriter("310000085000903000905000307804000106000401000690000073030502010000804000020706090");
    ProblemWriter("693784512487512936125963874932651487568247391741398625319475268856129743274836159");
    ProblemWriter("869725413512934687374168529798246135231857946456319872683571294925483761147692358");

    // build a process/thread to manage the problem solving
    while(1) {
        SolveThreadControler();
    }
}