//Cody Herberholz
//cherber2@pdx.edu
//Minichess project

#include <stdio.h>

const int ROW_DIM = 6;
const int COL_DIM = 6;

class Board
{
    public:
        Board();
        //~Board();
        void display();

    private:
        char board[ROW_DIM][COL_DIM];

};
