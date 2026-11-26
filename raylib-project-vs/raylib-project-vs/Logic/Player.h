#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Player
{
private:
	std::string name;
	std::string password;
	int highScore;
public:
	Player();
	explicit Player(const std::string& playerName, const std::string& playerPassword);

	std::string getName();
	int getHighScore();

	void setHighScore(int score);
	void setName(const std::string& newName);

	bool verifyPassword(const std::string& pwd) const;

	json toJson() const;
	static Player fromJson(const json& j);
};

