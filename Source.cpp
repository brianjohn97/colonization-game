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

int row = 4;
int col = 4;
vector<int> teamOne;
vector<int> teamTwo;
vector<vector<string>> board(row, vector<string>(col));
string winner = "0";
vector<int> places;
bool gameIsOver = false;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
int thePlayer = 1;
int teamOneScore = 0;
int teamTwoScore = 0;
int totalTurns = 0;


void split(){
    for (int i = 1; i <= (teamOnePlayers+teamTwoPlayers); i++){
        if(i <= teamOnePlayers){
            teamOne.push_back(i);
            continue;
        }
        teamTwo.push_back(i);
    } 
}
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
    cout << endl;
}
void saveMap(const char* filename){
    //open the file in binary mode
    //ofstream file(filename, ios::binary);

    //write the map data to the file
    //for(const auto& x: board){
        //file.write(reinterpret_cast<const char*> (&x[0]), row * sizeof(int));
    //}
    // ofstream files;
    // files.open(filename, ios::out | ios::trunc | ios::binary);
	// files.write((char*)&sizes, sizeof(sizes));
    // files.close();
    // files.open(filename, ios::out | ios::app | ios::binary);
    


    // for(size_t b =0;b<board.size(); b++){
    //     files.write((char*)&board[b][0], board[b].size()*sizeof(string));
    // }

    // //close the file
    // files.close();
}
void loadMap(const char* filename){
    //open the file in binary mode
    //ifstream file(filename, ios::binary);

    //read the map data from the file
    //for(auto& x : board){
        //file.read(reinterpret_cast<char*> (&x[0]), row * sizeof(int));
    //}
    // vector<vector<string>> numbers2;
    // int sizes2[2];
    // ifstream file1;
    // file1.open(filename, ios::in | ios::binary);
    // file1.read((char*)&sizes2, sizeof(sizes2));

    // string temp;
	// for (int x = 0; x < row; x++) {
	// 	numbers2.push_back(vector<string>());
	// 	for (int y = 0; y < col; y++) {
	// 		file1.read((char*)&temp, sizeof(string));
	// 		numbers2[x].push_back(temp);
	// 		cout << numbers2.at(x).at(y) << " ";
	// 	}
	// 	cout << endl;
    // }
    // file1.close();
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

    if(row < 2 || col < 2){
        cout << "The Board size is too small to play!\n";
        exit(0);
    }

    //check if they inputted a 0 for either team and exit
    if(teamTwoPlayers <= 0 || teamOnePlayers <= 0){
        cout << "\nThere needs to be at least 1 player on each team to play this game.\n";
        cout << "Try again with an appropriate amount of players for the game.\n\n";
        exit(0);
    }
    //totalTurns = row * col;
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
void missile(int player, int x, int y){
    int t1 = 0;
    int t2 = 0;
    for(int i=0;i < (teamOnePlayers + teamTwoPlayers); i++){
        if(player == teamOne[i]){
            if(board[x][y] == "T1"){teamOneScore--;board[x][y] = "0";break;}
            board[x][y] = "T1";
            teamOneScore++;
            t1++;

            for (int j = 0; j < 8; ++j) {
                //check the left
                if(y > 0){
                    if(board[x][y-1] == "T1"){t1++;}else if(board[x][y-1] == "T2"){t2++;}
                }
                //check the right
                if(y < col-1){
                    if(board[x][y+1] == "T1"){t1++;}else if(board[x][y+1] == "T2"){t2++;}
                }
                //check above
                if(x > 0){
                    if(board[x-1][y] == "T1"){t1++;}else if(board[x-1][y] == "T2"){t2++;}
                }
                //check below
                if(x < row-1){
                    if(board[x+1][y] == "T1"){t1++;}else if(board[x+1][y] == "T2"){t2++;}
                }
                //check above to the left
                if(x > 0 && col > 0){
                    if(board[x-1][y-1] == "T1"){t1++;}else if(board[x-1][y-1] == "T2"){t2++;}
                }
                //check above to the right
                if(x > 0 && y < col-1){
                    if(board[x-1][y+1] == "T1"){t1++;}else if(board[x-1][y+1] == "T2"){t2++;}
                }
                //check below and to the left
                if(x < (row-1) && y > 0){
                    if(board[x+1][y-1] == "T1"){t1++;}else if(board[x+1][y-1] == "T2"){t2++;}
                }
                //check below and to the right
                if(x < (row - 1) && y < (col - 1)){
                    if(board[x+1][y+1] == "T1"){t1++;}else if(board[x+1][y+1] == "T2"){t2++;}
                }
            }
            if(t2 == 0){break;}
            if(t1 > t2){
                //place all the pieces for team 1
                board[x+1][y+1] = "T1"; teamOneScore++;
                board[x+1][y-1] = "T1"; teamOneScore++;
                board[x-1][y+1] = "T1"; teamOneScore ++;
                board[x-1][y-1] = "T1"; teamOneScore++;
                board[x+1][y] = "T1"; teamOneScore++;
                board[x-1][y] = "T1"; teamOneScore++;
                board[x][y+1] = "T1"; teamOneScore++;
                board[x][y-1] = "T1"; teamOneScore++;
                break;
            }
            break;
        }
        if (player == teamTwo[i]){
            if(board[x][y] == "T2"){teamTwoScore--;board[x][y] = "0";break;}
            board[x][y] = "T2";
            teamTwoScore++;
            t2++;
            for (int j = 0; j < 8; ++j) {
                //check the left
                if(y > 0){
                    if(board[x][y-1] == "T2"){t2++;}else if(board[x][y-1] == "T1"){t1++;}
                }
                //check the right
                if(y < col-1){
                    if(board[x][y+1] == "T2"){t2++;}else if(board[x][y+1] == "T1"){t1++;}
                }
                //check above
                if(x > 0){
                    if(board[x-1][y] == "T2"){t2++;}else if(board[x-1][y] == "T1"){t1++;}
                }
                //check below
                if(x < row-1){
                    if(board[x+1][y] == "T2"){t2++;}else if(board[x+1][y] == "T1"){t1++;}
                }
                //check above to the left
                if(x > 0 && col > 0){
                    if(board[x-1][y-1] == "T2"){t2++;}else if(board[x-1][y-1] == "T1"){t1++;}
                }
                //check above to the right
                if(x > 0 && y < col-1){
                    if(board[x-1][y+1] == "T2"){t2++;}else if(board[x-1][y+1] == "T1"){t1++;}
                }
                //check below and to the left
                if(x < (row-1) && y > 0){
                    if(board[x+1][y-1] == "T2"){t2++;}else if(board[x+1][y-1] == "T1"){t1++;}
                }
                //check below and to the right
                if(x < (row - 1) && y < (col - 1)){
                    if(board[x+1][y+1] == "T2"){t2++;}else if(board[x+1][y+1] == "T1"){t1++;}
                }
            }
            if(t1 == 0){break;}
            if(t1 > t2){
                //place all the pieces for team 1
                board[x+1][y+1] = "T2"; teamTwoScore++;
                board[x+1][y-1] = "T2"; teamTwoScore++;
                board[x-1][y+1] = "T2"; teamTwoScore ++;
                board[x-1][y-1] = "T2"; teamTwoScore++;
                board[x+1][y] = "T2"; teamTwoScore++;
                board[x-1][y] = "T2"; teamTwoScore++;
                board[x][y+1] = "T2"; teamTwoScore++;
                board[x][y-1] = "T2"; teamTwoScore++;
                break;
            }
            break;
        }
    }
}
void * player(void * arg){
    int threadID = (int)(long)arg;


    while (!gameIsOver){
        while(thePlayer != threadID);
        bool flag = false;
        int x = rand() % row;
        int y = rand() % col;
        for (int j = 0; j < places.size(); j++){
            if(x == places[j] && y == places[j+1]){
                flag = true;
                break;
            }
            j++;
        }
        if (flag){
            continue;
        }
        cout << "P"<<threadID << " launched a missisle to coordinate"
        "["<<x<<"]["<<y<<"]!\n";
        pthread_mutex_lock(&myMutex);
        missile(threadID, x, y);
        printBoard();
        sleep(3);
        pthread_mutex_unlock(&myMutex);
        if(thePlayer == (teamOnePlayers+teamTwoPlayers)){
            thePlayer = 1;
            continue;
        }
        thePlayer++;

    }
    return (void*)0;
    
}
void * supervisor(void * arg){
    //will check if the game has ended
//    while(!gameIsOver){
//        if((teamOneScore + teamTwoPlayers) == totalTurns){
//            gameIsOver = true;
//        }
//    }
//
//    for (int i=0; i<row;i++){
//            for(int j=0;j<col;j++){
//                if(board[i][j] == "T1"){teamOneScore++;}
//                if(board[i][j] == "T2"){teamTwoScore++;}
//            }
//    }
//    if(teamOneScore > teamTwoScore){
//        winner = "1";
//    }else if (teamTwoScore > teamOneScore){
//        winner = "2";
//    }else{winner = "3";}

    //send signal to the players that the game is over and to stop shooting missiles

    //pronounce winner
//    if(winner == "1"){
//        cout << "\nTeam 1 has won the game! If you would would to retry restart the game with the same parameters\n";
//    }else if(winner == "2"){
//        cout << "\nTeam 2 has won the game! If you would would to retry restart the game with the same parameters\n";
//    }else if(winner == "3"){
//        cout << "\nUnfortunately there was no winner! Theres also no losers! It was a tie!\n"
//                "If you would like to retry, then restart the game with the same parameters. \n";
//    }
    return (void*)0;
}

int main(int argc, char * argv[]) {
    srand(static_cast<unsigned int>(time(0)));
    //getPlayersAndBoard(argc, argv);
    int totalPlayers = teamOnePlayers + teamTwoPlayers;


    //create board
    vector<string> temp;

    for(int i=0;i < row;i++){
        for (int j = 0; j < col; j++){
            board[i][j] = "0";
        }
        
    }

    printStart();

    cout << endl;

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
