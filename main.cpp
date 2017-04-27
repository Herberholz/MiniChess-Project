//Cody Herberholz
//cherber2@pdx.edu
//Minichess Project

//When dealing with flags from command line ( --random) create separate class 
//(player class) to play random game, have a game where i play against computer, 
//and a game that connects to server for 2 player game

#include "board.h"



int main(int argc, char ** argv)
{
    Board game;

    game.display();

    return 0;
}
