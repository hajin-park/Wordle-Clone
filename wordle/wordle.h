#ifndef WORDLE_H
#define WORDLE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <cmath>
#include "game_screen.h"

using namespace std;


string getRandomWord() {
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(0, 2315); // define the range
    int number = distr(gen);
    string answer_lower;
    ifstream file("words.txt");
    for (int i = 0; i < number; i++) getline(file, answer_lower);
    file.close();
    return answer_lower;
}

void startGame(Stats& stats) {
    vector<string> guesses;
    vector<vector<string>> colors;
    string guess_lower;
    string answer_lower = getRandomWord();
    string answer_upper = answer_lower;
    transform(answer_upper.begin(), answer_upper.end(), answer_upper.begin(), ::toupper);
    string gameState = "active";

    while (gameState == "active") {
        bool valid_guess = false;
        string line;
        while (!valid_guess) {
            ifstream allowed_words_file("allowed.txt");
            ifstream answer_words_file("words.txt");
            cout << "Enter guess: ";
            cin >> guess_lower;
            while (getline(allowed_words_file, line) || getline(answer_words_file, line)) {
                if ((line.find(guess_lower) != string::npos && guess_lower.size() == 5) || guess_lower == answer_lower) {
                    valid_guess = true;
                    break;
                }
            }
            allowed_words_file.close();
            answer_words_file.close();
        }

        string guess_upper = guess_lower;
        transform(guess_upper.begin(), guess_upper.end(), guess_upper.begin(), ::toupper);
        guesses.push_back(guess_upper);
        string temp_answer = answer_upper;
        vector<string> color = {BLACK, BLACK, BLACK, BLACK, BLACK};

        for (int i = 0; i < 10; i++) {
            size_t index = temp_answer.find(guess_upper[(i % 5)]);
            if (floor((float)i/float(5)) == 0) {
                if (index != string::npos && index == (i % 5)) {
                    color[index] = GREEN;
                    temp_answer[index] = ' ';
                }
            } else {
                if (index != string::npos) {
                    color[i % 5] = YELLOW;
                    temp_answer[index] = ' ';
                }
            }
        }
        colors.push_back(color);

        string black_keys;
        string yellow_keys;
        string green_keys;

        ifstream keyboard_file_in("keyboard.txt");
        if (keyboard_file_in.is_open()) {
            string line;
            getline(keyboard_file_in, line);
            black_keys = line + string(5, ' ');
            getline(keyboard_file_in, line);
            yellow_keys = line + string(5, ' ');
            getline(keyboard_file_in, line);
            green_keys = line + string(5, ' ');
        } else {
            cerr << "Error: File could not be opened" << endl;
        }  
        keyboard_file_in.close();

        ofstream keyboard_file_out("keyboard.txt", ios::trunc);
        if (keyboard_file_out.is_open()) {
            for (int i = 0; i < 5; i++) {
                if (colors.back()[i] == BLACK) {
                    black_keys[black_keys.size()-5+i] = guess_upper[i];
                } else if (colors.back()[i] == YELLOW) {
                    yellow_keys[yellow_keys.size()-5+i] = guess_upper[i];
                } else if (colors.back()[i] == GREEN) {
                    green_keys[green_keys.size()-5+i] = guess_upper[i];
                }
            }
            keyboard_file_out << black_keys << '\n' << yellow_keys << '\n' << green_keys;
        } else {
            cerr << "Error: File could not be opened" << endl;
        }
        keyboard_file_out.close();

        if (guess_upper == answer_upper) {
            gameState = "win";
        } else if (guesses.size() == 6) {
            gameState = "lose";
        }

        printGameScreen(guesses, answer_upper, gameState, colors);
    }

    stats.Words.push_back(answer_upper);
    stats.Attempts.push_back(guesses.size());
    stats.Wins.push_back((gameState == "win") ? "Yes" : "No");

    cin.ignore();
    while (!gameState.empty()) getline(cin, gameState);
}

#endif