#include <iostream>
#include <fstream>
#include <vector>
#include "print.h"
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

using namespace std;

int main(int argc, char* argv[]) {
    int gameState = 0;
    int prev_gameState;
    string line;
    vector<string> colorOptions = {BLACK, YELLOW, GREEN, RESET};
    vector<vector<string>> colors;
    vector<vector<string>> prev_colors = {{RESET}};
    vector<vector<char>> keyboard = {{'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},{'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},{'Z', 'X', 'C', 'B', 'N', 'M', 'V'}};

    while (true) {
        ifstream file("../wordle/stats.txt");
        if (file.is_open()) {
            file >> gameState;
            if (gameState == 0) {
                if (prev_gameState != gameState) {
                    system("clear");
                    cout << "Please launch the Wordle game" << endl;
                    ofstream file2("../wordle/keyboard.txt", ios::trunc);
                    file2.close();
                    prev_gameState = gameState;
                    prev_colors = {{RESET}};
                }
            } else if (gameState == 1) {
                if (prev_gameState != gameState) {
                    system("clear");
                    cout << "Waiting for Wordle game round to start" << endl;
                    ofstream file2("../wordle/keyboard.txt", ios::trunc);
                    file2.close();
                    prev_gameState = gameState;
                    prev_colors = {{RESET}};
                }
            } else {
                prev_gameState = gameState;
                colors.clear();
                for (vector<char> row : keyboard) {
                    colors.push_back({});
                    for (char c : row) {
                        ifstream file2("../wordle/keyboard.txt");
                        colors.back().push_back(colorOptions[3]);
                        if (file.is_open()) {
                            for (int i = 0; i < 3; i++) {
                                getline(file2, line);
                                if (line.find(c) != string::npos) {
                                    colors.back().back() = colorOptions[i];
                                }
                            }
                        } else {
                            cerr << "Error: File could not be opened" << endl;
                        }
                    }
                }
                if (prev_colors != colors) {
                    system("clear");
                    printKeyboard(keyboard, colors);
                    prev_colors = colors;
                }
            }
        } else {
            cerr << "Error: File could not be opened" << endl;
        }
        file.close();
    }

    return 0;
}