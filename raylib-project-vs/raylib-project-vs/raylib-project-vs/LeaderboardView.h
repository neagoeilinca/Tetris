#pragma once

#include <string>
#include <utility>
#include <vector>

#include "PlayerManager.h"

class LeaderboardView
{
private:
    PlayerManager& playerManager;
    std::vector<std::pair<std::string, int>> leaderboard;

    void loadLeaderboard();

public:
    explicit LeaderboardView(PlayerManager& pm);

    void refresh();
    void draw() const;
};