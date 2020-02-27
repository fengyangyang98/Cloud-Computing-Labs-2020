#ifndef _SUDOKU_SOLVER_H_
#define _SUDOKU_SOLVER_H_

#include "global.h"



#define BoardStringIndex(row, col) row * BOARD_LEN + col


// the program to solve the single problem
void SudokuSolverThread(string board);

#endif