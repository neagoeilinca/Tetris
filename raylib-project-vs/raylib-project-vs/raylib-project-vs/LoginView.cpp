#include "raylib.h"

#include "LoginView.h"
#include "UIConstants.h"

LoginView::LoginView(PlayerManager& pm) : playerManager(pm) {}

void LoginView::reset()
{
    username.clear();
    password.clear();
    errorMessage.clear();
    inputUsername = true;
}

void LoginView::handleInput()
{
    if (IsKeyPressed(KEY_TAB))
    {
        inputUsername = !inputUsername;
    }

    int key = GetCharPressed();
    while (key > 0)
    {
        if (inputUsername && username.length() < 20)
        {
            username += static_cast<char>(key);
        }
        else if (!inputUsername && password.length() < 20)
        {
            password += static_cast<char>(key);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (inputUsername && !username.empty())
        {
            username.pop_back();
        }
        else if (!inputUsername && !password.empty())
        {
            password.pop_back();
        }
    }
}

std::shared_ptr<Player> LoginView::attemptLogin()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        auto player = playerManager.login(username, password);

        if (player)
        {
            return player;
        }
        else
        {
            errorMessage = "Invalid username or password";
        }
    }

    return nullptr;
}

void LoginView::draw() const
{
    BeginDrawing();
    ClearBackground(Color{ 20, 20, 30, 255 });

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    DrawText("TETRIS LOGIN", centerX - 150, centerY - 100, 40, RAYWHITE);

    Color usernameBg = inputUsername ? Color{ 60, 60, 100, 255 } : Color{ 40, 40, 60, 255 };
    DrawRectangle(centerX - 150, centerY - 20, 300, 40, usernameBg);
    DrawRectangleLines(centerX - 150, centerY - 20, 300, 40, RAYWHITE);
    DrawText("Username:", centerX - 150, centerY - 50, 20, LIGHTGRAY);
    DrawText(username.c_str(), centerX - 140, centerY - 10, 20, RAYWHITE);

    Color passwordBg = !inputUsername ? Color{ 60, 60, 100, 255 } : Color{ 40, 40, 60, 255 };
    DrawRectangle(centerX - 150, centerY + 50, 300, 40, passwordBg);
    DrawRectangleLines(centerX - 150, centerY + 50, 300, 40, RAYWHITE);
    DrawText("Password:", centerX - 150, centerY + 20, 20, LIGHTGRAY);
    std::string maskedPassword(password.length(), '*');
    DrawText(maskedPassword.c_str(), centerX - 140, centerY + 60, 20, RAYWHITE);

    if (!errorMessage.empty())
    {
        DrawText(errorMessage.c_str(), centerX - 150, centerY + 110, 18, RED);
    }

    DrawText("Press ENTER to login | TAB to switch field | ESC to go back",
        centerX - 280, centerY + 150, 16, LIGHTGRAY);

    EndDrawing();
}