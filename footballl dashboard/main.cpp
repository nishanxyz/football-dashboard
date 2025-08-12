#include "DashboardApi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <exception>
#include <limits>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN "cls"
#else
#include <cstdlib>
#define CLEAR_SCREEN "clear"
#endif

class InteractiveDashboard {
private:
    DashboardApi api;
    std::map<std::string, std::string> leagues = {
        {"1", "BSA"},  // Campeonato Brasileiro Série A
        {"2", "PL"},   // Premier League
        {"3", "ELC"},  // Championship
        {"4", "CL"},   // UEFA Champions League
        {"5", "EC"},   // European Championship
        {"6", "FL1"},  // Ligue 1
        {"7", "BL1"},  // Bundesliga
        {"8", "SA"}    // Serie A
    };

    std::map<std::string, std::string> leagueNames = {
        {"BSA", "Campeonato Brasileiro Série A"},
        {"PL", "Premier League"},
        {"ELC", "Championship"},
        {"CL", "UEFA Champions League"},
        {"EC", "European Championship"},
        {"FL1", "Ligue 1"},
        {"BL1", "Bundesliga"},
        {"SA", "Serie A"}
    };

public:
    InteractiveDashboard(const std::string& apiKey) : api(apiKey) {}

    void clearScreen() {
        system(CLEAR_SCREEN);
    }

    void printHeader(const std::string& title) {
        std::cout << "+" << std::string(78, '=') << "+\n";
        std::cout << "|" << std::setw(40 + title.length() / 2) << title
            << std::setw(40 - title.length() / 2) << "|\n";
        std::cout << "+" << std::string(78, '=') << "+\n\n";
    }

    void printBox(const std::string& content, int width = 76) {
        std::cout << "+" << std::string(width, '-') << "+\n";
        std::cout << "| " << std::left << std::setw(width - 2) << content << "|\n";
        std::cout << "+" << std::string(width, '-') << "+\n";
    }

    void printMatchTable(const std::vector<MatchInfo>& matches, const std::string& tableTitle) {
        std::cout << "\n+" << std::string(78, '=') << "+\n";
        std::cout << "|" << std::setw(40 + tableTitle.length() / 2) << tableTitle
            << std::setw(40 - tableTitle.length() / 2) << "|\n";
        std::cout << "+" << std::string(78, '=') << "+\n";

        if (matches.empty()) {
            std::cout << "|" << std::setw(39) << "No matches available"
                << std::setw(39) << "|\n";
        }
        else {
            std::cout << "| " << std::left << std::setw(25) << "Home Team"
                << "| " << std::setw(5) << "Score"
                << "| " << std::setw(25) << "Away Team"
                << "| " << std::setw(12) << "Status" << " |\n";
            std::cout << "+" << std::string(26, '-') << "+" << std::string(6, '-')
                << "+" << std::string(26, '-') << "+" << std::string(13, '-') << "+\n";

            int count = 0;
            for (const auto& match : matches) {
                if (count >= 5) break;

                std::string scoreStr;
                if (match.status == "FINISHED" || match.status == "LIVE") {
                    scoreStr = std::to_string(match.homeScore) + " - " + std::to_string(match.awayScore);
                }
                else {
                    scoreStr = "vs";
                }

                std::cout << "| " << std::left << std::setw(25) << match.homeTeam.substr(0, 24)
                    << "| " << std::setw(5) << scoreStr
                    << "| " << std::setw(25) << match.awayTeam.substr(0, 24)
                    << "| " << std::setw(12) << match.status.substr(0, 11) << " |\n";
                ++count;
            }
        }

        std::cout << "+" << std::string(78, '=') << "+\n";
    }

    void showMainMenu() {
        printHeader("*** FOOTBALL DASHBOARD ***");

        std::cout << "+----------------------------------------------------------------------------+\n";
        std::cout << "|                              MAIN MENU                                     |\n";
        std::cout << "+----------------------------------------------------------------------------+\n";
        std::cout << "|  1. [LIVE] Live Matches                                                   |\n";
        std::cout << "|  2. [UPCOMING] Upcoming Matches (Choose League)                          |\n";
        std::cout << "|  3. [RESULTS] Recent Match Results                                        |\n";
        std::cout << "|  4. [EXIT] Exit                                                           |\n";
        std::cout << "+----------------------------------------------------------------------------+\n";
        std::cout << "\n>> Enter your choice (1-4): ";
    }

    void showLeagueMenu() {
        printHeader("*** SELECT LEAGUE FOR UPCOMING MATCHES ***");

        std::cout << "+----------------------------------------------------------------------------+\n";
        std::cout << "|                            AVAILABLE LEAGUES                               |\n";
        std::cout << "+----------------------------------------------------------------------------+\n";
        std::cout << "|  1. [BRA] Campeonato Brasileiro Serie A                                   |\n";
        std::cout << "|  2. [ENG] Premier League                                                  |\n";
        std::cout << "|  3. [ENG] Championship                                                    |\n";
        std::cout << "|  4. [EUR] UEFA Champions League                                           |\n";
        std::cout << "|  5. [EUR] European Championship                                           |\n";
        std::cout << "|  6. [FRA] Ligue 1                                                         |\n";
        std::cout << "|  7. [GER] Bundesliga                                                      |\n";
        std::cout << "|  8. [ITA] Serie A                                                         |\n";
        std::cout << "|  9. [<--] Back to Main Menu                                               |\n";
        std::cout << "+----------------------------------------------------------------------------+\n";
        std::cout << "\n>> Enter your choice (1-9): ";
    }

    int getValidInput(int min, int max) {
        int choice;
        while (true) {
            if (std::cin >> choice && choice >= min && choice <= max) {
                std::cin.ignore(10000, '\n');
                return choice;
            }
            else {
                std::cout << "X Invalid input! Please enter a number between "
                    << min << " and " << max << ": ";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        }
    }

    void waitForUser() {
        std::cout << "\n>> Press Enter to continue...";
        std::cin.ignore(10000, '\n');
    }

    void showLiveMatches() {
        clearScreen();
        printHeader("*** LIVE MATCHES ***");

        std::cout << ">> Fetching live matches...\n";

        try {
            auto data = api.fetchLiveMatches();
            printMatchTable(data.matches, "LIVE MATCHES");

            if (data.matches.empty()) {
                printBox("INFO: No live matches at the moment. Check back later!");
            }

        }
        catch (const std::exception& e) {
            printBox("ERROR: Error fetching live matches: " + std::string(e.what()));
        }

        waitForUser();
    }

    void showUpcomingMatches(const std::string& leagueCode) {
        clearScreen();
        std::string leagueName = leagueNames[leagueCode];
        printHeader("*** UPCOMING MATCHES - " + leagueName + " ***");

        std::cout << ">> Fetching upcoming matches for " << leagueName << "...\n";

        try {
            auto data = api.fetchUpcomingMatches(leagueCode);
            printMatchTable(data.matches, "UPCOMING MATCHES");

            if (data.matches.empty()) {
                printBox("INFO: No upcoming matches scheduled for this league.");
            }

        }
        catch (const std::exception& e) {
            printBox("ERROR: Error fetching upcoming matches: " + std::string(e.what()));
        }

        waitForUser();
    }

    void showRecentResults() {
        clearScreen();
        printHeader("*** RECENT MATCH RESULTS ***");

        std::cout << ">> Fetching recent match results...\n";

        try {
            auto data = api.fetchRecentResults();
            printMatchTable(data.matches, "RECENT RESULTS");

            if (data.matches.empty()) {
                printBox("INFO: No recent results available.");
            }

        }
        catch (const std::exception& e) {
            printBox("ERROR: Error fetching recent results: " + std::string(e.what()));
        }

        waitForUser();
    }

    void run() {
        while (true) {
            clearScreen();
            showMainMenu();

            int choice = getValidInput(1, 4);

            switch (choice) {
            case 1:
                showLiveMatches();
                break;

            case 2: {
                while (true) {
                    clearScreen();
                    showLeagueMenu();

                    int leagueChoice = getValidInput(1, 9);

                    if (leagueChoice == 9) {
                        break; // Back to main menu
                    }

                    std::string leagueCode = leagues[std::to_string(leagueChoice)];
                    showUpcomingMatches(leagueCode);
                }
                break;
            }

            case 3:
                showRecentResults();
                break;

            case 4:
                clearScreen();
                printHeader("*** GOODBYE! ***");
                std::cout << "Thank you for using Football Dashboard!\n\n";
                return;
            }
        }
    }
};

int main() {
    std::string apiKey = "fe728bc5476c4cae90140b7399cd3005";

    try {
        InteractiveDashboard dashboard(apiKey);
        dashboard.run();
    }
    catch (const std::exception& e) {
        std::cerr << "X Critical error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}