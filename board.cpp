//Cody Herberholz
//cherber2@pdx.edu
//Minichess project

#include "board.h"



//Constructor for move struct
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



//Constructor for piece representation struct
Piece_rep::Piece_rep()
{
    king = 0;
    queen = 0;
    bishop = 0;
    knight = 0;
    rook = 0;
    pawn = 0;
};



//Constructor for piece flag values
Piece_flag::Piece_flag()
{
    w_queen = 0;  
    b_queen = 0; 
    w_rook = 0;   
    b_rook = 0;  
    w_bishop = 0;
    b_bishop = 0;
    w_knight = 0; 
    b_knight = 0;
};



//Constructor
Board::Board()
{
    onmove = -1;      //indicates that white is on move
    move_num = 1;     //holds how many moves have gone by
    move_index = 0;   //set to zero to indicate first move is best unless specified
    string[0] = '\0'; //holds the string representation of board coordinates
    ndepth = 0;       //holds depth of negamax
    abdepth = 0;      //holds depth of alpha beta

    char temp[ROW_DIM][COL_DIM] = {
        {'k','q','b','n','r','\0'},
        {'p','p','p','p','p','\0'},
        {'.','.','.','.','.','\0'},
        {'.','.','.','.','.','\0'},
        {'P','P','P','P','P','\0'},
        {'R','N','B','Q','K','\0'}
    };
    
    //copies board over
    for(int i = 0; i < ROW_DIM; ++i)
        for(int j = 0; j < COL_DIM; ++j)
            board[i][j] = temp[i][j];
}



//Task:   Displays move number, side on move, and board state
//Input:  Side on move
//Output: Board representation to stdout
void Board::display(int player)
{
    char color; //holds what color is on move

    if(player == 1)
        color = 'B';
    else if(player == -1)
        color = 'W';
    else
    {
        fprintf(stderr, "Player is not valid within display\n");
        exit(0);
    }

    fprintf(stdout, "%d %c\n", move_num, color);
    for(int i = 0; i < ROW_DIM; ++i)
        fprintf(stdout, "%s\n", board[i]);
    
    printf("\n");
}



//Task:   Used to display the list of moves available
//Input:  List of moves and the index which indicates how far to travel in array
//Output: List of available moves to stdout
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



//Task:   Reads in a board representation from stdin
//Input:  Board representation, move number , and side on move
//Output: N/A
int Board::read_board()
{
    char color; //holds side on move
    int result = 0;

    result = fscanf(stdin, "%d", &move_num);
    result = fscanf(stdin, "%s", &color);

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
        result = fscanf(stdin, "%s", board[i]);

    if(!result)
    {
        fprintf(stdout, "Error with stdin");
        exit(0);
    }

    display(onmove); //displays board that was read in
    return 0;
}



//Task:   Takes a single move struct and uses it to physically make move on board
//Input:  Single move struct
//Output: Returns if win was found from making move
int Board::move(Move & loc, int player)
{
    int piece = 0; //holds piece at point of origin
    int queen = 0; //holds ascii value of queen for side on move
    int win = 0;   //holds whether move ends in a win

    if(player == 1)
        queen = 113;
    else if(player == -1)
        queen = 81;

    //if row/col are invalid then return error and crash
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

    //store piece at origin and set to '.'
    piece = board[loc.from_row][loc.from_col];
    board[loc.from_row][loc.from_col] = 46;

    //if move location is not empty then handle capture case
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



//Task:   Takes in a char array containing valid move
//Input:  Char array containing valid move (EX: a2-a3)
//Output: Returns if win condition is met
int Board::move(char coord[],int player)
{
    int piece   = 0;  //holds piece at origin location
    int win     = 0;  //holds if win condition met
    int queen   = 0;  //holds ascii value of queen for side on move
    int capture = 0;  //holds piece to be captured and checks if it is a king
    int fr_col  = -1;  //origin col
    int to_col  = -1;  //move col
    int fr_row  = -1; //origin row
    int to_row  = -1; //move row

    fr_col = coord[0] - 97; //a is 97, subtract that to get the col number
    to_col = coord[3] - 97; //a is 97, subtract that to get the col number
    fr_row = coord[1] - 54; //subtracts value by ascii value of 6
    to_row = coord[4] - 54; //subtracts value by ascii value of 6

    //if not 0 then flip sign
    if(fr_row != 0)
        fr_row = - fr_row;
    if(to_row != 0)
        to_row = - to_row;

    if(player == 1)
        queen = 113;
    else if(player == -1)
        queen = 81;
    else
    {
        fprintf(stderr, "player in move(coords) is invalid");
        fprintf(stderr, "player: %d\n", player);
        exit(0);
    }


    //capture piece value from origin and make '.'
    piece = board[fr_row][fr_col];
    board[fr_row][fr_col] = 46;
    
    //handle capture condition
    if(board[to_row][to_col] != 46)
    {
        capture = board[to_row][to_col];
        if(capture == 75 || capture == 107)
            win = 1;
    }
    
    //fprintf(stdout, "player: %d\n", player);
    //if moving to a back row and piece is a pawn then promote to queen
    if((to_row == 0 || to_row == 5) && (piece == 80 || piece == 112))
        board[to_row][to_col] = queen;
    else
        board[to_row][to_col] = piece;

    return win;
}



//Task:   Takes a move that is passed in and wipes it off the board
//Input:  Struct of move taken
//Output: N/A
int Board::undo_move(Move loc)
{
    int piece = 0; //holds piece from location being undone

    //if row/col are not in bounds then through an error and crash
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

    //undo move and put back at original location
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



//Task:   Takes in list of moves and sorts them by score of board state
//Input:  Takes original list, new list to be filled, and then number of moves
//Output: Returns new ordered list by reference 
int Board::move_order(Move source[], Move dest[], int total)
{
    int max = -100000;  //holds max score
    int grabbed[total]; //holds one if element transfered
    int hold = -1;      //holds index of move to be placed 

    //initialize grabbed
    for(int k = 0; k < total; ++k)
        grabbed[k] = 0;

    //XXX Try to find better sort algorithm

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
                #ifdef RANDOMIZE
                else if(source[j].score == max)
                {
                    //take random if score is the same
                    int rando = rand() % 2;
                    if(rando)
                        hold = j;
                }
                #endif
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
    
    return 0;
}



//Task:   Scans for available moves
//Input:  Takes in original x/y location and how far to move x/y, flag that indicates whether
//          to stop short, capture flag, move list, and index for move list
//Output: Fills move list by using pass by reference
int Board::scan(int x0,int y0,int dx,int dy,int stop_short,int capture,Move list[],int & index)
{
    int x = x0;                //sets x to origin
    int y = y0;                //sets y to origin
    int piece = board[y0][x0]; //holds piece value at current board location
    int color1 = 0;            //flag for back piece
    int color2 = 0;            //flag for white piece

    //sets color flag based on what ascii range piece falls into
    if(piece > 97 && piece < 115)
        color1 = 1;
    else if(piece > 65 && piece < 83)
        color1 = 2;
    
    //loop until other piece is hit or until piece leaves board
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



//Task:   Runs a symetrical scan of the board
//Input:  Takes in original x/y location and how far to move x/y, flag that indicates whether
//          to stop short, capture flag, move list, and index for move list
//Output: Fills move list by using pass by reference
int Board::symm_scan(int x,int y,int dx,int dy,int stop_short,int capture,Move list[],int & index)
{
    int temp = 0; //temporary variable

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



//Task:   Based on what piece is on the board, pass in specific arguements to symscan/scan
//Input:  Takes in location on board, move list to be filled , and index in move list
//Output: Returns Move list using pass by reference
int Board::move_list(int x, int y, Move list[], int & index)
{
    int stop_short = 0; //indicates whether to stop or not
    int capture = 1;    //default true, then false or only
    int direction = 1;  //determines where to move pawn on y axis
    int piece = 0;      //holds piece at board location
    
    piece = board[y][x];

    if(piece == piece_val.king || piece == piece_val.queen)
    {
        if(piece == piece_val.king)
            stop_short = 1;

        symm_scan(x,y,0,1,stop_short,capture,list,index);
        symm_scan(x,y,1,1,stop_short,capture,list,index);
        return 1;
    }
    //rook can capture vert/horiz but not bishop
    else if(piece == piece_val.bishop || piece == piece_val.rook)
    {
        if(piece == piece_val.bishop)
        {
            stop_short = 1;
            capture = 0;
        }
        symm_scan(x,y,0,1,stop_short,capture,list,index);

        if(piece == piece_val.bishop)
        {
            stop_short = 0;
            capture = 1;
            symm_scan(x,y,1,1,stop_short,capture,list,index);
        }
        return 1;
    }
    else if(piece == piece_val.knight)
    {
        symm_scan(x,y,1,2,true,capture,list,index);
        symm_scan(x,y,-1,2,true,capture,list,index);
        return 1;
    }
    else if(piece == piece_val.pawn)
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



//Task:   Goes through and finds all available moves of pieces for player on move
//Input:  Move list to be filled
//Output: Returns the index which is the total number of moves
int Board::movegen(Move list[], int player)
{
    //XXX need to try and improve performance of move gen. 
    //learn about lazy move generation and add move ordering
    //try to overload + operator to add arrays rather than pass as argument

    int index = 0; //when i add move increment index
    int lower_bound = 0;
    int upper_bound = 0;
    int piece = 0;
    onmove = player;

    //might need to pass in onmove from prune algorithm
    if(onmove == 1)
    {
        lower_bound = 97;
        upper_bound = 115;
        piece_val.king = 107;
        piece_val.queen = 113;
        piece_val.bishop = 98;
        piece_val.knight = 110;
        piece_val.rook = 114;
        piece_val.pawn = 112;
    }
    else if(onmove == -1)
    {
        lower_bound = 65;
        upper_bound = 83;
        piece_val.king = 75;
        piece_val.queen = 81;
        piece_val.bishop = 66;
        piece_val.knight = 78;
        piece_val.rook = 82;
        piece_val.pawn = 80;
    }
    else
    {
        fprintf(stderr, "Onmove not recognized");
        exit(0);
    }

    //need to pass array in and use index to mark where we are
    for(int i = 0; i < ROW_DIM; ++i)
    {
        for(int j = 0; j < COL_DIM-1; ++j) //used Col_dim - 1 to avoid null character
        {
            piece = board[i][j];

            if(piece > lower_bound && piece < upper_bound)
            {
                move_list(j,i,list,index); //switched j and i to mimic positions of x & y
            }
        }
    }

    return index;
}



//count up scores for both players and take difference
//must change program to pick random move worst for opponent
//program must calculate value of state opponent gets for each possible move, choose worst
//value of state is positive when side on move has advantage

//Task:   Find score for state of board by finding difference of pieces
int Board::state_eval(Move &loc)
{
    //take in single move struct
    //make move, eval state, and then undo move made. Return score/add score to struct
    //-make sure to increase points for pawn that has free pass to promotion
    
    int result = 0;      //holds return value from move function
    int piece = 0;       //holds piece value from board
    int player = 0;      //holds flag to indicate which color the piece is
    int score1 = 0;      //holds score for player 1
    int score2 = 0;      //holds score for player 2
    int sum = 0;         //holds difference between scores
    Piece_flag flag;     //struct holding indicators of if piece is present
//    int r = -1;          //holds new row value

    result = move(loc,onmove); //returns if king is taken

    //if king captured from move then set score and return
    if(result == 1)
    {
        undo_move(loc);
        loc.score = 10000;
        return 1;
    }
 
    //loop through board and add up points for both sides
    for(int i = 0; i < ROW_DIM; ++i)
    {
        for(int j = 0; j < COL_DIM-1; ++j)
        {
            piece = board[i][j];
            
            if(piece > 97 && piece < 115)
            {
                player = 1; //Black
//                r = i + 1;
            }
            else if(piece > 65 && piece < 83)
            {
                player = 2; //White
//                r = i - 1;
            }
            //king is not taken into account for score evaluation
            if(piece != 46)
            {
                if(player == 1)
                {
                    if(piece == 107)
                    {
                        score1 = score1; //dont change for king
  //                      if(move_num < 11)
  //                          score1 += eval_king(i,j,r); //if protected then add points
                    }
                    else if(piece == 98)//bishop
                    {
                        score1 += 300;
                        flag.b_bishop = 1;
                        if(i != 0 && j != 2)
                            score1 += 50;
                    }
                    else if(piece == 110)//knight
                    {
                        score1 += 300;
                        flag.b_knight = 1;
                        if(i != 0 && j != 3)
                            score1 += 50;
                    }
                    else if(piece == 112)//pawn
                    {
                        score1 += 100;
                        score1 += eval_pawn(i,j);
                    }
                    else if(piece == 113)//queen
                    {
                        score1 += 900;
                        flag.b_queen = 1; //says back has its queen
                    }
                    else if(piece == 114)//rook
                    {
                        score1 += 500;
                        flag.b_rook = 1;
                        if(i != 0 && j != 4)
                            score1 += 50;
                    }
                    else
                    {
                        fprintf(stderr, "unknown piece in eval\n");
                        exit(0);
                    }
                }
                else if(player == 2)
                {
                    if(piece == 75)
                    {
                        score2 = score2; //dont change for king
 //                       if(move_num < 11)
 //                           score2 += eval_king(i,j,r);
                    }
                    else if(piece == 66)//bishop
                    {
                        score2 += 300;
                        flag.w_bishop = 1;
                        if(i != 5 && j != 2)
                            score2 += 50;

                    }
                    else if(piece == 78)//knight
                    {
                        score2 += 300;
                        flag.w_knight = 1;
                        if(i != 5 && j != 1)
                            score2 += 50;
                    }
                    else if(piece == 80)//pawn
                    {
                        score2 += 100;
                        score2 += eval_pawn(i,j);
                    }
                    else if(piece == 81)//queen
                    {
                        score2 += 900;
                        flag.w_queen = 1; //says white has queen
                    }
                    else if(piece == 82)//rook
                    {
                        score2 += 500;
                        flag.w_rook = 1;
                        if(i != 5 && j != 0)
                            score2 += 50;
                    }
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

    
    //take difference based on who is on move
    if(onmove == 1)
    {
        score2 += avoid_sacrifice(loc.piece_capture, flag);
        sum = score1 - score2;
    }
    else if(onmove == -1)
    {
        score1 += avoid_sacrifice(loc.piece_capture, flag);
        sum = score2 - score1;
    }

    loc.score = sum;
    undo_move(loc);

    return 0;
}



//Task:   Evaluates the king position and gives extra points if it is protected
//Input:  Position on board and which row to search
//Output: Score for king positioning
int Board::eval_king(int i, int j, int r)
{
    int score = 0;
    int points = 200;
    int lbound = 0;
    int ubound = 0;

    if(onmove == 1)
    {
        lbound = 97;
        ubound = 115;
    }
    else if(onmove == -1)
    {
        lbound = 65;
        ubound = 85;
    }
    else
    {
        fprintf(stderr, "error in king eval");
        exit(0);
    }

    //makes sure black king has protection in front
    if(i != 5)
    {
        if(board[r][j] > lbound && board[r][j] < ubound)
            score += points;
        if(j != 0)
        {
            if(board[r][j-1] > lbound && board[r][j-1] < ubound)
                score += points;
        }
        if(j != 4)
        {
            if(board[r][j+1] > lbound && board[r][j+1] < ubound)
                score += points;
        }
    }
    //makes sure white king has protection in front
    else if(i == 5)
    {

        if(board[r][j] > lbound && board[r][j] < ubound)
        {
            score += points;
        }
        
        if(j != 0)
        {
            if(board[r][j-1] > lbound && board[r][j-1] < ubound)
                score += points;
        }
        if(j != 4)
        {
            if(board[r][j+1] > lbound && board[i][j+1] < ubound)
                score += points;
        }
    }

    //if on home square then add 50 points
    if(i == 5 || i == 0)
        score += 50;

    return score;
}



//Task:   Evaluates pawns in order to score based off positioning and structure
//Input:  Position on board
//Output: Score for pawn
int Board::eval_pawn(int i, int j)
{
    int score = 0;

    //----Need to test out pawn structure eval more----
    //implementing this slows down program so i don't search as deep
/*
    //Points for pawn structure
    if(j != 0)
    {
        if(board[i+1][j-1] == piece_val.pawn)
            score += 10;
        if(board[i][j-1] == piece_val.pawn) //connected pawn
            score += 15;
        if(board[i-1][j-1] == piece_val.pawn)
            score += 10;
    }
    if(j != 4)
    {
        if(board[i+1][j+1] == piece_val.pawn)
            score += 10;
        if(board[i][j+1] == piece_val.pawn) //connected pawn
            score += 15;
        if(board[i-1][j+1] == piece_val.pawn)
            score += 10;
    }

    if(board[i+1][j] == piece_val.pawn) //doubled pawn
        score -= 15;
    if(board[i-1][j] == piece_val.pawn) //doubled pawn
        score -= 15;

    //isolated pawn 
    if(score <= 0)
        score = -15;
*/

    //gives extra points for being close to promotion
    if(onmove == -1)
    {
        if(i == 1)
        {
            if(board[i-1][j] == 46)
                score += 300;
            else
                score += 150;
        }
        else if(i == 2)
        {
            if(board[i-1][j] == 46 && board[i-2][j] == 46)
                score += 200;
            else 
                score += 100;
        }
    }
    else if(onmove == 1)
    {
        if(i == 4)
        {
            if(board[i+1][j] == 46)
                score += 300;
            else 
                score += 150;
        }
        else if(i == 3)
        {
            if(board[i+1][j] == 46 && board[i+2][j] == 46)
                score += 200;
            else
                score += 100;
        }
    }
    //in opening game keep pawns protecting king
    if(move_num < 10)
    {
        if(onmove == -1)
        {
            if(i == 4 && j == 4)
            {
                if(board[i][j] == piece_val.pawn)
                    score += 50;
                if(board[i][j-1] == piece_val.pawn)
                    score += 50;
            }
        }
        if(onmove == 1)
        {
            if(i == 1 && j == 0)
            {
                if(board[i][j] == piece_val.pawn)
                    score += 50;
                if(board[i][j+1] == piece_val.pawn)
                    score += 50;
            }
        }
    }

    return score;
}



//Task:   If a piece was captured it makes sure that if piece is captured that 
//          onmove does not have. give negative points to avoid possible sacrifice
//Input:  Piece that was captured and struct of piece flags
//Output: The score that was calculated
int Board::avoid_sacrifice(int capture, Piece_flag flag)
{
    int score = 0;

    if(capture == 0)
        return 0;

    //if capture piece is a queen and other player queen flag not recognized
    //then subtract points for possible queen sacrifice
    if(capture == 113 || capture == 81)
    {
        if(onmove == 1 && flag.b_queen == 0)
            score = -200;
        if(onmove == -1 && flag.w_queen == 0)
            score = -200;
    }
    //if capture piece is knight and onmove has no knight subtract points for 
    //knight sacrifice
    else if(capture == 110 || capture == 78)
    {
        if(onmove == 1 && flag.b_knight == 0)
            score = -200;
        if(onmove == -1 && flag.w_knight == 0)
            score = -200;
    }
    
    //if capture piece is bishop and onmove has no bishop subtract points for 
    //bishop sacrifice
    else if(capture == 98 || capture == 66)
    {
        if(onmove == 1 && flag.b_bishop == 0)
            score = -200;
        if(onmove == -1 && flag.w_bishop == 0)
            score = -200;
    }
    
    //if capture piece is a rook and onmove has no rook subtract points 
    else if(capture == 114 || capture == 82)
    {
        if(onmove == 1 && flag.b_rook == 0)
            score = -200;
        if(onmove == -1 && flag.w_rook == 0)
            score = -200;
    }


    return score;
}



//Task:   Recursively goes through possible moves and finds best possible move to make
//Input:  Takes player on move, depth of search, and score of board state from move
//Output: Returns the best score found from search
int Board::negamax(int player, int depth, int score)
{
    //Base case that searches for depth of 0 or score to end game
    //score passed from opponent perspective so i negate the score when returning it
    if(depth <= 0 || score >= 10000)
    {
        return -score; //flip score since it is score from opponent
    }

    Move list[70];  //holds list of moves
    int moves = movegen(list, player); //generates list of moves
   
    //if no legal moves, then return loss
    if(moves == 0)
    {
        return -10000;
    }

    Move ordered[moves]; //holds list of ordered moves
    move_order(list, ordered, moves); //orders moves
    
    move(ordered[0], player); //make move on board
    int max_val = - negamax(- player, depth - 1, ordered[0].score);
    undo_move(ordered[0]);//undo move made
    
    for(int i = 1; i < moves; ++i) //starts at element 1 due to move 0 being taken
    {
        move(ordered[i], player); //makes move
        int val = - negamax(- player, depth - 1, ordered[i].score);

        undo_move(ordered[i]); //undo move

        if(val > max_val)
        {
            max_val = val;
            if(depth == DEPTH) //keep track of which move leads to best path
            {               
                move_index = i;
            }
        }
    }

    //before last return make selected move, display it, and then store in string
    if(depth == DEPTH)
    {
            onmove = player;
            move(ordered[move_index], player);
            display(-player);

            snprintf(string, 6,"%c%d-%c%d", ordered[move_index].f_col_coord,
                                         ordered[move_index].f_row_coord,
                                         ordered[move_index].t_col_coord,
                                         ordered[move_index].t_row_coord);
            #ifdef UNDO_NEGA
            undo_move(ordered[move_index]);
            #endif
    }
    return max_val;
}



//Task:   Will search through state space like negamax but prune out branches of 
//          the tree to speed up the search, thus allowing for deeper searches
//Input:  Player on move, Depth to search, score of state, alpha and beta value
//Output: The value of the best state evaluation found
int Board::ab_prune(int player, int depth, int score, int alpha, int beta)
{
    //Base case that searches for depth of 0 or score to end game
    //score passed from opponent perspective so i negate the score when returning it
    if(depth <= 0 || score >= 10000)
    {
        return -score; //flip score since it is score from opponent
    }
 
    Move list[70];  //holds list of moves
    int moves = movegen(list, player); //generates list of moves
   
    //if no legal moves, then return loss
    if(moves == 0)
    {
        return -10000;
    }

    Move ordered[moves]; //holds list of ordered moves
    move_order(list, ordered, moves); //orders moves
    
    move(ordered[0], player); //make move on board
    int max_val = - ab_prune(- player, depth - 1, ordered[0].score, - beta, - alpha);

    undo_move(ordered[0]);//undo move made

    if(max_val >= beta)
    {

        if(depth == ABDEPTH)
        {
//            fprintf(stdout, "BEGINNING\n");
            move(ordered[0], player);
            display(-player);
            snprintf(string, 6,"%c%d-%c%d", ordered[0].f_col_coord,
                                         ordered[0].f_row_coord,
                                         ordered[0].t_col_coord,
                                         ordered[0].t_row_coord);

            #ifdef UNDO_AB
            undo_move(ordered[move_index]);
            #endif

        }
        
        return max_val;
    }

    if(max_val > alpha)
        alpha = max_val;

    for(int i = 1; i < moves; ++i) //starts at element 1 due to move 0 being taken
    {
        move(ordered[i], player); //makes move
        int val = - ab_prune(- player, depth - 1, ordered[i].score, - beta, - alpha);
        undo_move(ordered[i]); //undo move

        if(val >= beta)
        {
            if(depth == ABDEPTH) //keep track of which move leads to best path
            {               
//                fprintf(stdout, "Made itttttttttttt\n");
                move_index = i;
                move(ordered[move_index], player);
                display(-player);
                snprintf(string, 6,"%c%d-%c%d", ordered[move_index].f_col_coord,
                                             ordered[move_index].f_row_coord,
                                             ordered[move_index].t_col_coord,
                                             ordered[move_index].t_row_coord);
                #ifdef UNDO_AB
                undo_move(ordered[move_index]);
                #endif

            }
            return val;
        }

        if(val > max_val)
        {
            max_val = val;
            if(depth == ABDEPTH) //keep track of which move leads to best path
                move_index = i;
        }
        if(val > alpha)
            alpha = val;
    }

    //before last return make selected move, display it, and then store in string
    if(depth == ABDEPTH)
    {
//        fprintf(stdout, "EXIT\n");
        move(ordered[move_index], player);
        display(-player);

        snprintf(string, 6,"%c%d-%c%d", ordered[move_index].f_col_coord,
                                     ordered[move_index].f_row_coord,
                                     ordered[move_index].t_col_coord,
                                     ordered[move_index].t_row_coord);

        #ifdef UNDO_AB
        undo_move(ordered[move_index]);
        #endif

    }
    return max_val;
}


//Task:   Recursively goes through possible moves and finds best possible move to make
//Input:  Takes player on move, depth of search, and score of board state from move
//Output: Returns the best score found from search
int Board::id_negamax(int player, int depth, int score)
{
    //Base case that searches for depth of 0 or score to end game
    //score passed from opponent perspective so i negate the score when returning it
    if(depth <= 0 || score >= 10000)
    {
        return -score; //flip score since it is score from opponent
    }

    Move list[70];  //holds list of moves
    int moves = movegen(list, player); //generates list of moves
   
    //if no legal moves, then return loss
    if(moves == 0)
    {
        return -10000;
    }

    Move ordered[moves]; //holds list of ordered moves
    move_order(list, ordered, moves); //orders moves
    
    move(ordered[0], player); //make move on board
    int max_val = - id_negamax(- player, depth - 1, ordered[0].score);
    undo_move(ordered[0]);//undo move made
    
    for(int i = 1; i < moves; ++i) //starts at element 1 due to move 0 being taken
    {
        gettimeofday(&time_end, NULL);
        timersub(&time_end, &time_start, &time_result);
        if(time_result.tv_sec >= TIME)
            return max_val;

        move(ordered[i], player); //makes move
        int val = - id_negamax(- player, depth - 1, ordered[i].score);

        undo_move(ordered[i]); //undo move

        if(val > max_val)
        {
            max_val = val;
            if(depth == ndepth && time_result.tv_sec < TIME) //keep track of which move leads to best path
            {               
                move_index = i;
            }
        }
    }

    //before last return make selected move, display it, and then store in string
    if(depth == ndepth && time_result.tv_sec < TIME)
    {
            onmove = player;
            snprintf(string, 6,"%c%d-%c%d", ordered[move_index].f_col_coord,
                                         ordered[move_index].f_row_coord,
                                         ordered[move_index].t_col_coord,
                                         ordered[move_index].t_row_coord);
    }
    return max_val;
}



//Task:   Will search through state space like negamax but prune out branches of 
//          the tree to speed up the search, thus allowing for deeper searches
//Input:  Player on move, Depth to search, score of state, alpha and beta value
//Output: The value of the best state evaluation found
int Board::id_ab_prune(int player, int depth, int score, int alpha, int beta)
{
    //Base case that searches for depth of 0 or score to end game
    //score passed from opponent perspective so i negate the score when returning it
    if(depth <= 0 || score >= 10000)
    {
        return -score; //flip score since it is score from opponent
    }

    Move list[70];  //holds list of moves
    int moves = movegen(list, player); //generates list of moves
   
    //if no legal moves, then return loss
    if(moves == 0)
    {
        return -10000;
    }

    Move ordered[moves]; //holds list of ordered moves
    move_order(list, ordered, moves); //orders moves
    
    move(ordered[0], player); //make move on board
    int max_val = - id_ab_prune(- player, depth - 1, ordered[0].score, - beta, - alpha);

    undo_move(ordered[0]);//undo move made

        gettimeofday(&time_end, NULL);
        timersub(&time_end, &time_start, &time_result);
        if(time_result.tv_sec >= TIME)
            return max_val;

    if(max_val >= beta)
    {

        if(depth == abdepth && time_result.tv_sec < TIME)
        {
//            fprintf(stdout, "BEGINNING\n");
              snprintf(string, 6,"%c%d-%c%d", ordered[0].f_col_coord,
                                         ordered[0].f_row_coord,
                                         ordered[0].t_col_coord,
                                         ordered[0].t_row_coord);

        }
        
        return max_val;
    }

    if(max_val > alpha)
        alpha = max_val;

    for(int i = 1; i < moves; ++i) //starts at element 1 due to move 0 being taken
    {
        gettimeofday(&time_end, NULL);
        timersub(&time_end, &time_start, &time_result);
        if(time_result.tv_sec >= TIME)
            return max_val;

        move(ordered[i], player); //makes move
        int val = - id_ab_prune(- player, depth - 1, ordered[i].score, - beta, - alpha);

        undo_move(ordered[i]); //undo move

        if(val >= beta)
        {
            if(depth == abdepth && time_result.tv_sec < TIME) //keep track of which move leads to best path
            {               
//                fprintf(stdout, "Made itttttttttttt\n");
                move_index = i;
                snprintf(string, 6,"%c%d-%c%d", ordered[move_index].f_col_coord,
                                             ordered[move_index].f_row_coord,
                                             ordered[move_index].t_col_coord,
                                             ordered[move_index].t_row_coord);
            }
            return val;
        }

        if(val > max_val)
        {
            max_val = val;
            if(depth == abdepth && time_result.tv_sec < TIME) //keep track of which move leads to best path
                move_index = i;
        }
        if(val > alpha)
            alpha = val;
    }

    //before last return make selected move, display it, and then store in string
    if(depth == abdepth && time_result.tv_sec < TIME)
    {
//        fprintf(stdout, "EXIT\n");
          snprintf(string, 6,"%c%d-%c%d", ordered[move_index].f_col_coord,
                                     ordered[move_index].f_row_coord,
                                     ordered[move_index].t_col_coord,
                                     ordered[move_index].t_row_coord);

    }
    return max_val;
}



//Task:   As long as time permits I will increment the depth searched by 2 and 
//          use the best move from the last full search when time runs out
//Input:  Takes in the player on move
//Output: returns the max value returned from alpha beta
int Board::iterative_deep(int player)
{
    //save move coords from each depth search and make move once out of for loop
    //within negamax check time, if over then return and use last move
    //search for 8 seconds
    int value = 0; 
    ndepth = 2;
    abdepth = 2;
    
    gettimeofday(&time_start, NULL);
    
//    value = id_negamax(player, ndepth, 0);
    value = id_ab_prune(player, abdepth, 0, -10000, 10000);
    if(onmove != player)
        onmove = player;

    //since depth of 2 counts as 1 move for both sides i start there and then
    //increment by two each time. I divide i by 2 to represent a single move
    //that will be added to move num. To search move 40 at depth two i will get
    //41 and thus i check for if value is less than 42

    for(int i = 4; ((i/2) + move_num) < 42; i+=2)
    {
        //ndepth = i;
        abdepth = i;
        move_index = 0;
//        value = id_negamax(player, ndepth, 0);

        value = id_ab_prune(player, abdepth, 0, -10000, 10000);
        if(onmove != player)
            onmove = player;
        
        gettimeofday(&time_end, NULL);
        timersub(&time_end, &time_start, &time_result);
        
//        fprintf(stdout, "%d seconds %d microseconds\n", (int)time_result.tv_sec, (int)time_result.tv_usec);
        
        //if time passes 8 seconds then break loop
        if(time_result.tv_sec >= TIME)
        {
            abdepth -= 2;
            break;
        }
        if(value == -10000)
            break;
        if(value == 10000)
            break;
//        if(i == ABDEPTH)
//            break;
    }

    //fprintf(stdout, "Depth: %d\n", ndepth);
    fprintf(stdout, "Depth: %d\n", abdepth);
    move(string, player);
    display(-player);
    return value;
}
