//Cody Herberholz
//cherber2@pdx.edu
//Mini-Chess project

#include "player.h"

//might be able to create a drived class from board that will handle running a 
//random game or player vs player game. that would defeat purpose of player class.
//need to make sure player can make random game possible from outside board class

Player::Player()
{

}

int Player::random_game(Board & game)
{
    //create a function in player to randomly sort the returned move list
    
    int result = game.random_game(-1); //white goes first
    fprintf(stdout, "%d\n", result);
    //Move list[102];
    //game.display();

    //game.read_board();
    //game.display();

    //game.movegen(list);

    //server(list);

    return 0;
}

int Player::player_vs_ai(Board & game)
{

    return 0;
}

int Player::player_vs_player(Board & game)
{

    return 0;
}

int Player::server(Move list[])
{

    return 0;
}
