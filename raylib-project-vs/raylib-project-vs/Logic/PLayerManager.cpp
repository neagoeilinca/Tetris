#include "pch.h"
#include "PlayerManager.h"
#include <fstream>
#include <iostream>

PlayerManager::PlayerManager(const std::string& filePath)
    : playersFilePath(filePath)
{
    loadAllPlayers();
}

std::shared_ptr<Player> PlayerManager::login(const std::string& name, const std::string& password)
{
    auto it = players.find(name);

    if (it == players.end())
    {
        return nullptr;  // Player not found
    }

    Player p = Player::fromJson(it->second);

    if (!p.verifyPassword(password))
    {
        return nullptr;  // Wrong password
    }

    // Store the player reference so we can update it later
    auto playerPtr = std::make_shared<Player>(p);
    // Update the map with the shared_ptr for future saves
    players[name] = playerPtr->toJson();

    return playerPtr;
}

bool PlayerManager::registerPlayer(const std::string& name, const std::string& password)
{
    if (players.find(name) != players.end())
    {
        return false;  // Player already exists
    }

    Player newPlayer(name, password);
    players[name] = newPlayer.toJson();
    saveAllPlayers();

    return true;
}

void PlayerManager::savePlayer(const std::shared_ptr<Player>& player)
{
    if (player)
    {
        std::cout << "\n=== savePlayer() called ===" << std::endl;
        std::cout << "Player: " << player->getName() << std::endl;
        std::cout << "High Score before save: " << player->getHighScore() << std::endl;

        json playerJson = player->toJson();
        std::cout << "JSON: " << playerJson.dump() << std::endl;

        players[player->getName()] = playerJson;
        saveAllPlayers();

        std::cout << "✓ Player saved to file" << std::endl;
        std::cout << "========================\n" << std::endl;
    }
}

void PlayerManager::loadAllPlayers()
{
    std::ifstream file(playersFilePath);

    if (file.is_open())
    {
        json data;
        file >> data;
        players = data.get<std::map<std::string, json>>();
        file.close();
    }
}

void PlayerManager::saveAllPlayers()
{
    std::ofstream file(playersFilePath);
    json data(players);
    file << data.dump(4);
    file.close();
}

std::vector<std::pair<std::string, int>> PlayerManager::getTopPlayers(int count)
{
    std::vector<std::pair<std::string, int>> topPlayers;

    for (const auto& [name, playerJson] : players)
    {
        Player p = Player::fromJson(playerJson);
        topPlayers.push_back({ name, p.getHighScore() });
    }

    // Sort by high score in descending order
    std::sort(topPlayers.begin(), topPlayers.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    // Return top N
    if (topPlayers.size() > static_cast<size_t>(count))
    {
        topPlayers.resize(count);
    }

    return topPlayers;
}
