#include "raylib.h"

#include "RegisterView.h"
#include "UIConstants.h"

RegisterView::RegisterView(PlayerManager& pm) : playerManager(pm) {}

void RegisterView::reset()
{
    username.clear();
    password.clear();
    confirmPassword.clear();
    message.clear();
    inputField = 0;
}

void RegisterView::handleInput()
{
    if (IsKeyPressed(KEY_TAB))
    {
        inputField = (inputField + 1) % 3;
    }

    int key = GetCharPressed();
    while (key > 0)
    {
        if (inputField == 0 && username.length() < 20)
        {
            username += static_cast<char>(key);
        }
        else if (inputField == 1 && password.length() < 20)
        {
            password += static_cast<char>(key);
        }
        else if (inputField == 2 && confirmPassword.length() < 20)
        {
            confirmPassword += static_cast<char>(key);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (inputField == 0 && !username.empty())
        {
            username.pop_back();
        }
        else if (inputField == 1 && !password.empty())
        {
            password.pop_back();
        }
        else if (inputField == 2 && !confirmPassword.empty())
        {
            confirmPassword.pop_back();
        }
    }
}

bool RegisterView::attemptRegister()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        if (username.empty() || password.empty())
        {
            message = "Username and password cannot be empty";
            return false;
        }

        if (password != confirmPassword)
        {
            message = "Passwords do not match";
            return false;
        }

        if (playerManager.registerPlayer(username, password))
        {
            message = "Registration successful! Press ESC to go back";
            return true;
        }
        else
        {
            message = "Username already exists";
            return false;
        }
    }

    return false;
}

void RegisterView::draw() const
{
    BeginDrawing();
    ClearBackground(Color{ 20, 20, 30, 255 });

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    DrawText("CREATE ACCOUNT", centerX - 180, centerY - 120, 40, RAYWHITE);

    // Username field
    Color usernameBg = inputField == 0 ? Color{ 60, 60, 100, 255 } : Color{ 40, 40, 60, 255 };
    DrawRectangle(centerX - 150, centerY - 50, 300, 40, usernameBg);
    DrawRectangleLines(centerX - 150, centerY - 50, 300, 40, RAYWHITE);
    DrawText("Username:", centerX - 150, centerY - 85, 20, LIGHTGRAY);
    DrawText(username.c_str(), centerX - 140, centerY - 40, 20, RAYWHITE);

    // Password field
    Color passwordBg = inputField == 1 ? Color{ 60, 60, 100, 255 } : Color{ 40, 40, 60, 255 };
    DrawRectangle(centerX - 150, centerY + 10, 300, 40, passwordBg);
    DrawRectangleLines(centerX - 150, centerY + 10, 300, 40, RAYWHITE);
    DrawText("Password:", centerX - 150, centerY - 20, 20, LIGHTGRAY);
    std::string maskedPassword(password.length(), '*');
    DrawText(maskedPassword.c_str(), centerX - 140, centerY + 20, 20, RAYWHITE);

    // Confirm Password field
    Color confirmBg = inputField == 2 ? Color{ 60, 60, 100, 255 } : Color{ 40, 40, 60, 255 };
    DrawRectangle(centerX - 150, centerY + 70, 300, 40, confirmBg);
    DrawRectangleLines(centerX - 150, centerY + 70, 300, 40, RAYWHITE);
    DrawText("Confirm Pwd:", centerX - 150, centerY + 35, 20, LIGHTGRAY);
    std::string maskedConfirm(confirmPassword.length(), '*');
    DrawText(maskedConfirm.c_str(), centerX - 140, centerY + 80, 20, RAYWHITE);

    // Message
    Color msgColor = message.find("successful") != std::string::npos ? GREEN : RED;
    if (!message.empty())
    {
        DrawText(message.c_str(), centerX - 200, centerY + 130, 16, msgColor);
    }

    DrawText("Press ENTER to register | TAB to switch field | ESC to go back",
        centerX - 300, centerY + 170, 16, LIGHTGRAY);

    EndDrawing();
}