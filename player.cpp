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
    Move list[102];
    //game.display();

    game.read_board();
    game.display();

    game.movegen(list);


    //game.move(list[i]);
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
