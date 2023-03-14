#include <iostream>
#include <vector>
#include "wordle.h"
#include "menu_screen.h"
#include "stats_screen.h"
#include "tutorial_screen.h"

using namespace std;

int main(int argc, char* argv[]) {
    Stats stats;
    string user_option;
    
    system("clear");
    readStatsFile(stats);
    updateStatsFile(stats);
    printMenuScreen();
    
    while (cin >> user_option) {
        cin.ignore();

        if (user_option == "5") {
            stats.gameState = 0;
            updateStatsFile(stats);
            system("clear");
            break;

        } else if (user_option == "1") {
            system("clear");
            stats.gameState = 2;
            updateStatsFile(stats);
            startGame(stats);
            stats.gameState = 1;
            updateStatsFile(stats);

        } else if (user_option == "2") {
            while (!user_option.empty()) {
                system("clear");
                printTutorialScreen();
                getline(cin, user_option);
            }

        } else if (user_option == "3") {
            while (!user_option.empty()) {
                system("clear");
                printStatsScreen(
                    stats.timesPlayed, 
                    stats.averageAttempts, 
                    stats.winPercentage, 
                    stats.currentStreak, 
                    stats.longestStreak, 
                    stats.Words, 
                    stats.Attempts, 
                    stats.Wins);
                getline(cin, user_option);
            }

        } else if (user_option == "4") {
            resetStatsFile();
            readStatsFile(stats);
            while (!user_option.empty()) {
                system("clear");
                printStatsScreen(
                    stats.timesPlayed, 
                    stats.averageAttempts, 
                    stats.winPercentage, 
                    stats.currentStreak, 
                    stats.longestStreak, 
                    stats.Words, 
                    stats.Attempts, 
                    stats.Wins);
                getline(cin, user_option);
            }
        }

        system("clear");
        printMenuScreen();
    }

    return 0;
}