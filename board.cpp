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
   //move_coord[0] = '\0';
   f_col_coord = '\0';
   t_col_coord = '\0';
   f_row_coord = -1;
   t_row_coord = -1;
   promotion = -1;
   piece_capture = -1;
}

//Constructor
Board::Board()
{
    onmove = -1;  //indicates that white is on move
    move_num = 0; //holds how many moves have gone by

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
   }
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
    
    if(coord[1] == '6' || coord[4] == '6')
    {
        if(coord[1] == '6')
            fr_row = 0;
        if(coord[4] == '6') 
            to_row = 0;
    }
    if(coord[1] == '5' || coord[4] == '5')
    {
        if(coord[1] == '5')
            fr_row = 1;
        if(coord[4] == '5') 
            to_row = 1;
    }
    if(coord[1] == '4' || coord[4] == '4')
    {
        if(coord[1] == '4')
            fr_row = 2;
        if(coord[4] == '4') 
            to_row = 2;
    }
    if(coord[1] == '3' || coord[4] == '3')
    {
        if(coord[1] == '3')
            fr_row = 3;
        if(coord[4] == '3') 
            to_row = 3;
    }
    if(coord[1] == '2' || coord[4] == '2')
    {
        if(coord[1] == '2')
            fr_row = 4;
        if(coord[4] == '2') 
            to_row = 4;
    }
    if(coord[1] == '1' || coord[4] == '1')
    {
        if(coord[1] == '1')
            fr_row = 5;
        if(coord[4] == '1') 
            to_row = 5;
    }
    
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
int Board::fill_move(int x0,int y0,int x, int y,Move list[],int & index)
{   
    int f_row_change = 0;
    int t_row_change = 0;
        
    //insert move 
    list[index].from_row = y0;
    list[index].from_col = x0;
    list[index].to_row = y;
    list[index].to_col = x;
    
    //let the move funtion handle creating board coordinates
    // XXX Fix ugly code
    if(y0 == 0 || y == 0)
    {
        if(y0 == 0)
            f_row_change = 6;
        if(y == 0) 
            t_row_change = 6;
    }
    if(y0 == 1 || y == 1)
    {
        if(y0 == 1)
            f_row_change = 5;
        if(y == 1)
            t_row_change = 5;
    }
    if(y0 == 2 || y == 2)
    {
        if(y0 == 2)
            f_row_change = 4;
        if(y == 2)
            t_row_change = 4;
    }
    if(y0 == 3 || y == 3)
    {
        if(y0 == 3)
            f_row_change = 3;
        if(y == 3)
            t_row_change = 3;
    }
    if(y0 == 4 || y == 4)
    {
        if(y0 == 4)
            f_row_change = 2;
        if(y == 4)
            t_row_change = 2;
    }
    if(y0 == 5 || y == 5)
    {
        if(y0 == 5)
            f_row_change = 1;
        if(y == 5)
            t_row_change = 1;
    }

    //XXX may not need coords?
    list[index].f_col_coord = x0 + 97;
    list[index].t_col_coord = x + 97;
    list[index].f_row_coord = f_row_change; 
    list[index].t_row_coord = t_row_change;

    ++index; //increment to next index


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

        fill_move(x0,y0,x,y,list,index);
    
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

//    fprintf(stdout, "%d\n", index);   
//    display_moves(list,index);
    
    //for(int i = 0; i < index; ++i)
    //{
//        win = move(list[i]);
//        display();
//        undo_move(list[i]);
//        display();
    //}

    return index;
}

//Task:
int Board::ab_prune()
{

    return 0;
}
