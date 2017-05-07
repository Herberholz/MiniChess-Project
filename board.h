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
    Move();
    int from_row;
    int from_col;
    int to_row;
    int to_col;
    //char move_coord[6];
    char f_col_coord;
    int f_row_coord;
    char t_col_coord;
    int t_row_coord;
};


//Task:
class Board
{
    public:
        Board();
        //~Board();
        void display();         //print out board to stream
        void display_moves(Move[],int);   //prints out all moves available for state
        int read_board();       //read in board from stream
        int move(Move loc);     //if piece is legal then makes move and provides new state
        //int move(char coord[]); //decodes argument, checkes if move legal, then calls move
        int fill_move(int,int,int,int,Move[],int&);           //fills move list
        int scan(int,int,int,int,int,int,Move[],int&);             //scans to find pieces
        int symm_scan(int,int,int,int,int,int,Move[],int&);//tries all for rotational symmetries of a given scan
        int move_list(int,int,Move[],int&);        //scans to find moves for each piece
        int movegen(Move list[]);          //generates all legal moves that can be made from given state
    
    private:
        char board[ROW_DIM][COL_DIM];
        int onmove;
        int move_num;
};
