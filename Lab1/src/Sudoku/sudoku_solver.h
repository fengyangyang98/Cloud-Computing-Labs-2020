#ifndef _SUDOKU_SOLVER_H_
#define _SUDOKU_SOLVER_H_

#include "global.h"

#define BoardStringIndex(row, col) row * BOARD_LEN + col


string resultBuffer[RESULT_BUFFER_LEN];

// the program to solve one part of the problem
bool SudokuChecker(string board, int row, int col, int temp_number);


// the program to solve the single problem
void SudokuSolver();


void SudokuResult();

#endif