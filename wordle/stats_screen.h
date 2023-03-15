#ifndef STATS_SCREEN_H
#define STATS_SCREEN_H

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

void printStatsScreen(
                int timesPlayed, 
                int averageAttempts, 
                int winPercentage, 
                int currentStreak, 
                int longestStreak, 
                vector<string> Words, 
                vector<int> Attempts, 
                vector<string> Wins) {
    cout << "==========================" << endl;
    cout << "    STATISTICS SUMMARY" << endl;
    cout << "==========================" << endl;
    cout << "Times Played:" << setw(13) << right << timesPlayed << endl;
    cout << "Average Attempts:" << setw(9) << right << averageAttempts << endl;
    cout << "Win Percentage:" << setw(10) << right << winPercentage << '%' << endl;
    cout << "Current Streak:" << setw(11) << right << currentStreak << endl;
    cout << "Longest Streak:" << setw(11) << right << longestStreak << endl << endl << endl;
    cout << "--------------------------" << endl;
    cout << "WORD     ATTEMPTS      WIN" << endl;
    cout << "--------------------------" << endl;
    for (int i = 0; i < Words.size(); i++) {
        cout << Words[i] << setw(12) << right << Attempts[i] << setw(9) << right << Wins[i] << endl;
    }
    cout << endl << endl;
    cout << "Press [enter] to continue" << endl;
}

#endif