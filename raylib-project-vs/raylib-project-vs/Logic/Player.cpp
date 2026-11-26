#include "pch.h"
#include "Player.h"

Player::Player()
	: name(""), password(""), highScore(0)
{
}

Player::Player(const std::string& playerName, const std::string& playerPassword)
	: name(playerName), password(playerPassword), highScore(0)
{
}

std::string Player::getName()
{
	return name;
}

int Player::getHighScore()
{
	return highScore;
}

void Player::setHighScore(int score)
{
	highScore = score;
}

void Player::setName(const std::string& newName)
{
	name = newName;
}

bool Player::verifyPassword(const std::string& pwd) const
{
    return password == pwd;
}

json Player::toJson() const
{
    json j;
    j["name"] = name;
    j["password"] = password;
    j["highScore"] = highScore;
    return j;
}

Player Player::fromJson(const json& j)
{
    Player p;
    p.name = j["name"];
    p.password = j["password"];
    p.highScore = j["highScore"];
    return p;
}