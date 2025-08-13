#ifndef DASHBOARDAPI_H
#define DASHBOARDAPI_H

#include <string>
#include <vector>

struct MatchInfo {
    std::string homeTeam;
    std::string awayTeam;
    int homeScore;
    int awayScore;
    std::string status;
    std::string matchDate;
};

struct StandingInfo {
    int position;
    std::string teamName;
    int playedGames;
    int won;
    int draw;
    int lost;
    int goalsFor;
    int goalsAgainst;
    int points;
};

struct DashboardData {
    std::vector<MatchInfo> matches;
    std::vector<StandingInfo> standings;
};

class DashboardApi {
private:
    std::string apiKey;

public:
    DashboardApi(const std::string& key) : apiKey(key) {}

    // Fetch upcoming matches for a specific competition
    DashboardData fetchUpcomingMatches(const std::string& competitionCode);

    // Fetch live matches across all competitions
    DashboardData fetchLiveMatches();

    // Fetch recent results (all competitions)
    DashboardData fetchRecentResults();

    // Fetch standings for a specific competition
    DashboardData fetchStandings(const std::string& competitionCode);

    // Legacy method for backward compatibility
    DashboardData fetchMatches(const std::string& competitionCode);
};

#endif
