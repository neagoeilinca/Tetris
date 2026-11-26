#pragma once

#include "raylib.h"

#include "IGameController.h"
#include "IGameListener.h"

#include <array>

// Game UI (board + side panel + overlays). Observer of the game logic.
class TetrisView : public IGameListener
{
private:
    IGameController& controller;
    int score = 0;
    EGameStatus status = NOT_STARTED;
    float tickTimer = 0.0f;

    std::array<Color, 7> pieceColors = {
        SKYBLUE, BLUE, ORANGE, YELLOW, GREEN, PURPLE, RED
    };

    Color getColor(EPieceType type) const;
    void randomizePalette();

    void drawBoard() const;
    void drawSidePanel() const;
    void drawOverlay(const char* title, const char* subtitle) const;

public:
    explicit TetrisView(IGameController& gameController);
    ~TetrisView() override;

    // IGameListener
    void onGameStatusChanged(EGameStatus newStatus) override;
    void onBoardChanged() override;
    void onScoreChanged(int newScore) override;

    void handleInput();
    void update(float deltaTime);
    void draw() const;
};