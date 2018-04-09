# Cody Herberholz
# MiniChess-Project



Requirements

For this project, I was required to implement a timed Iterative Deepening 
negamax alpha-beta pruned search and be able to successfully play games on the 
Internet MiniChess Server using this search.



Program Architecture

My program is implemented in C++ using Object Oriented Programming.  I have a 
Board class that handles the board itself along with all of the search and 
evaluation implementations. I then have a Player class derived from board which 
contains the Imcs play function along with all of my test functions. I have 
another class Netops which contains everything needed to manage the server 
portion of the program. This code was borrowed from 
//https://github.com/BartMassey/imcs/tree/master/client/c.
In order to successfully run the program, make sure to compile with make and 
then use the a.out executable in order to connect to Imcs. 
Examples shown below:
    ./a.out A W18000 username password
    ./a.out O B username password

Testing feature inputs (explained further in Feature Set section):
    ./a.out –random
    ./a.out –1vrand
    ./a.out –1vnega
    ./a.out –randvnega
    ./a.out –abvnega
    ./a.out –test < test1.txt
    ./a.out –testab
    ./a.out –id < test1.txt



Feature Set

The program can connect and play games on the Imcs server as well as provides a 
suite of testing features. These features include being able to play a random 
player vs random player, random player vs human player, negamax vs human, 
random vs negamax, abprune vs negamax, a test function that reads in a board 
state and displays a move, another test function that plays negamax and abprune 
against a negamax in order to make sure that they make the same move at the same 
depth, and then a function that reads in a board and displays abprune move and 
then iterative deepening abprune move where a cutoff can be applied to show 
that they make the same move. 
One of the novel features that my player contains is that when performing 
iterative deepening I increment by two each time. This allows me to reach a 
deeper state then 1 depth increase per loop. This also allows my player to 
evaluate and give a move with confidence on how the opponent will react to my 
move. With a depth of 7 I would make my move without seeing how the opponent 
will react to my move. My method avoids this unknown state. I have an even 
time limit of 8 seconds per move, as my move number gets closer to draw state 
I don’t search past that. This shrinks my search time, thus allowing me to 
search a little bit longer than the original 7 seconds.
For my evaluator, I implemented pawn evaluation, king evaluation, and a function 
that negates points for sacrificing major pieces. I originally gave out points 
or subtracted points for pawn structure (connected, backward, isolated, doubled pawns) 
but commented that out for the moment in favor of the speed increase. I kept 
the boosted points to pawns having a free pass to the opponents home row to help 
push for pawn promotions and then gave bonus points for the first ten moves for 
the two pawns in front of the king to stay put. I then gave a bonus to major 
pieces besides the queen when the leave their home row.



Performance

In terms of performance I feel that my program performed fairly well. I had to 
work out some issues here and there when implementing alpha beta pruning but 
after implementing iterative deepening on top of a thoroughly tested alpha beta 
negamax search I saw a significant increase in how well my player did on Imcs. 
This led my program to win against the tacklebot 95% of the time with a 5% draw 
rate. I have played other players on Imcs and have done well against other 
players with transposition tables. This has led my program DeepBlue to a second 
place rating on Imcs. During semi-finals my program didn’t lose but was lead 
toward draw a lot. Since then I have fixed an issue that didn’t let me optimize 
my code via the flag –O3. The optimization boost has caused my current search 
depth to be around an average of 10 ply in the beginning and 12 ply midgame. 
As of now there are no known bugs present in my program, I still occasionally 
sacrifice pieces so I could do more testing to see if I can find a more 
efficient way to do this. I suppose this is because the computer can see where 
it leads to is a desired state but it wouldn’t hurt to look into it more. 



Testing and Evaluation

When building my project my first series of test came after creating my move 
generator. I used the provided move gen test cases to make sure that all my 
produced moves were legal. I also created a random player to help test these 
moves and then I built a function that allowed me to play a game against it so 
I can monitor real time. During the process of building negamax and alpha beta 
I separated the negamax into a single function and then ab/negamax into a 
separate function that way I could use the test functions discussed above to 
make sure that they both produce the same move at the same depth. The same 
process was followed for iterative deepening although I did not create any 
functions for it to play against my ab/negamax player. I just used the Imcs 
server play to help monitor my time as I played games.



Future Plans

I intend to implement the transposition table next. This will allow 
me to store and quickly access duplicate states to speed my search. I also 
intend to find a way to speed up my program to improve my depth. I am currently 
generating moves, where I then make that move temporarily in order to evaluate 
the state of the board to store the evaluation score of that particular move. 
I then undo the move and continue the pattern. This means that I am searching 
the entire board and evaluating both sides for every single possible legal 
move for as deep as time will let me go. This leads me to believe my bottleneck 
may be within move generation so if I can improve this aspect of my program I 
should get a significant boost in performance. 
