//Cody Herberholz
//cherber2@pdx.edu
//Minichess project

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>
#include "netops.h"


const int ROW_DIM = 6; //Max row dimensions for board
const int COL_DIM = 6; //Max col dimensions for board
const int DEPTH = 4;   //Depth of how far Negamax/AB will search


//Struct holds move info for pieces on board
struct Move
{
    Move();
    int from_row;      //origin row location
    int from_col;      //origin col location
    int to_row;        //row location piece is moving to
    int to_col;        //col location piece is moving to
    char f_col_coord;  //char representation of origin col
    int f_row_coord;   //int representation of origin row
    char t_col_coord;  //char representation of to col
    int t_row_coord;   //int representation of to row
    int promotion;     //holds ascii value of piece before promotion
    int piece_capture; //holds piece capture
    int score;         //holds score of state if move is made
};



//This Board class will provide functions that specifically deal with the state of the board
class Board
{
    public:
        Board();
        void display(int);                 //print out board to stream
        void display_moves(Move[],int);    //prints out all moves available for state
        int read_board();                  //read in board from stream
        int move(Move & loc);              //if piece is legal then makes move and provides new state
        int move(char coord[]);            //decodes argument, checkes if move legal, then calls move
        int undo_move(Move loc);           //undos a move that was made
        int move_order(Move[], Move[], int);  //orders move based on heuristic score
        int scan(int,int,int,int,int,int,Move[],int&);  //scans to find pieces
        int symm_scan(int,int,int,int,int,int,Move[],int&);//tries all for rotational symmetries of a given scan
        int move_list(int,int,Move[],int&);//scans to find moves for each piece
        int movegen(Move list[]);          //generates all legal moves that can be made from given state
        int state_eval(Move &loc);         //values state of board for player
        int negamax(int,int,int);          //searches tree representation of state space for best move
        int ab_prune(int,int,int,int,int); //uses alpha beta pruning to shrink state space search
    
    protected:
        char board[ROW_DIM][COL_DIM]; //holds minichess board 
        int onmove;                   //signifies which player is on move
        int move_num;                 //signifies what the current move is 
        int move_index;               //holds index of which move to make based off negamax/ab prune
        char string[5];               //holds the string representation of move that is passed to server
};



//The Player class is derived from Board and contains all the different ways a player can 
//play minichess. These functions are activated from flags like --random that will be 
//processed from within the main class
class Player: public Board
{
    public:
        Player();
        int random_game(int);       //random  vs random    --random
        int rand_vs_player(int);    //player  vs random    --1vrand
        int nega_vs_player(int);    //player  vs negamax   --1vnega
        int rand_vs_nega(int);      //random  vs negamax   --randvnega
        int abprune_vs_nega(int);   //abprune vs negamax   --abvnega
        void test(int);             //reads in a board state and displays move   --test
        int imcs_play(int, char**); //connects to imcs server to play a game

    protected:
        Netops net; //contains necessary components to connect to server
};
