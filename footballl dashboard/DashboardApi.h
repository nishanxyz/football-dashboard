#ifndef DASHBOARD_API_H
#define DASHBOARD_API_H

#include <string>
#include <vector>

struct MatchInfo {
    std::string homeTeam;
    std::string awayTeam;
    int homeScore = 0;
    int awayScore = 0;
    std::string status;
    std::string matchDate;
};

struct DashboardData {
    std::vector<MatchInfo> matches;
};

class DashboardApi {
private:
    std::string apiKey;

public:
    explicit DashboardApi(const std::string& key) : apiKey(key) {}

    // Get upcoming matches for a specific league
    DashboardData fetchUpcomingMatches(const std::string& competitionCode);

    // Get live matches
    DashboardData fetchLiveMatches();

    // Get recent match results
    DashboardData fetchRecentResults();

    // Original method (can be kept for backward compatibility)
    DashboardData fetchMatches(const std::string& competitionCode);
};

#endif