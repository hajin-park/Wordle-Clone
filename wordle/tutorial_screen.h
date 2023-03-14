#ifndef TUTORIAL_SCREEN_H
#define TUTORIAL_SCREEN_H
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

#include <iostream>

using namespace std;

void printTutorialScreen() {
    cout << "=============================================" << endl;
    cout << "                 HOW TO PLAY" << endl;
    cout << "=============================================" << endl;
    cout << "Guess the Wordle in 6 tries." << endl << endl << endl;
    cout << "HOW TO PLAY:" << endl;
    cout << "- Each guess must be a valid 5 letter word." << endl;
    cout << "- The color of the tiles will change to show" << endl;
    cout << "  you how close your guess was to the word." << endl << endl << endl;
    cout << GREEN << " --- " << RESET << " ---  ---  ---  ---" << endl;
    cout << GREEN << "| W |" << RESET << "| E || A || R || Y |" << endl;
    cout << GREEN << " --- " << RESET << " ---  ---  ---  ---" << endl;
    cout << "W is in the word and in the correct spot." << endl << endl;
    cout << " --- " << YELLOW << " --- " << RESET << " ---  ---  ---" << endl;
    cout << "| P |" << YELLOW << "| I |" << RESET << "| L || L || S |" << endl;
    cout << " --- " << YELLOW << " --- " << RESET << " ---  ---  ---" << endl;
    cout << "I is in the word but in the wrong spot." << endl << endl;
    cout << " ---  ---  --- " << BLACK << " --- " << RESET << " ---" << endl;
    cout << "| V || A || G |" << BLACK << "| U |" << RESET << "| E |" << endl;
    cout << " ---  ---  --- " << BLACK << " --- " << RESET << " ---" << endl;
    cout << "U is not in the word in any spot." << endl << endl << endl;
    cout << "Press [enter] to continue" << endl;
}

#endif