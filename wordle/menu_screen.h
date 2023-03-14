#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <iostream>

using namespace std;

void printMenuScreen() {
    cout << "=========================" << endl;
    cout << "    WELCOME TO WORDLE" << endl;
    cout << "=========================" << endl << endl << endl;
    cout << "1. Play Wordle" << endl;
    cout << "2. How to Play" << endl;
    cout << "3. Statistics Summary" << endl;
    cout << "4. Reset Statistics" << endl;
    cout << "5. Exit" << endl << endl << endl;
    cout << "Select an option:" << endl;
}

#endif