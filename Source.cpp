#include <iostream>
#include <pthread.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <cstring>

using namespace std;

//global variables
int teamOnePlayers = 2;
int teamTwoPlayers = 2;
int row = 5;
int col = 5;
vector<vector<string>> board;
string winner = "0";
vector<int> places;

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
        //if(i > 9){cout << i << " ";}
        //else{cout << i << "  ";}
        for (int j=0;j<col;j++){
            if(board[i][j] != "0"){
                cout << "["  << board[i][j] << " ]\t";
                continue;
            }
            cout << "[ "  << board[i][j] << " ]\t";
        }
        cout << endl;
    }
}
void saveMap(const char* filename){
    //open the file in binary mode
    ofstream file(filename, ios::binary);

    //write the map data to the file
    for(const auto& width : board){
        file.write(reinterpret_cast<const char*> (&width[0]), col * sizeof(int));
    }

    //close the file
    file.close();
}
void loadMap(const char* filename){
    //open the file in binary mode
    ifstream file(filename, ios::binary);

    //read the map data from the file
    for(auto& width : board){
        file.read(reinterpret_cast<char*> (&width[0]), col * sizeof(int));
    }
}
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

    if(row <= 2 || col <= 2){
        cout << "The Board size is too small to play!\n";
        exit(0);
    }

    //check if they inputted a 0 for either team and exit
    if(teamTwoPlayers <= 0 || teamOnePlayers <= 0){
        cout << "\nThere needs to be at least 1 player on each team to play this game.\n";
        cout << "Try again with an appropriate amount of players for the game.\n\n";
        exit(0);
    }
}
void updateBoard(){

    saveMap("board.bin");
    board.clear();
    board.resize(row, vector<string>(col));
    loadMap("board.bin");
    for(const auto& width : board){
        for (string value : width){
            cout << value << " ";
        }
        cout << endl;
    }
}
void placingPlayers(int player, int x, int y){
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
void printStart(){
    
    srand(static_cast<unsigned int>(time(0)));
    cout << "\nThe goal of the colonization game is to" 
            "\nconquer the rectangular map from the opposite team!\n"
            "These are the starting positions for all the players.\n\n";
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
        if (flag == true){continue;}
        placingPlayers(i, x, y);
        places.push_back(x);
        places.push_back(y);
    }
    printBoard();
    cout << endl;
}
void missile(){

}
void * player(void * threadID){
    
    return (void*)0;
}
void * supervisor(void * ThreadID){
    //will check if the game has ended
    
    //send signal to the players that the game is over and to stop shooting missiles

    //pronounce winner
    if(winner == "1"){
        cout << "\nTeam 1 has won the game! If you would would to retry restart the game with the same parameters\n";
    }else if(winner == "2"){
        cout << "\nTeam 2 has won the game! If you would would to retry restart the game with the same parameters\n";
    }
    return (void*)0;
}

int main(int argc, char * argv[]) {

    //getPlayersAndBoard(argc, argv);
    
    //create board
    vector<string> temp;

    for(int i=0;i < row;i++){
        for (int j = 0; j < col; j++){
            temp.push_back({"0"});
        }
        board.push_back(temp);
    }
    
    printStart();

    
    
    /*
    //pthread_t myThreads[10];

    /*
    for (int i = 1; i <= teamOnePlayers + teamTwoPlayers; i++){
        int rval = pthread_create(&myThreads[i], NULL, player, (void *)(long)i);    
    }*/
    
    

    board.clear();
    return 0;
}
