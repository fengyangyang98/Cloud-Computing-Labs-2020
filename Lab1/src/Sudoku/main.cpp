#include "main.h"
#include "sudoku_solver.h"

string problems[10];


int main(){
    // build a process/thread to manage the input
    problems[0] = "310000085000903000905000307804000106000401000690000073030502010000804000020706090";
    problems[1] = "693784512487512936125963874932651487568247391741398625319475268856129743274836159";
    problems[2] = "869725413512934687374168529798246135231857946456319872683571294925483761147692358";


    SudokuSolver(problems[0], 0);
    SudokuSolver(problems[1], 0);
    SudokuSolver(problems[2], 0);


    // build a process/thread to manage the problem solving
}