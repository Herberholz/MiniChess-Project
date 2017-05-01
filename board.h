//Cody Herberholz
//cherber2@pdx.edu
//Minichess project

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

const int ROW_DIM = 6;
const int COL_DIM = 6;

//Whenever white is onmove need to bump the move number

struct Move
{
    int from_row;
    int from_col;
    int to_row;
    int to_col;
};


//Task:
class Board
{
    public:
        Board();
        //~Board();
        void display();         //print out board to stream
        int read_board();       //read in board from stream
        int move(Move loc);     //if piece is legal then makes move and provides new state
        int move(char coord[]); //decodes argument, checkes if move legal, then calls move
        int movegen();          //generates all legal moves that can be made from given state
    
    private:
        char board[ROW_DIM][COL_DIM];
        int onmove;
        int move_num;
};
