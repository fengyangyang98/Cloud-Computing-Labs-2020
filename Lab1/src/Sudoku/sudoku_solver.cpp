#include "sudoku_solver.h"


bool SudokuChecker(string board, int row, int col, char temp_number) {
    for(int i = 0; i < BOARD_LEN; i++) {
        if(board[BoardStringIndex(row, i)] == temp_number) return false;
        if(board[BoardStringIndex(i, col)] == temp_number) return false;
    }

    int start_row = (row / SUB_BOARD_LEN) * SUB_BOARD_LEN;
    int start_col = (col / SUB_BOARD_LEN) * SUB_BOARD_LEN;

    for(int m = start_row; m <= start_row + 2; m++){
        for(int k = start_col; k <= start_col + 2; k++){
            if(board[BoardStringIndex(m, k)] == temp_number) return false;
        }
    }

    return true;
}

void SudokuSolver(string& board, int index) {
    if(index ==  BOARD_SCALE){
        // get the sulution
        printf("One Solution: %s\n", board.c_str());
    }
    else{
        int row = index / BOARD_LEN;
        int col = index % BOARD_LEN;

        if(board[index] != '0') SudokuSolver(board, index + 1);
        else {
            for(char i = '1'; i <= '9'; i++){
                if(SudokuChecker(board, row, col, i)){
                    board[BoardStringIndex(row, col)] = i;
                    SudokuSolver(board, index + 1);
                    board[BoardStringIndex(row, col)] = '0';
                }
            }
        }
    }
}

void SudokuSolverThread(string board) {
    SudokuSolver(board, 0);
}

