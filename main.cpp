//Cody Herberholz
//cherber2@pdx.edu
//Minichess Project

//When dealing with flags from command line ( --random) create separate class 
//(player class) to play random game, have a game where i play against computer, 
//and a game that connects to server for 2 player game

//Possibly might have to initialize Board game in Player functions or just pass 
//it into each function called

#include "board.h"

int main(int argc, char ** argv)
{
    Player adversary;
    //Board game;
    
    //Temporarilly set to random setting, must be set to zero for final implementation
    int choice = 2;
    int result = 0;
    srand(time(NULL));

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
            result = adversary.random_game(-1); //pass in -1 since white goes first
            fprintf(stdout, "%d\n", result);
            break;
        //1 player
        case 2:
            adversary.display();
            result = adversary.player_vs_ai(-1);
            fprintf(stdout, "%d\n", result);
            break;
        //error state
        default:
            fprintf(stderr, "Flag Error");
    }

    return 0;
}
