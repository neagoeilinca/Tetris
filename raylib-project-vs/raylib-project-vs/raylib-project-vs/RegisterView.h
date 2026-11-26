#pragma once

#include <string>

#include "PlayerManager.h"

class RegisterView
{
private:
    std::string username;
    std::string password;
    std::string confirmPassword;
    std::string message;
    int inputField = 0;  // 0 = username, 1 = password, 2 = confirm password
    PlayerManager& playerManager;

public:
    explicit RegisterView(PlayerManager& pm);

    void reset();
    void handleInput();
    bool attemptRegister();
    void draw() const;
};