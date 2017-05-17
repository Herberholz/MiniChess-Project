//Cody Herberholz
//cherber2@pdx.edu
//Minichess project

#include "board.h"

//would be useful to have a piece list that contains locations of each piece on board
//each opponent would have their own piece list



//Constructor for struct
Move::Move()
{
   from_row = -1;
   from_col = -1;
   to_row = -1;
   to_col = -1;
   f_col_coord = '\0';
   t_col_coord = '\0';
   f_row_coord = -1;
   t_row_coord = -1;
   promotion = -1;
   piece_capture = -1;
   score = -1;
}



//Constructor
Board::Board()
{
    onmove = -1;  //indicates that white is on move
    move_num = 0; //holds how many moves have gone by
    move_index = 0; //set to zero to indicate first move is best unless specified
    string[0] = '\0';

    //XXX temporarily hard code board and then copy to actual board
    
    char temp[ROW_DIM][COL_DIM] = {
        {'k','q','b','n','r','\0'},
        {'p','p','p','p','p','\0'},
        {'.','.','.','.','.','\0'},
        {'.','.','.','.','.','\0'},
        {'P','P','P','P','P','\0'},
        {'R','N','B','Q','K','\0'}
    };
    
    for(int i = 0; i < ROW_DIM; ++i)
        for(int j = 0; j < COL_DIM; ++j)
            board[i][j] = temp[i][j];
}



//Task:
void Board::display()
{
    char color;

    if(onmove == 1)
        color = 'B';
    else if(onmove == -1)
        color = 'W';
    else
    {
        fprintf(stderr, "onmove is not valid\n");
        exit(0);
    }

    fprintf(stdout, "%d %c\n", move_num, color);
    for(int i = 0; i < ROW_DIM; ++i)
        fprintf(stdout, "%s\n", board[i]);
    
    printf("\n");
}



//Task:
void Board::display_moves(Move list[], int index)
{
    for(int i = 0; i < index; ++i)
    {
        fprintf(stdout, "%c%d-%c%d\n", list[i].f_col_coord,list[i].f_row_coord,
                                       list[i].t_col_coord,list[i].t_row_coord);
        fprintf(stdout, "%d\n", list[i].score);    
    }
    fprintf(stdout, "\n");
}



//Task:
int Board::read_board()
{
    char color;

    //Take heed when reading into move_num
    //if my move_num is different from opponents at start will it be off??
    fscanf(stdin, "%d", &move_num);
    fscanf(stdin, "%s", &color);

    if(color == 'B')
        onmove = 1;
    else if(color == 'W')
        onmove = -1;
    else
    {
        fprintf(stderr, "Color not recognized\n");
        exit(0);
    }

    for(int i = 0; i < ROW_DIM; ++i)
        fscanf(stdin, "%s", board[i]);

    return 0;
}



//Task:
int Board::move(Move & loc)
{
    //simple make move
    //have to deal with promotions and points still
    int piece = 0;
    int queen = 0;
    int win = 0;

    if(onmove == 1)
        queen = 113;
    else if(onmove == -1)
        queen = 81;
 
    if(loc.to_row < 0 || loc.to_row > 5)
    {
        //display();
        fprintf(stdout, "%d\n", loc.to_row);
        fprintf(stderr, "Invalid row move\n");
        exit(0);
    }
    if(loc.to_col < 0 || loc.to_col > 4)
    {
        fprintf(stderr, "Invalid col move\n");
        exit(0);
    }

    piece = board[loc.from_row][loc.from_col];
    board[loc.from_row][loc.from_col] = 46;

    if(board[loc.to_row][loc.to_col] != 46)
    {
        loc.piece_capture = board[loc.to_row][loc.to_col];
        if(loc.piece_capture == 75 || loc.piece_capture == 107)
            win = 1;
    }

    //if move location is at end and piece is pawn then promote to queen
    if((loc.to_row == 0 || loc.to_row == 5) && (piece == 80 || piece == 112))
    {
        loc.promotion = piece;
        board[loc.to_row][loc.to_col] = queen;
    }
    else
        board[loc.to_row][loc.to_col] = piece;

    return win;
}



//Task:
int Board::move(char coord[])
{
    int piece   = 0;
    int win     = 0;
    int queen   = 0;
    int capture = 0;
    int fr_col  = 0;
    int to_col  = 0;
    int fr_row  = -1;
    int to_row  = -1;

    fr_col = coord[0] - 97; //a is 97, subtract that to get the col number
    to_col = coord[3] - 97;
    fr_row = coord[1] - 54;
    to_row = coord[4] - 54;

    if(fr_row != 0)
        fr_row = - fr_row;
    if(to_row != 0)
        to_row = - to_row;

    if(onmove == 1)
        queen = 113;
    else if(onmove == -1)
        queen = 81;

    piece = board[fr_row][fr_col];
    board[fr_row][fr_col] = 46;
    
    if(board[to_row][to_col] != 46)
    {
        capture = board[to_row][to_col];
        if(capture == 75 || capture == 107)
            win = 1;
    }
    
    if((to_row == 0 || to_row == 5) && (piece == 80 || piece == 112))
        board[to_row][to_col] = queen;
    else
        board[to_row][to_col] = piece;

    return win;
}



//Task:
int Board::undo_move(Move loc)
{
    int piece = 0;

    if(loc.to_row < 0 || loc.to_row > 5)
    {
        fprintf(stderr, "Invalid row move\n");
        exit(0);
    }
    if(loc.to_col < 0 || loc.to_col > 4)
    {
        fprintf(stderr, "Invalid col move\n");
        exit(0);
    }

    piece = board[loc.to_row][loc.to_col];
    board[loc.to_row][loc.to_col] = 46;
    board[loc.from_row][loc.from_col] = piece;

    //if capture happened then put piece back
    if(loc.piece_capture > 0)
    {
        board[loc.to_row][loc.to_col] = loc.piece_capture;
    }
    //if there was a promotion then undo
    if(loc.promotion > 0)
    {
        board[loc.from_row][loc.from_col] = loc.promotion;
    }
    return 0;
}


//Task:
int Board::move_order(Move source[], Move dest[], int total)
{
    int max = -100000;
    int grabbed[total]; //holds one if element transfered
    int hold = -1;

    //initialize grabbed
    for(int k = 0; k < total; ++k)
        grabbed[k] = 0;

    //fill move struct with values of best move until end and then swap 
    for(int i = 0; i < total; ++i)
    {
        max = -100000;
        hold = -1;

        for(int j = 0; j < total; ++j)
        {
            if(grabbed[j] == 0)
            {
                if(source[j].score > max)
                {
                    max = source[j].score;
                    hold = j; //holds location of max move score
                }
                else if(source[j].score == max)
                {
                    //take random if score is the same
                    int rando = rand() % 2;
                    if(rando)
                        hold = j;
                }
            }
        }
        grabbed[hold] = 1;
        dest[i].from_row = source[hold].from_row;
        dest[i].from_col = source[hold].from_col;
        dest[i].to_row = source[hold].to_row;
        dest[i].to_col = source[hold].to_col;
        
        dest[i].f_col_coord = source[hold].f_col_coord;
        dest[i].t_col_coord = source[hold].t_col_coord;
        dest[i].f_row_coord = source[hold].f_row_coord; 
        dest[i].t_row_coord = source[hold].t_row_coord;

        dest[i].promotion = source[hold].promotion;
        dest[i].piece_capture = source[hold].piece_capture;
        dest[i].score = source[hold].score;
}
    //display_moves(dest, total);
    
    return 0;
}


//Task:
int Board::scan(int x0,int y0,int dx,int dy,int stop_short,int capture,Move list[],int & index)
{
    //XXX save time by saving extra state to avoid piece scan
    int x = x0;
    int y = y0;
    int piece = board[y0][x0];
    int color1 = 0;
    int color2 = 0;

    if(piece > 97 && piece < 115)
        color1 = 1;
    else if(piece > 65 && piece < 83)
        color1 = 2;
    
    do
    {
        x += dx;
        y += dy;
        //changed boundaries to mimic real x and y graph
        //so x is j and y is i
        if(x < 0 || x > 4)
            break;
        if(y < 0 || y > 5)
            break;
        if(board[y][x] != 46) //board position != period
        {
            if(board[y][x] > 97 && board[y][x] < 115)
                color2 = 1;
            else if(board[y][x] > 65 && board[y][x] < 83)
                color2 = 2;
            if(color1 == color2)
                break;
            if(capture == 0)
                break;
            stop_short = 1;
        }
        else if(capture == 2)
            break;

        //fill_move(x0,y0,x,y,list,index);
        //insert move 
        list[index].from_row = y0;
        list[index].from_col = x0;
        list[index].to_row = y;
        list[index].to_col = x;
        
        list[index].f_col_coord = x0 + 97;
        list[index].t_col_coord = x + 97;
        list[index].f_row_coord = 6 - y0; 
        list[index].t_row_coord = 6 - y;

        state_eval(list[index]); //evaluate state
        
        ++index; //increment to next index
       
    }while(!stop_short);

    return 0;
}



//Task:
int Board::symm_scan(int x,int y,int dx,int dy,int stop_short,int capture,Move list[],int & index)
{
    int temp = 0;

    for(int i = 0; i < 4; ++i)
    {
        scan(x,y,dx,dy,stop_short,capture,list,index);
        temp = dx;
        dx = dy;
        dy = temp;
        dy = -dy;

    }
    return 0;
}



//Task:
int Board::move_list(int x, int y, Move list[], int & index)
{
    //XXX Move this beginning bit out of the double for loop to save time
    //store in struct? piece_rep
    //-----------------------------------------------------------------------
    int stop_short = 0; //indicates whether to stop or not
    int capture = 1; //default true, then false or only
    int direction = 1; //determines where to move pawn on y axis
    int piece = 0;
    int king = 0;
    int queen = 0;
    int bishop = 0;
    int knight = 0;
    int rook = 0;
    int pawn = 0;

    if(onmove == 1)
    {
        king = 107;
        queen = 113;
        bishop = 98;
        knight = 110;
        rook = 114;
        pawn = 112;
    }
    else if(onmove == -1)
    {
        king = 75;
        queen = 81;
        bishop = 66;
        knight = 78;
        rook = 82;
        pawn = 80;
    }
    else
    {
        fprintf(stderr, "Onmove not recognized");
        exit(0);
    }
    //------------------------------------------------------------------------

    piece = board[y][x];

    if(piece == king || piece == queen)
    {
        if(piece == king)
            stop_short = 1;

        symm_scan(x,y,0,1,stop_short,capture,list,index);
        symm_scan(x,y,1,1,stop_short,capture,list,index);
        return 1;
    }
    //rook can capture vert/horiz but not bishop
    else if(piece == bishop || piece == rook)
    {
        if(piece == bishop)
        {
            stop_short = 1;
            capture = 0;
        }
        symm_scan(x,y,0,1,stop_short,capture,list,index);

        if(piece == bishop)
        {
            stop_short = 0;
            capture = 1;
            symm_scan(x,y,1,1,stop_short,capture,list,index);
        }
        return 1;
    }
    else if(piece == knight)
    {
        symm_scan(x,y,1,2,true,capture,list,index);
        symm_scan(x,y,-1,2,true,capture,list,index);
        return 1;
    }
    else if(piece == pawn)
    {
        direction = -1;
        if(onmove == 1)
            direction = 1;
        scan(x,y,-1,direction,true,2,list,index); //can only capture
        scan(x,y,1,direction,true,2,list,index);  //can only capture
        scan(x,y,0,direction,true,0,list,index);  //can't capture
    }
    else
    {
        fprintf(stderr, "Piece identification error");
        exit(0);
    }

    return 0;
}



//Task:
//Output: Returns a list of strings/arrays (a1-a2, b1-b2)
int Board::movegen(Move list[])
{
    //XXX need to try and improve performance of move gen. 
    //learn about lazy move generation and add move ordering
    //try to overload + operator to add arrays rather than pass as argument

    int index = 0; //when i add move increment index
    int lower_bound = 0;
    int upper_bound = 0;
    int piece = 0;

    //might need to pass in onmove from prune algorithm
    if(onmove == 1)
    {
        lower_bound = 97;
        upper_bound = 115;
    }
    else if(onmove == -1)
    {
        lower_bound = 65;
        upper_bound = 83;
    }
    else
    {
        fprintf(stderr, "Onmove not recognized");
        exit(0);
    }

    //need to pass array in and use index to mark where we are
    for(int i = 0; i < ROW_DIM; ++i)
    {
        for(int j = 0; j < COL_DIM-1; ++j)//used Col_dim - 1 to avoid null character
        {
            piece = board[i][j];

            if(piece > lower_bound && piece < upper_bound)
            {
                move_list(j,i,list,index); //switched j and i to mimic positions of x & y
            }
        }
    }

//    display_moves(list,index);
    
//    for(int i = 0; i < index; ++i)
//    {
//        win = move(list[i]);
//        display();
//        undo_move(list[i]);
//        display();
    //}

    return index;
}



//count up scores for both players and take difference
//must change program to pick random move worst for opponent
//program must calculate value of state opponent gets for each possible move, choose worst
//value of state is positive when side on move has advantage
//Task:
int Board::state_eval(Move &loc)
{
    //take in single move struct
    //make move and eval state then undo move made and return score/add score to struct
    //-make sure to increase points for pawn that has free pass to promotion
    //XXX Make sure to keep track of overall score for each player, not each move individually
    int result = 0;
    int piece = 0;
    int player = 0;
    int score1 = 0; //holds score for player 1
    int score2 = 0; //holds score for player 2
    int sum = 0;    //holds difference between scores
    
    result = move(loc); //returns if king is taken

    if(result == 1)
    {
        undo_move(loc);
        loc.score = 10000;
        return 1;
    }
 
    for(int i = 0; i < ROW_DIM; ++i)
    {
        for(int j = 0; j < COL_DIM-1; ++j)
        {
            piece = board[i][j];
            
            if(piece > 97 && piece < 115)
                player = 1; //Black
            else if(piece > 65 && piece < 83)
                player = 2; //White
            
            if(piece != 46)
            {
                if(player == 1)
                {
                    if(piece == 107)
                        score1 = score1; //dont change for king
                    else if(piece == 98)//bishop
                        score1 += 300;
                    else if(piece == 110)//knight
                        score1 += 300;
                    else if(piece == 112)//pawn
                        score1 += 100;
                    else if(piece == 113)//queen
                        score1 += 900;
                    else if(piece == 114)//rook
                        score1 += 500;
                    else
                    {
                        fprintf(stderr, "unknown piece in eval\n");
                        exit(0);
                    }
                }
                else if(player == 2)
                {
                    if(piece == 75)
                        score2 = score2; //dont change for king
                    else if(piece == 66)//bishop
                        score2 += 300;
                    else if(piece == 78)//knight
                        score2 += 300;
                    else if(piece == 80)//pawn
                        score2 += 100;
                    else if(piece == 81)//queen
                        score2 += 900;
                    else if(piece == 82)//rook
                        score2 += 500;
                    else
                    {
                        fprintf(stderr, "unknown piece in eval\n");
                        exit(0);
                    }
                }
                else
                {
                    fprintf(stderr, "Error in state_eval\n");
                    exit(0);
                }
            }
        }
    }
    if(onmove == 1)
        sum = score1 - score2;
    else if(onmove == -1)
        sum = score2 - score1;
    loc.score = sum;
    undo_move(loc);

    return 0;
}


//Task:
int Board::negamax(int player, int depth, int score)
{
    //move_index = 0;
    if(depth <= 0 || score >= 10000)
    {
        return -score; //flip score since it is score from opponent
    }
//    fprintf(stdout, "%d\n", onmove);
    onmove = player; //be cautious when returning that i am on right player move
//    fprintf(stdout, "%d\n\n", onmove);

    Move list[102];
    int moves = movegen(list);
   
    //if no legal moves, then return loss
    if(moves == 0)
    {
        onmove = - player;
        return -10000;
    }
    Move ordered[moves];
    move_order(list, ordered, moves);
    
//    display_moves(ordered, moves);
 
    move(ordered[0]); //make move on board
//    display();    
//    if(depth == DEPTH-1)
//        display();
    int max_val = - negamax(- player, depth - 1, ordered[0].score);

    onmove = player;

    undo_move(ordered[0]);//undo move made
    
    //fprintf(stdout, "Player: %d\n", player);
    //fprintf(stdout, "Max_val: %d\n", max_val);        
    //display_moves(ordered, moves);

    for(int i = 1; i < moves; ++i) //starts at element 1 due to move 0 being taken
    {
        onmove = player;

        move(ordered[i]);
//        display();
//        if(depth == DEPTH-1)
//            display();
        int val = - negamax(- player, depth - 1, ordered[i].score);

    //    fprintf(stdout, "%d\n", onmove);
        onmove = player;
        undo_move(ordered[i]);

    //    fprintf(stdout, "Val: %d\n\n", val);
  
//        if(depth == DEPTH)
//        {
//            fprintf(stdout, "Player: %d\n", player);
//            fprintf(stdout, "Max_val: %d\n", max_val);
//            fprintf(stdout, "Val: %d\n\n", val);
//        }
        if(val > max_val)
        {
            max_val = val;
            if(depth == DEPTH) //keep track of which move leads to best path
            {               
                move_index = i;
            }
        }

    }

    if(depth == DEPTH)
    {
        //if max val is take king then do it
//        if(max_val == 10000)
//        {
//            fprintf(stdout, "takeKing\n");
//            move(ordered[0]);
//            display();
//            sprintf(string, "%c%d-%c%d", ordered[0].f_col_coord,
//                                         ordered[0].f_row_coord,
//                                         ordered[0].t_col_coord,
//                                         ordered[0].t_row_coord);
//        }
//        else
//        {
//            fprintf(stdout, "%d\n", onmove);
            onmove = player;
            move(ordered[move_index]);
            display();
            sprintf(string, "%c%d-%c%d", ordered[move_index].f_col_coord,
                                         ordered[move_index].f_row_coord,
                                         ordered[move_index].t_col_coord,
                                         ordered[move_index].t_row_coord);

//        }
//        fprintf(stdout, "%d\n", move_index);
//        display_moves(ordered, moves);
    }

    //onmove = - player;
    return max_val;
}


//Task:
int Board::ab_prune(int player)
{
    
    return 0;
}
