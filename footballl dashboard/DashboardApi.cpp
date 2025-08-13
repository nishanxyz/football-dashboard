#include "DashboardApi.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

DashboardData DashboardApi::fetchUpcomingMatches(const std::string& competitionCode) {
    std::string url = "https://api.football-data.org/v4/competitions/" + competitionCode + "/matches?status=SCHEDULED&limit=5";

    cpr::Response r = cpr::Get(
        cpr::Url{ url },
        cpr::Header{ {"X-Auth-Token", apiKey} }
    );

    json j;
    try {
        j = json::parse(r.text);
    }
    catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return DashboardData();
    }

    if (!j.contains("matches") || !j["matches"].is_array()) {
        std::cerr << "Unexpected JSON structure.\n";
        return DashboardData();
    }

    DashboardData data;
    int count = 0;
    for (const auto& match : j["matches"]) {
        if (count >= 5) break;

        MatchInfo m;

        if (match.contains("homeTeam") && match["homeTeam"].contains("name")) {
            m.homeTeam = match["homeTeam"]["name"].get<std::string>();
        }
        else {
            m.homeTeam = "Unknown";
        }

        if (match.contains("awayTeam") && match["awayTeam"].contains("name")) {
            m.awayTeam = match["awayTeam"]["name"].get<std::string>();
        }
        else {
            m.awayTeam = "Unknown";
        }

        m.status = match.value("status", "SCHEDULED");
        m.homeScore = 0;
        m.awayScore = 0;

        if (match.contains("utcDate")) {
            m.matchDate = match["utcDate"].get<std::string>();
        }

        data.matches.push_back(m);
        ++count;
    }

    return data;
}

DashboardData DashboardApi::fetchLiveMatches() {
    std::string url = "https://api.football-data.org/v4/matches?status=LIVE";

    cpr::Response r = cpr::Get(
        cpr::Url{ url },
        cpr::Header{ {"X-Auth-Token", apiKey} }
    );

    json j;
    try {
        j = json::parse(r.text);
    }
    catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return DashboardData();
    }

    if (!j.contains("matches") || !j["matches"].is_array()) {
        return DashboardData();
    }

    DashboardData data;
    int count = 0;
    for (const auto& match : j["matches"]) {
        if (count >= 5) break;

        MatchInfo m;

        if (match.contains("homeTeam") && match["homeTeam"].contains("name")) {
            m.homeTeam = match["homeTeam"]["name"].get<std::string>();
        }
        else {
            m.homeTeam = "Unknown";
        }

        if (match.contains("awayTeam") && match["awayTeam"].contains("name")) {
            m.awayTeam = match["awayTeam"]["name"].get<std::string>();
        }
        else {
            m.awayTeam = "Unknown";
        }

        m.status = match.value("status", "LIVE");
        m.homeScore = 0;
        m.awayScore = 0;

        if (match.contains("score") && match["score"].contains("fullTime")) {
            const auto& fullTime = match["score"]["fullTime"];

            if (fullTime.contains("home") && !fullTime["home"].is_null()) {
                m.homeScore = fullTime["home"].get<int>();
            }

            if (fullTime.contains("away") && !fullTime["away"].is_null()) {
                m.awayScore = fullTime["away"].get<int>();
            }
        }

        data.matches.push_back(m);
        ++count;
    }

    return data;
}

DashboardData DashboardApi::fetchRecentResults() {
    std::string url = "https://api.football-data.org/v4/matches";

    cpr::Response r = cpr::Get(
        cpr::Url{ url },
        cpr::Header{ {"X-Auth-Token", apiKey} }
    );

    json j;
    try {
        j = json::parse(r.text);
    }
    catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return DashboardData();
    }

    if (!j.contains("matches") || !j["matches"].is_array()) {
        return DashboardData();
    }

    DashboardData data;
    int count = 0;
    for (const auto& match : j["matches"]) {
        if (count >= 5) break;

        MatchInfo m;

        if (match.contains("homeTeam") && match["homeTeam"].contains("name")) {
            m.homeTeam = match["homeTeam"]["name"].get<std::string>();
        }
        else {
            m.homeTeam = "Unknown";
        }

        if (match.contains("awayTeam") && match["awayTeam"].contains("name")) {
            m.awayTeam = match["awayTeam"]["name"].get<std::string>();
        }
        else {
            m.awayTeam = "Unknown";
        }

        m.status = match.value("status", "FINISHED");
        m.homeScore = 0;
        m.awayScore = 0;

        if (match.contains("score") && match["score"].contains("fullTime")) {
            const auto& fullTime = match["score"]["fullTime"];

            if (fullTime.contains("home") && !fullTime["home"].is_null()) {
                m.homeScore = fullTime["home"].get<int>();
            }

            if (fullTime.contains("away") && !fullTime["away"].is_null()) {
                m.awayScore = fullTime["away"].get<int>();
            }
        }

        data.matches.push_back(m);
        ++count;
    }

    return data;
}



DashboardData DashboardApi::fetchStandings(const std::string& competitionCode) {
    std::string url = "https://api.football-data.org/v4/competitions/" + competitionCode + "/standings";

    cpr::Response r = cpr::Get(
        cpr::Url{ url },
        cpr::Header{ {"X-Auth-Token", apiKey} }
    );

    json j;
    try {
        j = json::parse(r.text);
    }
    catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return DashboardData();
    }

    if (!j.contains("standings") || !j["standings"].is_array() || j["standings"].empty()) {
        std::cerr << "No standings data found.\n";
        return DashboardData();
    }

    DashboardData data;

    // Get the first standings table (usually the overall table)
    const auto& standingsTable = j["standings"][0];

    if (!standingsTable.contains("table") || !standingsTable["table"].is_array()) {
        std::cerr << "Invalid standings table structure.\n";
        return DashboardData();
    }

    for (const auto& standing : standingsTable["table"]) {
        StandingInfo s;

        s.position = standing.value("position", 0);
        s.points = standing.value("points", 0);
        s.playedGames = standing.value("playedGames", 0);
        s.won = standing.value("won", 0);
        s.draw = standing.value("draw", 0);
        s.lost = standing.value("lost", 0);
        s.goalsFor = standing.value("goalsFor", 0);
        s.goalsAgainst = standing.value("goalsAgainst", 0);

        if (standing.contains("team") && standing["team"].contains("name")) {
            s.teamName = standing["team"]["name"].get<std::string>();
        }
        else {
            s.teamName = "Unknown Team";
        }

        data.standings.push_back(s);
    }

    return data;
}

// Keep the original method for backward compatibility
DashboardData DashboardApi::fetchMatches(const std::string& competitionCode) {
    return fetchUpcomingMatches(competitionCode);
}
