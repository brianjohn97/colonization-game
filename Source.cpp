#include <iostream>
#include <pthread.h>
#include <vector>
#include <fstream>

using namespace std;

//global variables
int teamOnePlayers = 0;
int teamTwoPlayers = 0;
int row = 10;
int col = 10;
vector<vector<int>> map(row, vector<int>(col));

void saveMap(const char* filename){
    //open the file in binary mode
    ofstream file(filename, ios::binary);

    //write the map data to the file
    for(const auto& width : map){
        file.write(reinterpret_cast<const char*> (&width[0]), col * sizeof(int));
    }

    //close the file
    file.close();
}
void loadMap(const char* filename){
    //open the file in binary mode
    ifstream file(filename, ios::binary);

    //read the map data from the file
    for(auto& width : map){
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

    //check if they inputted a 0 for either team and exit
    if(teamTwoPlayers == 0 || teamOnePlayers == 0){
        cout << "\nThere needs to be at least 1 player on each team to play this game.\n";
        cout << "Try again with an appropriate amount of players for the game.\n\n";
        exit(0);
    }
}

int main(int argc, char * argv[]) {
    //getPlayersAndBoard(argc, argv);

    map[3][5] = 1;
    map[7][2] = 2;

    saveMap("map.bin");

    map.clear();


    map.resize(row, vector<int>(col));

    loadMap("map.bin");

    for(const auto& width : map){
        for (int value : width){
            cout << value << " ";
        }
        cout << endl;
    }


    return 0;
}