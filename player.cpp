//Cody Herberholz
//cherber2@pdx.edu
//Mini-Chess project

#include "board.h"

//might be able to create a drived class from board that will handle running a 
//random game or player vs player game. that would defeat purpose of player class.
//need to make sure player can make random game possible from outside board class

Player::Player()
{

}

int Player::random_game(int player)
{
    //move function will return if king is taken
    Move list[102];
    onmove = player;
    int moves = movegen(list);

    //if no legal move available then you lose
    if(!moves)
        return -1;

    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 40)
            return 0;
        ++move_num;
    }

    int max_val = -1;
    int val = -2;
    int rando = rand() % moves;
    
    //make random move from list
    int result = move(list[rando]);
    display();
    
    //king caught
    if(result == 1)
        return 1;
    else
        val = -random_game(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}

int Player::player_vs_ai(int player)
{
    //move function will return if king is taken
    Move list[102];
    onmove = player;
    int result = 0;
    int moves = movegen(list);

    //if no legal move available then you lose
    if(!moves)
        return -1;

    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 40)
            return 0;
        ++move_num;
    }

    int max_val = -1;
    int val = -2;
    int rando = rand() % moves;
    
    if(onmove == -1)
    {
        char coords[6];
        fprintf(stdout, "Please select move(EX: a2-a3): ");
        fscanf(stdin, "%s", coords);
        result = move(coords); //move by passing in coords
    }
    else
    {
        //make random move from list
        result = move(list[rando]);
    }
    display();
    
    //king caught
    if(result == 1)
        return 1;
    else
        val = -player_vs_ai(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;

    return 0;
}

int Player::player_vs_player()
{

    return 0;
}

int Player::server(Move list[])
{

    return 0;
}
