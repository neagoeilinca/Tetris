#include "raylib.h"

#include "StartPage.h"
#include "UIConstants.h"

void StartPage::handleInput()
{
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedOption = (selectedOption - 1 + 3) % 3;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedOption = (selectedOption + 1) % 3;
    }
}

int StartPage::getSelectedOption() const
{
    return selectedOption;
}

void StartPage::draw() const
{
    BeginDrawing();
    ClearBackground(Color{ 20, 20, 30, 255 });

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    DrawText("TETRIS", centerX - 100, centerY - 150, 80, RAYWHITE);

    // Login option
    Color loginColor = selectedOption == 0 ? YELLOW : LIGHTGRAY;
    DrawText("Login", centerX - 50, centerY - 20, 40, loginColor);
    if (selectedOption == 0)
    {
        DrawText("> Press ENTER <", centerX - 100, centerY + 20, 20, YELLOW);
    }

    // Register option
    Color registerColor = selectedOption == 1 ? YELLOW : LIGHTGRAY;
    DrawText("Register", centerX - 80, centerY + 60, 40, registerColor);
    if (selectedOption == 1)
    {
        DrawText("> Press ENTER <", centerX - 100, centerY + 100, 20, YELLOW);
    }
    DrawText("Press ESC to Quit", centerX - 120, centerY + 250, 20, DARKGRAY);

    EndDrawing();
}