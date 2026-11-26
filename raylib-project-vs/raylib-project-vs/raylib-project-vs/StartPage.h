#pragma once

// Main menu: Login / Register
class StartPage
{
private:
    int selectedOption = 0;  // 0 = Login, 1 = Register, 2 = Leaderboard

public:
    void handleInput();
    int getSelectedOption() const;
    void draw() const;
};