//Cody Herberholz
//cherber2@pdx.edu
//Minichess project

#include "board.h"


//Constructor
Board::Board()
{
    //XXX temporarily hard code board and then copy to actual board
    char temp[ROW_DIM][COL_DIM] = {
        {'k','q','b','n','r','\0'},
        {'p','p','p','p','p','\0'},
        {'.','.','.','.','.','\0'},
        {'.','.','.','.','.','\0'},
        {'P','P','P','P','P','\0'},
        {'R','N','B','Q','K','\0'}
    };
    
    for(int i = 0; i < ROW_DIM; ++i)
        for(int j = 0; j < COL_DIM; ++j)
            board[i][j] = temp[i][j];
}

void Board::display()
{
    for(int i = 0; i < ROW_DIM; ++i)
        fprintf(stdout, "%s\n", board[i]);
}
