#ifndef WORDLE_H
#define WORDLE_H

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <cmath>

struct Wordle {
    // Game history statistics
    int gameState, timesPlayed, averageAttempts, winPercentage, currentStreak, longestStreak;
    std::vector<std::string> Words, Wins;
    std::vector<int> Attempts;

    // Custom Wordle length
    std::string length, wordsFile, allowedFile;

    Wordle() {
        readStatsFile();
        length = 5;
        wordsFile = "5-words.txt";
        allowedFile = "5-allowed.txt";
    }

    Wordle(std::string length, std::string words, std::string allowed) {
        readStatsFile();
        length = std::stoi(length);
        wordsFile = wordsFile;
        allowedFile = allowedFile;
    }

    // File manipulation methods
    void readStatsFile() {
        std::ifstream file("../../data/stats.txt");
        if (file) {
            Words.clear();
            Attempts.clear();
            Wins.clear();
            file >> gameState
                >> timesPlayed
                >> averageAttempts
                >> winPercentage
                >> currentStreak
                >> longestStreak;
            std::string line;
            getline(file, line); // Skip the first line of stats.txt
            while (getline(file, line)) {
                std::istringstream iss(line);
                std::string word, win;
                int attempt;
                iss >> word >> attempt >> win;
                Words.push_back(word);
                Attempts.push_back(attempt);
                Wins.push_back(win);
            }
            gameState = 1;
        } else std::cerr << "Error: Could not open file.\n";
    }

    void updateStatsFile() {
        std::ofstream file("../../data/stats.txt", std::ios::trunc);
        if (file.is_open()) {
            int numWins = 0, longestStreak = 0, currentStreak = 0, attemptSum = 0;
            for (std::string win : Wins) {
                if (win == "Yes") {
                    numWins++, currentStreak++;
                    if (currentStreak > longestStreak) longestStreak = currentStreak;
                } else currentStreak = 0;
            }
            for (int i : Attempts) attemptSum += i;
            timesPlayed = Words.size();
            averageAttempts = timesPlayed ? attemptSum/Attempts.size() : 0;
            winPercentage = timesPlayed ? ((float)numWins/Wins.size())*100 : 0;
            currentStreak = currentStreak, longestStreak = longestStreak;
            file << gameState << ' ' << timesPlayed << ' ' << averageAttempts << ' ' << winPercentage << ' ' << currentStreak << ' ' << longestStreak << '\n';
            for (int i = 0; i < Words.size(); i++) {
                file << Words[i] << ' ' << Attempts[i] << ' ' << Wins[i] << '\n';
            }
            file.close();
        } else std::cerr << "Error: Could not open file." << std::endl;
    }

    void resetStatsFile() {
        std::ofstream file("../../data/stats.txt", std::ios::trunc);
        if (file.is_open()) {
            file << "1 0 0 0 0 0 0";
            file.close();
        } else std::cerr << "Error: Could not open file." << std::endl;
    }

    void updateColorsFile(std::vector<std::vector<std::string>> colors, std::string guess_upper) {
        std::string black_keys;
        std::string yellow_keys;
        std::string green_keys;

        std::ifstream colors_file_in("../../data/colors.txt");

        // Keep track of previous keyboard colors
        if (colors_file_in.is_open()) {
            std::string line;
            getline(colors_file_in, line);
            black_keys = line + std::string(length, ' ');
            getline(colors_file_in, line);
            yellow_keys = line + std::string(length, ' ');
            getline(colors_file_in, line);
            green_keys = line + std::string(length, ' ');
        } else std::cerr << "Error: File could not be opened" << std::endl;
        colors_file_in.close();

        std::ofstream colors_file_out("../../data/colors.txt", std::ios::trunc);
        if (colors_file_out.is_open()) {

            // Add new colors to keyboard color rows
            for (int i = 0; i < length; i++) {
                if (colors.back()[i] == BLACK) {
                    black_keys[black_keys.size()-length+i] = guess_upper[i];
                } else if (colors.back()[i] == YELLOW) {
                    yellow_keys[yellow_keys.size()-length+i] = guess_upper[i];
                } else if (colors.back()[i] == GREEN) {
                    green_keys[green_keys.size()-length+i] = guess_upper[i];
                }
            }
            colors_file_out << black_keys << '\n' << yellow_keys << '\n' << green_keys;
        } else std::cerr << "Error: File could not be opened" << std::endl;
        colors_file_out.close();
    }

    // Console printing methods
    void printMenu() {
        std::cout << "=========================" << std::endl;
        std::cout << "    WELCOME TO WORDLE" << std::endl;
        std::cout << "=========================" << std::endl << std::endl << std::endl;
        std::cout << "1. Play Wordle" << std::endl;
        std::cout << "2. How to Play" << std::endl;
        std::cout << "3. Statistics Summary" << std::endl;
        std::cout << "4. Reset Statistics" << std::endl;
        std::cout << "5. Exit" << std::endl << std::endl << std::endl;
        std::cout << "Select an option:" << std::endl;
    }

    void printGame(std::vector<std::string> guesses, std::string answer, std::string gameState, std::vector<std::vector<std::string>> colors) {
        system("clear");
        for (int i = 0; i < guesses.size(); i++) {
            for (int j = 0; j < 5; j++) std::cout << colors[i][j] << " --- " << RESET;
            std::cout << std::endl;
            for (int j = 0; j < 5; j++) std::cout << colors[i][j] << "| " << guesses[i][j] << " |" << RESET;
            std::cout << std::endl;
            for (int j = 0; j < 5; j++) std::cout << colors[i][j] << " --- " << RESET;
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
        if (gameState == "win") {
            std::cout << "Splendid!" << std::endl << std::endl << std::endl;
            std::cout << "Press [enter] to continue" << std::endl;
        } else if (gameState == "lose") {
            std::cout << "The word was: " << answer << std::endl << std::endl << std::endl;
            std::cout << "Better luck next time!" << std::endl << std::endl << std::endl;
            std::cout << "Press [enter] to continue" << std::endl;
        }
    }

    void printTutorial() {
        std::cout << "=============================================" << std::endl;
        std::cout << "                 HOW TO PLAY" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << "Guess the Wordle in 6 tries." << std::endl << std::endl << std::endl;
        std::cout << "HOW TO PLAY:" << std::endl;
        std::cout << "- Each guess must be a valid 5 letter word." << std::endl;
        std::cout << "- The color of the tiles will change to show" << std::endl;
        std::cout << "  you how close your guess was to the word." << std::endl << std::endl << std::endl;
        std::cout << GREEN << " --- " << RESET << " ---  ---  ---  ---" << std::endl;
        std::cout << GREEN << "| W |" << RESET << "| E || A || R || Y |" << std::endl;
        std::cout << GREEN << " --- " << RESET << " ---  ---  ---  ---" << std::endl;
        std::cout << "W is in the word and in the correct spot." << std::endl << std::endl;
        std::cout << " --- " << YELLOW << " --- " << RESET << " ---  ---  ---" << std::endl;
        std::cout << "| P |" << YELLOW << "| I |" << RESET << "| L || L || S |" << std::endl;
        std::cout << " --- " << YELLOW << " --- " << RESET << " ---  ---  ---" << std::endl;
        std::cout << "I is in the word but in the wrong spot." << std::endl << std::endl;
        std::cout << " ---  ---  --- " << BLACK << " --- " << RESET << " ---" << std::endl;
        std::cout << "| V || A || G |" << BLACK << "| U |" << RESET << "| E |" << std::endl;
        std::cout << " ---  ---  --- " << BLACK << " --- " << RESET << " ---" << std::endl;
        std::cout << "U is not in the word in any spot." << std::endl << std::endl << std::endl;
        std::cout << "Press [enter] to continue" << std::endl;
    }

    void printStats() {
        std::cout << "==========================" << std::endl;
        std::cout << "    STATISTICS SUMMARY" << std::endl;
        std::cout << "==========================" << std::endl;
        std::cout << "Times Played:" << std::setw(13) << std::right << timesPlayed << std::endl;
        std::cout << "Average Attempts:" << std::setw(9) << std::right << averageAttempts << std::endl;
        std::cout << "Win Percentage:" << std::setw(10) << std::right << winPercentage << '%' << std::endl;
        std::cout << "Current Streak:" << std::setw(11) << std::right << currentStreak << std::endl;
        std::cout << "Longest Streak:" << std::setw(11) << std::right << longestStreak << std::endl << std::endl << std::endl;
        std::cout << "--------------------------" << std::endl;
        std::cout << "WORD     ATTEMPTS      WIN" << std::endl;
        std::cout << "--------------------------" << std::endl;
        for (int i = 0; i < Words.size(); i++) {
            std::cout << Words[i] << std::setw(12) << std::right << Attempts[i] << std::setw(9) << std::right << Wins[i] << std::endl;
        }
        std::cout << std::endl << std::endl;
        std::cout << "Press [enter] to continue" << std::endl;
    }

    // Game logic methods
    std::string getRandomWord() {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, 2315); // define the range
        int number = distr(gen);
        std::string answer_lower;
        std::ifstream file("../../data/words.txt");
        for (int i = 0; i < number; i++) getline(file, answer_lower);
        file.close();
        return answer_lower; 
    }

    std::vector<std::string> matchColors(std::string guess_upper, std::string answer_upper) {
        std::vector<std::string> color = {BLACK, BLACK, BLACK, BLACK, BLACK};

        // 10 iterations - check guess input word twice
        for (int i = 0; i < 10; i++) {
            size_t index = answer_upper.find(guess_upper[(i % 5)]);

            // First five iterations
            if (floor((float)i/float(5)) == 0) {

                // Letter is an exact match
                if (index != std::string::npos && index == (i % 5)) {
                    color[index] = GREEN;
                    answer_upper[index] = ' ';
                }
            
            // Last five iterations
            } else {
                if (index != std::string::npos) {

                    // Letter is an indirect match
                    color[i % 5] = YELLOW;
                    answer_upper[index] = ' ';
                }
            }
        }
        return color;
    }

    void play() {
        std::vector<std::string> guesses;
        std::vector<std::vector<std::string>> colors;
        std::string gameState = "active";
        bool valid_guess = false;
        std::string answer = getRandomWord();
        std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);

        while (gameState == "active") {
            bool valid_guess = false;
            std::string guess;
            std::string line;

            while (!valid_guess) {

                // Check if input guess is a valid n-letter word
                std::ifstream allowed_words_file("allowed.txt");
                std::ifstream answer_words_file("words.txt");
                std::cout << "Enter guess: ";
                std::cin >> guess;
                while (getline(allowed_words_file, line) || getline(answer_words_file, line)) {
                    if (line.find(guess) != std::string::npos && guess.size() == 5) {
                        valid_guess = true;
                        break;
                    }
                }
                allowed_words_file.close();
                answer_words_file.close();
            }

            // Assign colors to letters according to Wordle logic
            std::transform(guess.begin(), guess.end(), guess.begin(), ::toupper);
            guesses.push_back(guess);
            colors.push_back(matchColors(guess, answer));
            updateColorsFile(colors, guess);

            // Check game-ending conditions according to Wordle logic
            if (guess == answer) gameState = "win";
            else if (guesses.size() == 6) gameState = "lose";

            printGame(guesses, answer, gameState, colors);
        }

        // Update game history stats
        Words.push_back(answer);
        Attempts.push_back(guesses.size());
        Wins.push_back((gameState == "win") ? "Yes" : "No");

        std::cin.ignore();
        while (!gameState.empty()) getline(std::cin, gameState);   
    }
};

#endif