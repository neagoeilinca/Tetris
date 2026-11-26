#pragma once

#include <memory>
#include <string>

#include "PlayerManager.h"

class LoginView
{
private:
    std::string username;
    std::string password;
    std::string errorMessage;
    bool inputUsername = true;
    PlayerManager& playerManager;

public:
    explicit LoginView(PlayerManager& pm);

    void reset();
    void handleInput();
    std::shared_ptr<Player> attemptLogin();
    void draw() const;
};