#pragma once
#include "Player.h"
#include <vector>
#include <string>

class PlayerManager
{
private:
    std::string playersFilePath;
    std::map<std::string, json> players;

public:
    explicit PlayerManager(const std::string& filePath = "players.json");

    std::shared_ptr<Player> login(const std::string& name, const std::string& password);
    bool registerPlayer(const std::string& name, const std::string& password);
    void savePlayer(const std::shared_ptr<Player>& player);
    void loadAllPlayers();
    void saveAllPlayers();

    std::vector<std::pair<std::string, int>> getTopPlayers(int count = 10);
};