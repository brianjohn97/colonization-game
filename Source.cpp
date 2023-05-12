#include <iostream>
#include <pthread.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <string.h>
#include <mutex>
#include <atomic>

using namespace std;

//global variables
int teamOnePlayers = 2;
int teamTwoPlayers = 2;

int row = 25;
int col = 25;
vector<int> teamOne;
vector<int> teamTwo;
vector<vector<string>> board(row, vector<string>(col));
string winner = "0";
vector<int> places;
bool gameIsOver = false;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
int thePlayer = 1;


//checks all squares next to the fired missile spot
//and deals with it accordingly
//takes in the x and y coordinate plus the team number
void checkAdjacentSquares(int x, int y, int curPlayer){
    string team;
    string otherTeam;
    int t1 = 0;
    int t2 = 0;
    if(curPlayer == 1){team = "T1"; otherTeam = "T2";}
    else{ team = "T2"; otherTeam = "T1"; }


    //checks to see if the same team already has that location and to relieve it if they do
    if(board[x][y] == team){
        cout << "Landed on space already occupied by "<<team<<"! So you have lost this space!\n\n";
        board[x][y] = "0";
        return;
    }

    //notify the players that this player has taken over this spot from the enemy
    if(board[x][y] == otherTeam){
        cout << "You have landed on space that was occupied by the enemies and have taken it for your own!\n";
    }

    //place piece on the board incrementing their score and the t1 tally
    board[x][y] = team; t1++;

    //check the left
    if(y > 0){
        if(board[x][y-1] == team){t1++;}else if(board[x][y-1] == otherTeam){t2++;}
    }
    //check the right
    if(y < col-1){
        if(board[x][y+1] == team){t1++;}else if(board[x][y+1] == otherTeam){t2++;}
    }
    //check above
    if(x > 0){
        if(board[x-1][y] == team){t1++;}else if(board[x-1][y] == otherTeam){t2++;}
    }
    //check below
    if(x < row-1){
        if(board[x+1][y] == team){t1++;}else if(board[x+1][y] == otherTeam){t2++;}
    }
    //check above to the left
    if(x > 0 && y > 0){
        if(board[x-1][y-1] == team){t1++;}else if(board[x-1][y-1] == otherTeam){t2++;}
    }
    //check above to the right
    if(x > 0 && y < (col-1)){
        if(board[x-1][y+1] == team){t1++;}else if(board[x-1][y+1] == otherTeam){t2++;}
    }
    //check below and to the left
    if(x < (row-1) && y > 0){
        if(board[x+1][y-1] == team){t1++;}else if(board[x+1][y-1] == otherTeam){t2++;}
    }
    //check below and to the right
    if(x < (row - 1) && y < (col - 1)){
        if(board[x+1][y+1] == team){t1++;}else if(board[x+1][y+1] == otherTeam){t2++;}
    }

    int temp = 0;
    if(t2 == 0){ return; }
    if(t1 > t2){
        //place all pieces that it can depending on location
        cout << "The current team has the majority surrounding [" << x << "][" << y <<"] so it will take all adjacent squares with it!\n";
        //check left
        if(y > 0){
            if(board[x][y-1] == otherTeam || board[x][y-1] == "0"){board[x][y-1] = team;}

        }//check right
        if(y < col-1){
            if(board[x][y+1] == otherTeam || board[x][y+1] == "0" ){board[x][y+1] = team;}

        }//check above
        if(x > 0){
            if(board[x-1][y] == otherTeam || board[x-1][y] == "0"){board[x-1][y] = team;}

        }//check below
        if(x < row-1){
            if(board[x+1][y] == otherTeam || board[x+1][y] == "0"){board[x+1][y] = team;}

        }//above to the left
        if(x > 0 && y > 0){
            if(board[x-1][y-1] == otherTeam || board[x-1][y-1] == "0"){board[x-1][y-1] = team;}

        }//above to the right
        if(x > 0 && y < (col-1)){
            if(board[x-1][y+1] == otherTeam || board[x-1][y+1] == "0"){board[x-1][y+1] = team;}

        }//below to the left
        if(x < (row-1) && y > 0){
            if(board[x+1][y-1] == otherTeam || board[x+1][y-1] == "0") {board[x+1][y-1] = team;}

        }//below to the right
        if(x < (row - 1) && y < (col - 1)){
            if(board[x+1][y+1] == otherTeam || board[x+1][y+1] == "0") {board[x+1][y+1] = team;}

        }
    }
}

//puts all the players into two team vectors to make it easier who is
//on what team
void split(){
    for (int i = 1; i <= (teamOnePlayers+teamTwoPlayers); i++){
        if(i <= teamOnePlayers){
            teamOne.push_back(i);
            continue;
        }
        teamTwo.push_back(i);
    } 
}

//prints the current state of the board
void printBoard(){
    //print the numbers for the columns
    for (int i = 0; i < row; i++){
            if(i == 0){
                cout << "  " <<  i << "\t"; 
                continue;   
            }
            cout << "  " <<  i << "\t";
    }
    cout << endl;

    //print out elements 
    for(int i = 0; i< row;i++){
        for (int j=0;j<col;j++){
            if(board[i][j] != "0"){
                cout << "["  << board[i][j] << " ]\t";
                continue;
            }
            cout << "[ "  << board[i][j] << " ]\t";
        }
        cout << endl;
    }
    cout << endl;
}

//when the code is run with parameters this will see how many parameters there
//and print out which it's missing with examples
void printWhatsMissing(int argc){
    //check if they typed anything in, print what they need and exit
    if (argc < 2) {
        cout << "\nYou are missing everything!\n";
        cout << "You need 4 arguments: \n";
        cout << "The amount of players on team One\n";
        cout << "The amount of players on team Two\n";
        cout << "The amount of rows for the board\n";
        cout << "The amount of columns for the board\n";
        cout << "In that order. Should look something like: ./demo 2 3 10 20 \n\n";
        exit(0);

    //print out they are missing row, column and team player 2 sizes and exit
    } else if (argc < 3) {
        cout << "\nYou are missing the row and column sizes.";
        cout << "\nYou are also missing the amount of players on the second team. \n";
        cout << "Please re run the game with the size of the rows and the columns for the board"
                " and the amount of players on Team 2.\n\n";
        exit(0);

    //print out they are missing the row and column size and exit
    } else if (argc < 4) {
        cout << "\nYou are missing the row and column size of the board.\n";
        cout << "Please re run the game with the size of the rows and the columns for the board\n\n";
        exit(0);

    //print out they are missing the column size and exit
    } else if (argc < 5) {
        cout << "\nYou are missing the column size of the board.\n";
        cout << "Please re run the game with the size of the columns for the board\n\n";
        exit(0);

    //print out they have too many arguments and exit
    } else if (argc > 5) {
        cout << "\nThere are too many arguments \n";
        cout << "There should only be 4 included arguments:\n";
        cout << "The amount of players on team One\n";
        cout << "The amount of players on team Two\n";
        cout << "The amount of rows for the board\n";
        cout << "The amount of columns for the board\n";
        cout << "In that order. Should look something like: ./demo 2 3 10 20 \n\n";
        exit(0);
    }
}

//This function will use a try catch to convert the command line arguments int variables
//will also make check to make sure the amount of players per board sie makes sense
void getPlayersAndBoard(int  argc, char **argv) {

    //check if their missing an argument
    printWhatsMissing(argc);

    //convert what they inputted to an int and throw exception error if not integer
    try {
        teamOnePlayers = stoi(argv[1]);
        teamTwoPlayers = stoi(argv[2]);
        row = stoi(argv[3]);
        col = stoi(argv[4]);
    }catch (exception &err){
        cout << "\nInputted parameters were incorrect. please try again\n";
        cout << "Input only integers \n\n";
        exit(0);
    }

    //check if there are too many players for the board and exit
    if((teamTwoPlayers + teamTwoPlayers) > (row * col)){
        cout << "\nThere are too many players for the specified board size!\n";
        cout << "Try again with an appropriate amount of players for the board"
                "\nor increase the board size if possible.\n\n";
        exit(0);
    }

    if(row < 2 || col < 2){
        cout << "The Board size is too small to play!\n";
        cout << "Please increase the board size and try again!\n\n";
        exit(0);
    }

    //check if they inputted a 0 for either team and exit
    if(teamTwoPlayers <= 0 || teamOnePlayers <= 0){
        cout << "\nThere needs to be at least 1 player on each team to play this game.\n";
        cout << "Try again with an appropriate amount of players for the game.\n\n";
        exit(0);
    }
    if((teamOnePlayers + teamTwoPlayers) > 20){
        cout << "\nThere is a max total player count of 20!\n";
        cout << "\nPlease try again with a total player count of 20 or less! Thank you!\n\n";
        exit(0);
    }
    if(row > 10 || col > 10){
        cout << "\nThe board is too big! you would be waiting a long time for the game if you go above 10!\n";
        cout << "\nPlease try again with a more appropriate game board size!\n\n";
        exit(0);
    }
}

//takes in the player and their x and y coordinate
void placingPlayers(int player, int x, int y){

    //takes the current player int and places a string of which player it is
    //on the board
    switch (player)
    {
    case 1:
        board[x][y] = "P1";
        break;
    case 2:
        board[x][y] = "P2";
        break;
    case 3:
        board[x][y] = "P3";
        break;
    case 4:
        board[x][y] = "P4";
        break;
    case 5:
        board[x][y] = "P5";
        break;
    case 6:
        board[x][y] = "P6";
        break;
    case 7:
        board[x][y] = "P7";
        break;
    case 8:
        board[x][y] = "P8";
        break;
    case 9:
        board[x][y] = "P9";
        break;
    case 10:
        board[x][y] = "P10";
        break;
    case 11:
        board[x][y] = "P11";
        break;
    case 12:
        board[x][y] = "P12";
        break;
    case 13:
        board[x][y] = "P13";
        break;
    case 14:    
        board[x][y] = "P14";
        break;
    case 15:
        board[x][y] = "P15";
        break;
    case 16:
        board[x][y] = "P16";
        break;
    case 17:
        board[x][y] = "P17";
        break;
    case 18:
        board[x][y] = "P18";
        break;
    case 19:
        board[x][y] = "P19";
        break;
    case 20:
        board[x][y] = "P20";
        break;  
    default:
        break;
    }
}

//starts the game with rules and starting positions
void printStart(){
    
    //print the goal of the game
    cout << "\nThe goal of the colonization game is to" 
            "\nconquer the rectangular map from the opposite team!\n"
            "\nOnce the board is full, we will count how many players on each team there are to determine the winner!\n"
            "These are the starting positions for all the players.\n\n";

    //loops through all the players getting random ints for the coordinates
    //checking if that spot is already taken. then placing the player and putting the
    //coordinates into a vector
    for (int i = 1; i <= (teamOnePlayers + teamTwoPlayers); i++){
        bool flag = false;
        int x = rand() % row;
        int y = rand() % col;
        for (int j = 0; j < places.size(); j++){
            if(x == places[j] && y == places[j+1]){
                i--;
                flag = true;
                break;
            }
            j++;
        }
        if (flag){continue;}
        placingPlayers(i, x, y);
        places.push_back(x);
        places.push_back(y);
    }

    //print the starting board then put the players onto seperate vector teams then print
    //which player is on what team
    printBoard();
    cout << endl;
    split();
    cout << "Team 1: Players: ";
    for (int i = 0; i < teamOne.size(); i++){
        cout << teamOne[i] <<" ";
    }
    cout << endl;
    cout << "Team 2: Players: ";
    for (int i = 0; i < teamTwo.size(); i++){
        cout << teamTwo[i] << " ";
    }
    cout << endl << endl;
}

//called from player thread with coordinates and the player threadID
//loops through the players calling a function that checks all the squares around the spot the missile fired
//to see how many of those places are occupied
void missile(int player, int x, int y){

    for(int i=0;i < (teamOnePlayers + teamTwoPlayers); i++){
        if(player == teamOne[i]){
            checkAdjacentSquares(x, y, 1);
            break;
        }
        if (player == teamTwo[i]){
            checkAdjacentSquares(x, y, 2);
            break;
        }
    }
}

//player thread function. loop until the supervisor says the game is over getting random ints
//checks if a player is already there, locks the thread calls missile function to place the piece then unlocks
//sleeps to simulate loading each missile
void * player(void * arg){
    int threadID = (int)(long)arg;

    while (!gameIsOver){

        bool flag = false;
        int x = rand() % row;
        int y = rand() % col;
        int ranSleep = rand() % 3;

        for (int j = 0; j < places.size(); j++){
            if(x == places[j] && y == places[j+1]){
                flag = true;
                if(gameIsOver){break;}
                cout << "Fired missile could not land there because a soldier is already in that spot! will roll again!\n\n";
                break;
            }
            j++;
        }
        if (flag){ continue; }

        //for when it's not their turn.
        while(thePlayer != threadID);
        if(gameIsOver){break;}

        cout << "P"<<threadID << " launched a missile to coordinate ["<<x<<"]["<<y<<"]!\n\n";

        pthread_mutex_lock(&myMutex);
        missile(threadID, x, y);
        printBoard();
        sleep(ranSleep);
        pthread_mutex_unlock(&myMutex);

        //keeps the current player variable
        if(thePlayer == (teamOnePlayers+teamTwoPlayers)){
            thePlayer = 1;
            continue;
        }
        thePlayer++;
    }
    return (void*)0;
}

//checks if the game is over then figure out who the winner is
//then print the winner and end the program
void * supervisor(void * arg) {

    int team1Score = 0;
    int team2Score = 0;
    team1Score += teamOnePlayers;
    team2Score += teamTwoPlayers;

    //will check if the game has ended
    while(!gameIsOver){

        int counter = 0;

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if(board[i][j] == "0"){
                    counter++;
                }
            }
        }
        pthread_mutex_lock(&myMutex);
        if (counter == 0){
            gameIsOver = true;
            for (int i=0; i<row;i++) {
                for (int j = 0; j < col; j++) {

                    if (board[i][j] == "T1") { team1Score++; }
                    if (board[i][j] == "T2") { team2Score++; }

                }
            }
            break;
        }
        pthread_mutex_unlock(&myMutex);
    }

    //get winner
    if(team1Score > team2Score){
        winner = "1";
    }else if (team2Score > team1Score){
        winner = "2";
    }else{winner = "3";}


    //pronounce winner
    if(winner == "1"){
        cout << "\nTeam 1 has won the game! If you would would to retry restart the game with the same parameters\n";
    }else if(winner == "2"){
        cout << "\nTeam 2 has won the game! If you would would to retry restart the game with the same parameters\n";
    }else if(winner == "3"){
        cout << "\nUnfortunately there was no winner! Theres also no losers! It was a tie!\n"
                "If you would like to retry, then restart the game with the same parameters. \n";
    }
    exit(0);
}

int main(int argc, char * argv[]) {

    //creates the random seed
    srand(static_cast<unsigned int>(time(0)));

    //gets the players and board sizes as command line arguments
    getPlayersAndBoard(argc, argv);

    int totalPlayers = teamOnePlayers + teamTwoPlayers;

    //create board
    vector<string> temp;

    for(int i=0;i < row;i++){
        for (int j = 0; j < col; j++){
            board[i][j] = "0";
        }
        
    }

    //puts all the players onto the board randomly/prints the rules of the game
    //puts the location of the player pieces into a vector
    printStart();
    cout << endl;

    //creating the threads
    pthread_t supThread;
    pthread_t myThreads[totalPlayers];

    pthread_create(&supThread, 0, supervisor, NULL);
    for (int i = 1; i <= totalPlayers; i++){
        pthread_create(&myThreads[i], 0, player, (void *)(long)i);
    }
    for (int i = 1; i < totalPlayers; i++){
        pthread_join(myThreads[i], NULL);
    }
    return 0;
}
