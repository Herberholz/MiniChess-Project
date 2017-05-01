//Cody Herberholz
//cherber2@pdx.edu
//Minichess Project

//When dealing with flags from command line ( --random) create separate class 
//(player class) to play random game, have a game where i play against computer, 
//and a game that connects to server for 2 player game

//Possibly might have to initialize Board game in Player functions or just pass 
//it into each function called

#include "player.h"

int main(int argc, char ** argv)
{
    Player adversary;
    Board game;
    
    //Temporarilly set to random setting, must be set to zero for final implementation
    int choice = 1;

    //flag checking
    if(argc == 2)
    {
        if(strcmp(argv[1], "--random") == 0)
            choice = 1;
        else if(strcmp(argv[1], "--1player") == 0)
            choice = 2;

    }

    //default should be player vs player
    switch(choice)
    {
        //player vs player
        case 0:
            fprintf(stdout, "player vs player not implemented\n");
            break;
        //random
        case 1:
            adversary.random_game(game);
            break;
        //1 player
        case 2:
            break;
        //error state
        default:
            fprintf(stderr, "Flag Error");
    }

    //fprintf(stdout, "%s\n", argv[1]);
    //game.read_board();
    //game.display();

    return 0;
}
