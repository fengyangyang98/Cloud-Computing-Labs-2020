#include "thread_solver.h"

void SolveThreadControler() {
    string problem = ProblemReader();
    std::thread t(SudokuSolverThread, problem);
    t.join();
}

