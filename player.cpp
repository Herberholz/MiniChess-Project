//Cody Herberholz
//cherber2@pdx.edu
//Mini-Chess project

#include "board.h"

//might be able to create a drived class from board that will handle running a 
//random game or player vs player game. that would defeat purpose of player class.
//need to make sure player can make random game possible from outside board class



//Constructer but nothing valuable to initialize
Player::Player() {}



//Task:   Pits a random player against random player
//Input:  Side on move
//Output: Value of who won
int Player::random_game(int player)
{
    Move list[70];            //list that has size of all possible moves
    int moves = movegen(list,player); //generate moves

    //if no legal move available then you lose
    if(!moves)
        return -1;

    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 41) //1-41 = 40
            return 0;
    }

    int max_val = -1;
    int val = -2;
    int rando = rand() % moves;
    
    //make random move from list
    int result = move(list[rando],player);
    display(-player);
    if(player == -1)
        ++move_num;

    //king caught
    if(result == 1)
        return 1;
    else
        val = -random_game(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}



//Task:   Pits a random player against a human player
//Input:  Side on move
//Ouput:  Value indicating who won
int Player::rand_vs_player(int player)
{
    Move list[70];   //holds list of moves that is the size of all available moves
    int result = 0;  //holds value that move function returns
    int moves = movegen(list,player); //generates moves

    //if no legal move available then you lose
    if(!moves)
        return -1;

    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 41) //1-41 = 40 moves
            return 0;
    }

    int max_val = -1;
    int val = -2;
    int rando = rand() % moves;
   
    //had to place display() in both if and else to avoid move_num misprint
    if(player == -1)
    {
        char coords[6];
        fprintf(stdout, "Please select move(EX: a2-a3): ");
        int check = fscanf(stdin, "%s", coords);
        if(!check)
        {
            fprintf(stdout, "stdin error");
            exit(0);
        }
        result = move(coords,player); //move by passing in coords
        display(-player);
        ++move_num;
    }
    else
    {
        //make random move from list
        result = move(list[rando],player);
        display(-player);
    }
    
    //king caught
    if(result == 1)
        return 1;
    else
        val = -rand_vs_player(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}



//Task:   Pits a negamax player against a human player
//Input:  Player on move
//Output: Value indicating which player won
int Player::nega_vs_player(int player)
{
    Move list[70];    //move list with size that will allow all possible moves
    int result = 0;   //holds result that negamax returns
    int moves = movegen(list,player); //generates moves

    //if no legal move available then you lose
    if(!moves)
        return -1;

    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 41)
            return 0;
    }

    int max_val = -1;
    int val = -2;
    
    if(onmove == -1)
    {
        char coords[6];
        fprintf(stdout, "Please select move(EX: a2-a3): ");
        int check  = fscanf(stdin, "%s", coords);
        if(!check)
        {
            fprintf(stdout, "stdin error");
            exit(0);
        }
        result = move(coords,player); //move by passing in coords
        display(-player);
        ++move_num;
    }
    else
    {
        result = negamax(player, DEPTH, 0);
//        result = ab_prune(player,ABDEPTH, 0, -10000, 10000); 
    }
    
    //king caught
    if(result >= 10000)
        return 1;
    else if(result <= -10000) //opponent won
        return -1;
    else
        val = -nega_vs_player(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}



//Task:   Pits random player against negamax player
//Input:  Player on side
//Output: Value indicating which side won 
int Player::rand_vs_nega(int player)
{
    Move list[70];    //move list of all possible moves
    int result = 0;   //holds return result of negamax
    int rand_win = 0; //holds return result of random move
    int moves = movegen(list,player); //generates all possible moves

    //if no legal move available then you lose
    if(!moves)
        return -1;

    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 41)
            return 0;
    }

    int max_val = -1;
    int val = -2;
    
    if(player == -1)
    {
        move_index = 0;
        result = negamax(player, DEPTH, 0);
        ++move_num;
    }
    else
    {
        int rando = rand() % moves;
        rand_win = move(list[rando],player);
        display(-player);
    }
    
    //king caught
    if(rand_win == 1)
        return -1;
    if(result == 1)
        return 1;
    else if(result == -1)
        return -1;
    else
        val = -rand_vs_nega(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}



//Task:
//Input:
//Output:
int Player::abprune_vs_nega(int player)
{
    int result = 0;   //holds return result of negamax
    
    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 41)
            return 0;
    }

    int max_val = -1;
    int val = -2;
    
    if(player == -1)
    {
        move_index = 0;
        string[0] = '\0';
        ab_prune(player, ABDEPTH, 0, -10000, 10000);
        if(onmove != player)
            onmove = player;
        fprintf(stdout, "Move: %s\n", string);
        ++move_num;
    }
    else
    {
        move_index = 0;
        result = negamax(player, DEPTH, 0);
    }
    
    if(result >= 10000)
        return 1;
    else if(result <= -10000)
        return -1;
    else
        val = -abprune_vs_nega(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}



//Task:   Reads in a board from stdin and then evaluates and makes move
//Input:  Player on move
//Output: N/A
void Player::test()
{    
    int val = 0;
    read_board();
    int player = onmove;
    fprintf(stdout, "Onmove: %d\n", player);
    fprintf(stdout, "Negamax: \n\n" );
    //fprintf(stdout, "depth: %d\n", ndepth);
    val = negamax(player, DEPTH, 0);
    fprintf(stdout, "Onmove: %d\n", player);
    fprintf(stdout, "Move Taken: %s\n", string);
    fprintf(stdout, "%d\n", val);
    fprintf(stdout, "Move_index: %d\n", move_index);

    move_index = 0;
    string[0] = '\0';
    
    fprintf(stdout, "\n\nAlpha Beta: \n\n" );
    val = ab_prune(player, ABDEPTH, 0, -10000, 10000);
    if(player != onmove)
        onmove = player;
    fprintf(stdout, "Onmove: %d\n", onmove);
    fprintf(stdout, "Move Taken: %s\n", string);
    fprintf(stdout, "%d\n", val);
    fprintf(stdout, "Move_index: %d\n", move_index);
}


//Task:   Pits ab against negamax and AB at same time
//Input:  Player on side
//Output: Value indicating which side won 
int Player::test_ab(int player)
{
    int result = 0;   //holds return result of negamax

    //if move num is going past 40 specify a draw
    if(player == -1)
    {
        if(move_num == 41)
            return 0;
    }

    int max_val = -1;
    int val = -2;
    
    if(player == -1)
    {
        fprintf(stdout, "-----------------------\n");
        move_index = 0;
        result = negamax(player, DEPTH, 0);
        
        move_index = 0;
        result = ab_prune(player, ABDEPTH, 0, -10000, 10000);
        fprintf(stdout, "------------------------\n");
        //fprintf(stdout, "Nega_val: %d\n", result);
        ++move_num;
    }
    else
    {
        move_index = 0;
        result = ab_prune(player, ABDEPTH, 0, -10000, 10000);
    }
    
    //king caught
    if(result == 1)
        return 1;
    else if(result == -1)
        return -1;
    else
        val = -test_ab(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}


//Task:   Reads in a board from stdin and then evaluates and makes move
//Input:  Player on move
//Output: N/A
void Player::test_id()
{    
    int val = 0;
    read_board();
    int player = onmove;

    fprintf(stdout, "Onmove: %d\n", player);
    fprintf(stdout, "AB: \n\n" );
    val = ab_prune(player, ABDEPTH, 0, -10000, 10000);

    if(player != onmove)
        onmove = player;
    
    fprintf(stdout, "Onmove: %d\n", player);
    fprintf(stdout, "Move Taken: %s\n", string);
    fprintf(stdout, "%d\n", val);
    fprintf(stdout, "Move_index: %d\n", move_index);

    move_index = 0;
    string[0] = '\0';
    
    fprintf(stdout, "\n\nID AB: \n\n" );
    val = iterative_deep(player);
    fprintf(stdout, "Onmove: %d\n", onmove);
    fprintf(stdout, "Move Taken: %s\n", string);
    fprintf(stdout, "%d\n", val);
    fprintf(stdout, "Move_index: %d\n", move_index);
}



//Task:   Connects to imcs server to play a game against a bot/player
//Input:  Command line values (EX: ./a.out A W13842 Username password)
//Output: N/A
int Player::imcs_play(int argc, char ** argv)
{
    //O W DeepBlue Password
    //A B17534 DeepBlue Password

    setlinebuf(stdout);
    if(argc < 5 || argc > 7)
    {
        fprintf(stderr, "arg count not correct\n");
        exit(0);
    }

    char mecolor = '?';
    int megame = 0;
    
    switch(argv[1][0])
    {
        case 'O':
            switch(argv[2][0])
            {
                case 'W':
                case 'B':
                case '?':
                    mecolor = argv[2][0];

                    //set onmove to correct player
                    if(mecolor == 'W')
                        onmove = -1;
                    else if(mecolor == 'B')
                        onmove = 1;
                    else
                    {
                        fprintf(stderr, "onmove not set in server code");
                        exit(0);
                    }
                    break;
                default:
                    fprintf(stderr, "Error with Offer command\n");
                    exit(0);
            }
            break;
        case 'A':
        {
            char ch = argv[2][0];
            if(isdigit(ch))
                megame = atoi(&argv[2][1]);
            if(ch == 'W' || ch == 'B')
            {
                mecolor = ch;
                megame = atoi(&argv[2][1]);
     
                //set onmove to correct player
                if(mecolor == 'W')
                    onmove = -1;
                else if(mecolor == 'B')
                    onmove = 1;
                else
                {
                    fprintf(stderr, "onmove not set in server code");
                    exit(0);
                }
            }
            else
            {
                fprintf(stderr, "Error with Accept command\n");
                exit(0);
            }
            if(megame <= 0)
            {
                fprintf(stderr, "Connection Error\n");
                exit(0);
            }
            break;
        }
        default:
            fprintf(stderr, "argv error in server connection\n");
            exit(0);
    }
    
    char * meuser = argv[3];
    char * mepassword = argv[4];
    char * host = (char *)"imcs.svcs.cs.pdx.edu";
    if(argc > 5)
        host = argv[5];
    int port = 3589;
    if(argc > 6)
    {
        port = atoi(argv[6]);
        if(port <= 0)
        {
            fprintf(stderr, "Invalid Port");
            exit(0);
        }
    }
    FILE * nf = net.netopen(host, port);
    setlinebuf(nf);
    net.startlog();
    char * greeting = net.expectcmd(nf, 1, 100, 0);
    (void) strtok(greeting, " ");
    char * pgm = strtok(0, " ");
    assert(!strcmp(pgm, "imcs"));
    char * version = strtok(0, " \r\n");
    if(strcmp(version, "2.5"))
    {
        fprintf(stderr, "got unexpected imcs version %s\n", version);
        exit(0);
    }

    net.sendcmd(nf,(char*)"me %s %s", meuser, mepassword);
    
    (void) net.expectcmd(nf, 1, 201, 0);
    if(megame != 0)
    {
        if(mecolor == '?')
            net.sendcmd(nf, (char*)"accept %d", megame);
        else
            net.sendcmd(nf, (char*)"accept %d %c", megame, mecolor);
        (void) net.expectcmd(nf, 1, 105, 106, 0);
    }
    else
    {
        if(mecolor == '?')
            net.sendcmd(nf, (char*)"offer");
        else
            net.sendcmd(nf, (char*)"offer %c", mecolor);
        (void) net.expectcmd(nf, 1, 103, 0);
        net.logmsg((char*)"waiting for opponent");
        (void) net.expectcmd(nf, 1, 105, 106, 0);
        net.logmsg((char*)"opponent found");
    }

    //------------------------------------------------------------
    //can start implementing code to manipulate board below

    while(1)
    {
        int ch = fgetc(nf);
        int r = ungetc(ch, nf);
        assert(r != EOF);
        if(move_num > 41)
        {
            exit(0);
        }

        switch(ch)
        {
            case '?':
            {
                char * r = net.getnet(nf, (char*)"?");
                char * q = strtok(r, " ");
                int player = onmove;
                assert(!strcmp(q, "?"));

                //insert time management here
                move_index = 0; //reset move index
                string[0] = '\0';

                iterative_deep(player);

                //XXX fix onmove setting within movegen within negamax/ab
                if(player != onmove)
                    onmove = -onmove; //sets onmove back to correct number
                
                ++move_num;
                net.sendcmd(nf, string);
                continue;
            }
            case '!':
            {
                assert(fgetc(nf) == '!');
                int ch;
                char temp[6];
                int player = onmove;
                temp[0] = '\0';

                do
                    ch = fgetc(nf);
                while(isspace(ch));
                
                ungetc(ch, nf);
                int issue = fscanf(nf, "%s", temp);
                if(!issue)
                {
                    fprintf(stdout, "stdin error");
                    exit(0);
                }
                //net.logmsg((char*)"received move %s\n\n",temp);
                
                move(temp, -player); //make move
                continue;
            }
            case '=':
                (void) net.getnet(nf, (char*)"=");
                break;
            case 'X':
                (void) net.getnet(nf, (char*)"X");
                break;
            default:
                (void) net.getnet(nf, (char*)"...");
                continue;
        }
        break;
    }
    fclose(nf);
    
    return 0;
}
