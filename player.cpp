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
}

int Player::player_vs_player(int player)
{
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
    
    if(onmove == -1)
    {
        char coords[6];
        fprintf(stdout, "Please select move(EX: a2-a3): ");
        fscanf(stdin, "%s", coords);
        result = move(coords); //move by passing in coords
        display();
    }
    else
    {
        //make random move from list
        int depth = DEPTH;
        result = negamax(onmove, depth, 0);
        fprintf(stdout, "%d\n", result);
    }
    
    //king caught
    if(result >= 10000)
        return 1;
    else if(result <= -10000)
        return -1;
    else
        val = -player_vs_player(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}


int Player::rand_vs_nega(int player)
{
    Move list[102];
    onmove = player;
    int result = 0;
    int rand_win = 0;
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
    
    if(onmove == -1)
    {
        move_index = 0;
        result = negamax(onmove, DEPTH, 0);
//        fprintf(stdout, "%d\n\n", result);
    }
    else
    {
        int rando = rand() % moves;
        rand_win = move(list[rando]);
        display();
    }
    
    //king caught
    if(rand_win == 1)
        return -1;
    if(result >= 10000)
        return 1;
    else if(result <= -10000)
        return -1;
    else
        val = -rand_vs_nega(- player);
    
    if(val > max_val)
        max_val = val;
    
    return max_val;
}


int Player::test(int player)
{
    onmove = player;
    read_board();
    display();
    int val = negamax(player, DEPTH, 0);
    fprintf(stdout, "%d\n", val);
    fprintf(stdout, "Move_index: %d\n", move_index);

    return 0;
}


//Task:
int Player::imcs_play(int argc, char ** argv)
{
    //O W DeepBlue Password
    setlinebuf(stdout);
    if(argc < 5 || argc > 7)
    {
        fprintf(stderr, "arg count not correct\n");
        exit(0);
    }

    char mecolor = '?';
    int megame = 0;
    
    //fprintf(stdout, "%c\n", argv[1][0]); //O
    //fprintf(stdout, "%d\n", argv[2][0]); //W/B

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
                    else if(onmove == 'B')
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
            else if(ch == 'W' || ch == 'B')
            {
                mecolor = ch;
                megame = atoi(&argv[2][1]);
                
                //set onmove to correct player
                if(mecolor == 'W')
                    onmove = -1;
                else if(onmove == 'B')
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
    //can start using my code here

    //need to pull from nf
    //cover cases for ?, !, =, X, 
    
    while(1)
    {
        int ch = fgetc(nf);
        int r = ungetc(ch, nf);
        assert(r != EOF);
//        net.logmsg((char*)"ch= %c\n", ch);
        
//        if(isdigit(ch))
//        {
            //readstate/eval
//            continue;
//        }

        switch(ch)
        {
            case '?':
            {
                char * r = net.getnet(nf, (char*)"?");
                char * q = strtok(r, " ");
                assert(!strcmp(q, "?"));

                //insert time management here
                move_index = 0; //reset move index
                negamax(onmove, DEPTH, 0);
                net.logmsg((char*)"made move %s\n\n",string);
                net.sendcmd(nf, string);
                //if ponder then run negamax again
                continue;
            }
            case '!':
                assert(fgetc(nf) == '!');
                int ch;
                char temp[6];
                do
                    ch = fgetc(nf);
                while(isspace(ch));
                ungetc(ch, nf);
                fscanf(nf, "%s", temp);

                net.logmsg((char*)"received move %s\n\n",temp);
                move(temp); //make move
                continue;
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
