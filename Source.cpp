#include <iostream>

using namespace std;
void getPlayersAndBoard(int  argc, char **argv){
    if(argc < 2){
        cout << "\nYou are missing everything!\n";
        cout << "\nPlease enter the amount of players for each team and the rows and columns for the board! \n";
    }else if(argc < 3){
        cout << "\nYou are missing the row and column sizes.";
        cout << "\nYou are also missing the amount of players on the second team. \n\n";
    }else if(argc < 4){
        cout << "\nYou are missing the row and column size of the board.\n";
        cout << "Please re run the game with the size of the rows and the columns for the board\n\n";
    }else if(argc < 5){
        cout << "\nYou are missing the column size of the board.\n";
        cout << "Please re run the game with the size of the columns for the board\n\n";
    }
}
int main(int argc, char * argv[]) {
    getPlayersAndBoard(argc, argv);
    return 0;
}