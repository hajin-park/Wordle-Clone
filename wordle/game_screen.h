#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

#include <iostream>
#include <vector>

using namespace std;

void printTop(string COLOR) { cout << COLOR << " --- " << RESET; }
void printMiddle(char c, string COLOR) { cout << COLOR << "| " << c << " |" << RESET; }
void printBottom(string COLOR) { cout << COLOR << " --- " << RESET; }
void printGameScreen(vector<string> guesses, string answer, string gameState, vector<vector<string>> colors) {
    system("clear");

    for (int i = 0; i < guesses.size(); i++) {
        for (int j = 0; j < 5; j++) {
            printTop(colors[i][j]);
        }
        cout << endl;
        for (int j = 0; j < 5; j++) {
            printMiddle(guesses[i][j], colors[i][j]);
        }
        cout << endl;
        for (int j = 0; j < 5; j++) {
            printBottom(colors[i][j]);
        }
        cout << endl;
    }
    cout << endl << endl;
    if (gameState == "win") {
        cout << "Splendid!" << endl << endl << endl;
        cout << "Press [enter] to continue" << endl;
    } else if (gameState == "lose") {
        cout << "The word was: " << answer << endl << endl << endl;
        cout << "Better luck next time!" << endl << endl << endl;
        cout << "Press [enter] to continue" << endl;
    }
}

#endif