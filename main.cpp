//Cody Herberholz
//cherber2@pdx.edu
//Minichess Project

//When dealing with flags from command line ( --random) create separate class 
//(player class) to play random game, have a game where i play against computer, 
//and a game that connects to server for 2 player game

#include "board.h"

int main(int argc, char ** argv)
{
    Player adversary;
    int choice = 0;
    int result = 0;
    srand(time(NULL));

    //flag checking
    if(argc == 2)
    {
        if(strcmp(argv[1], "--random") == 0)
            choice = 1;
        else if(strcmp(argv[1], "--1vrand") == 0)
            choice = 2;
        else if(strcmp(argv[1], "--1vnega") == 0)
            choice = 3;
        else if(strcmp(argv[1], "--randvnega") == 0)
            choice = 4;
        else if (strcmp(argv[1], "--abvnega") == 0)
            choice = 5;
        else if (strcmp(argv[1], "--test") == 0)
            choice = 6;
        else if (strcmp(argv[1], "--testab") == 0)
            choice = 7;
        else if (strcmp(argv[1], "--id") == 0)
            choice = 8;
    }

    //default should be player vs player
    switch(choice)
    {
        //player vs player
        case 0:
            adversary.imcs_play(argc, argv);
            break;
        //random
        case 1:
            adversary.display(-1);
            result = adversary.random_game(-1); //pass in -1 since white goes first
            fprintf(stdout, "%d\n", result);
            break;
        //1 player
        case 2:
            adversary.display(-1);
            result = adversary.rand_vs_player(-1);
            fprintf(stdout, "%d\n", result);
            break;
        case 3:
            adversary.display(-1);
            result = adversary.nega_vs_player(-1);
            fprintf(stdout, "%d\n", result);
            break;
        case 4:
            adversary.display(-1);
            result = adversary.rand_vs_nega(-1);
            fprintf(stdout, "%d\n", result);
            break;
        case 5:
            adversary.display(-1);
            result = adversary.abprune_vs_nega(-1);
            fprintf(stdout, "%d\n", result);
            break;
        case 6:
            adversary.test();
            break;
        case 7:
            adversary.test_ab(-1);
            break;
        case 8:
            //adversary.iterative_deep(-1);
            adversary.test_id();
            break;
        //error state
        default:
            fprintf(stderr, "Flag Error");
    }

    return 0;
}
