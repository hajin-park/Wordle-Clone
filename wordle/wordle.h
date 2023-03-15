#ifndef WORDLE_H
#define WORDLE_H
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <random>

using namespace std;

struct Stats {
    int gameState, timesPlayed, averageAttempts, winPercentage, currentStreak, longestStreak;
    vector<string> Words, Wins;
    vector<int> Attempts;
};

void readStatsFile(Stats& stats) {
    ifstream file("stats.txt");
    if (file.is_open()) {
        stats.Words.clear(), stats.Attempts.clear(), stats.Wins.clear();
        file >> stats.gameState >> stats.timesPlayed >> stats.averageAttempts >> stats.winPercentage >> stats.currentStreak >> stats.longestStreak;
        string word, win, line;
        int attempt;
        getline(file, line); // Skip the first line of stats.txt
        while (getline(file, line)) {
            stats.Words.push_back(line.substr(0, line.find(' ')));
            line.erase(0, line.find(' ')+1);
            stats.Attempts.push_back(stoi(line.substr(0, line.find(' '))));
            line.erase(0, line.find(' ')+1);
            stats.Wins.push_back(line.substr(0, line.size()));
        }
        stats.gameState = 1;
        file.close();
    } else cerr << "Error: Could not open file." << endl;
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
    vector<string> guesses;         // {"poops", "peeps", "examp"}
    vector<vector<string>> colors;  // {{GREEN, BLACK, BLACK, BLACK, BLACK},{{GREEN, BLACK, BLACK, BLACK, BLACK}},{{GREEN, BLACK, BLACK, BLACK, BLACK}}}
    string guess_lower; // Lower
    string guess_upper;
    string answer_lower = getRandomWord();
    string answer_upper = answer_lower;
    string temp_answer;
    for (int i = 0; i < answer_upper.size(); i++) {
        answer_upper[i] = toupper(answer_upper[i]);
    }
    string gameState = "active";

    while (gameState == "active") {
        vector<string> color;
        string line;
        bool valid_guess = false;
        while (!valid_guess) {
            ifstream file("allowed.txt");
            ifstream file2("words.txt");
            cout << "Enter guess: ";
            cin >> guess_lower;
            while (getline(file, line)) {
                if ((line.find(guess_lower) != string::npos && guess_lower.size() == 5) || guess_lower == answer_lower) {
                    valid_guess = true;
                    file.close();
                    break;
                }
            }
            while (getline(file2, line)) {
                if ((line.find(guess_lower) != string::npos && guess_lower.size() == 5) || guess_lower == answer_lower) {
                    valid_guess = true;
                    file.close();
                    break;
                }
            }
            file.close();
        }

        guess_upper = guess_lower;
        for (int i = 0; i < guess_upper.size(); i++) {
            guess_upper[i] = toupper(guess_upper[i]);
        }

        guesses.push_back(guess_upper);
        temp_answer = answer_upper;

        for (int i = 0; i < guess_upper.size(); i++) {
            size_t index = temp_answer.find(guess_upper[i]);

            if (index != string::npos) {
                if (index == i) {
                    color.push_back(GREEN);
                } else {
                    color.push_back(YELLOW);
                }
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
            black_keys = line;
            getline(keyboard_file, line);
            yellow_keys = line;
            getline(keyboard_file, line);
            green_keys = line;

        } else {
            cerr << "Error: File could not be opened" << endl;
        }  
        keyboard_file.close();

        vector<string> color_rows = {black_keys, yellow_keys, green_keys};

        ofstream keyboard_file2("keyboard.txt", ios::trunc);
        if (keyboard_file2.is_open()) {
            for (int i = 0; i < 3; i++) {
                keyboard_file2 << color_rows[i];
                for (int j = 0; j < colors.back().size(); j++) {
                    if (i == 0 && colors.back()[j] == BLACK) {
                        keyboard_file2 << guess_upper[j];
                    } else if (i == 1 && colors.back()[j] == YELLOW) {
                        keyboard_file2 << guess_upper[j];
                    } else if (i == 2 && colors.back()[j] == GREEN) {
                        keyboard_file2 << guess_upper[j];
                    }
                }
                keyboard_file2 << '\n';
            }
        } else {
            cerr << "Error: File could not be opened" << endl;
        }
        keyboard_file2.close();

        if (guess_upper == answer_upper) {
            gameState = "win";
            printGameScreen(guesses, answer_upper, gameState, colors);
            break;
        } else if (guesses.size() == 6) {
            gameState = "lose";
            printGameScreen(guesses, answer_upper, gameState, colors);
            break;
        } else printGameScreen(guesses, answer_upper, gameState, colors);
    }

    stats.Words.push_back(answer_upper);
    stats.Attempts.push_back(guesses.size());
    stats.Wins.push_back((gameState == "win") ? "Yes" : "No");

    cin.ignore();
    while (!gameState.empty()) {
        getline(cin, gameState);
    }
}

#endif