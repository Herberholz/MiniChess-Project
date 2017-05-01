//Cody Herberholz
//cherber2@pdx.edu
//Minichess project

#include "board.h"


//Constructor
Board::Board()
{
    onmove = -1;  //indicates that white is on move
    move_num = 1; //holds how many moves have gone by

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

//Task:
void Board::display()
{
    char color;

    if(onmove == 1)
        color = 'B';
    else if(onmove == -1)
        color = 'W';
    else
    {
        fprintf(stderr, "onmove is not valid\n");
        exit(0);
    }

    fprintf(stdout, "%d %c\n", move_num, color);
    for(int i = 0; i < ROW_DIM; ++i)
        fprintf(stdout, "%s\n", board[i]);
}

//Task:
int Board::read_board()
{
    char color;
    int move_number = 0;

    //Take heed when reading into move_num
    //if my move_num is different from opponents at start will it be off??
    fscanf(stdin, "%d", &move_number);
    fscanf(stdin, "%s", &color);

    if(color == 'B')
        onmove = 1;
    else if(color == 'W')
        onmove = -1;
    else
    {
        fprintf(stderr, "Color not recognized\n");
        exit(0);
    }

    for(int i = 0; i < ROW_DIM; ++i)
        fscanf(stdin, "%s", board[i]);

    return 0;
}
