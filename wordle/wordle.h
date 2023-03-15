#ifndef WORDLE_H
#define WORDLE_H
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <random>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

struct Stats {
    int gameState, timesPlayed, averageAttempts, winPercentage, currentStreak, longestStreak;
    vector<string> Words, Wins;
    vector<int> Attempts;
};

void readStatsFile(Stats& stats) {
    ifstream file("stats.txt");
    if (file) {
        stats.Words.clear();
        stats.Attempts.clear();
        stats.Wins.clear();
        file >> stats.gameState
             >> stats.timesPlayed
             >> stats.averageAttempts
             >> stats.winPercentage
             >> stats.currentStreak
             >> stats.longestStreak;
        std::string line;
        std::getline(file, line); // Skip the first line of stats.txt
        while (std::getline(file, line)) {
            istringstream iss(line);
            string word, win;
            int attempt;
            iss >> word >> attempt >> win;
            stats.Words.push_back(word);
            stats.Attempts.push_back(attempt);
            stats.Wins.push_back(win);
        }
        stats.gameState = 1;
    } else {
        std::cerr << "Error: Could not open file.\n";
    }
}

void updateStatsFile(Stats& stats) {
    ofstream file("stats.txt", ios::trunc);
    if (file.is_open()) {
        int numWins = 0, longestStreak = 0, currentStreak = 0, attemptSum = 0;
        for (string win : stats.Wins) {
            if (win == "Yes") {
                numWins++, currentStreak++;
                if (currentStreak > longestStreak) longestStreak = currentStreak;
            } else currentStreak = 0;
        }
        for (int i : stats.Attempts) attemptSum += i;
        stats.timesPlayed = stats.Words.size();
        stats.averageAttempts = stats.timesPlayed ? attemptSum/stats.Attempts.size() : 0;
        stats.winPercentage = stats.timesPlayed ? ((float)numWins/stats.Wins.size())*100 : 0;
        stats.currentStreak = currentStreak, stats.longestStreak = longestStreak;
        file << stats.gameState << ' ' << stats.timesPlayed << ' ' << stats.averageAttempts << ' ' << stats.winPercentage << ' ' << stats.currentStreak << ' ' << stats.longestStreak << '\n';
        for (int i = 0; i < stats.Words.size(); i++) {
            file << stats.Words[i] << ' ' << stats.Attempts[i] << ' ' << stats.Wins[i] << '\n';
        }
        file.close();
    } else cerr << "Error: Could not open file." << endl;
}

void resetStatsFile() {
    ofstream file("stats.txt", ios::trunc);
    if (file.is_open()) {
        file << "1 0 0 0 0 0 0";
        file.close();
    } else cerr << "Error: Could not open file." << endl;
}

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
        vector<string> color;
        while (!valid_guess) {
            ifstream file("allowed.txt");
            ifstream file2("words.txt");
            cout << "Enter guess: ";
            cin >> guess_lower;
            while (getline(file, line) || getline(file2, line)) {
                if ((line.find(guess_lower) != string::npos && guess_lower.size() == 5) || guess_lower == answer_lower) {
                    valid_guess = true;
                    break;
                }
            }
            file.close();
            file2.close();
        }

        string guess_upper = guess_lower;
        transform(guess_upper.begin(), guess_upper.end(), guess_upper.begin(), ::toupper);
        guesses.push_back(guess_upper);
        string temp_answer = answer_upper;

        for (int i = 0; i < guess_upper.size(); i++) {
            size_t index = temp_answer.find(guess_upper[i]);
            if (index != string::npos) {
                color.push_back((index == i) ? GREEN : YELLOW);
                temp_answer[index] = ' ';
            } else {
                color.push_back(BLACK);

            }
        }
        colors.push_back(color);

        string black_keys;
        string yellow_keys;
        string green_keys;

        ifstream keyboard_file("keyboard.txt");
        if (keyboard_file.is_open()) {
            string line;
            getline(keyboard_file, line);
            black_keys = line + string(5, ' ');
            getline(keyboard_file, line);
            yellow_keys = line + string(5, ' ');
            getline(keyboard_file, line);
            green_keys = line + string(5, ' ');
        } else {
            cerr << "Error: File could not be opened" << endl;
        }  
        keyboard_file.close();

        ofstream keyboard_file2("keyboard.txt", ios::trunc);
        if (keyboard_file2.is_open()) {
            for (int i = 0; i < 5; i++) {
                if (colors.back()[i] == BLACK) {
                    black_keys[black_keys.size()-5+i] = guess_upper[i];
                } else if (colors.back()[i] == YELLOW) {
                    yellow_keys[yellow_keys.size()-5+i] = guess_upper[i];
                } else if (colors.back()[i] == GREEN) {
                    green_keys[green_keys.size()-5+i] = guess_upper[i];
                }
            }
            keyboard_file2 << black_keys << '\n' << yellow_keys << '\n' << green_keys;
        } else {
            cerr << "Error: File could not be opened" << endl;
        }
        keyboard_file2.close();

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