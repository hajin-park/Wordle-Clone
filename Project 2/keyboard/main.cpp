#include <iostream>
#include "keyboard.h"

using namespace std;

int main(int argc, char* argv[]) {
    Keyboard keyboard;
    string line;

    while (true) {
        ifstream file("../data/stats.txt");
        if (file.is_open()) {
            file >> keyboard.gameState;

            // wordle app is not running
            if (keyboard.gameState == 0) {
                if (keyboard.prevGameState != keyboard.gameState) {
                    system("clear");
                    cout << "Please launch the Wordle game" << endl;
                    ofstream file2("../data/colors.txt", ios::trunc);
                    file2.close();
                    keyboard.prevGameState = keyboard.gameState;
                    keyboard.prevColors = {{RESET}};
                }

            // Wordle app is running, not in game
            } else if (keyboard.gameState == 1) {
                if (keyboard.prevGameState != keyboard.gameState) {
                    system("clear");
                    cout << "Waiting for Wordle game round to start" << endl;
                    ofstream file2("../data/colors.txt", ios::trunc);
                    file2.close();
                    keyboard.prevGameState = keyboard.gameState;
                    keyboard.prevColors = {{RESET}};
                }

            // Wordle app is currently playing a game
            } else {
                keyboard.prevGameState = keyboard.gameState;
                keyboard.colors.clear();
                for (vector<char> row : keyboard.letters) {
                    keyboard.colors.push_back({});
                    for (char c : row) {
                        ifstream file2("../data/colors.txt");
                        keyboard.colors.back().push_back(keyboard.colorOptions[3]);
                        if (file.is_open()) {
                            for (int i = 0; i < 3; i++) {
                                getline(file2, line);
                                if (line.find(c) != string::npos) keyboard.colors.back().back() = keyboard.colorOptions[i];
                            }
                        } else cerr << "Error: File could not be opened" << endl;
                    }
                }
                if (keyboard.prevColors != keyboard.colors) {
                    system("clear");
                    keyboard.printKeyboard();
                    keyboard.prevColors = keyboard.colors;
                }
            }
        } else cerr << "Error: File could not be opened" << endl;
        file.close();
    }

    return 0;
}