#include "raylib.h"

#include "TetrisView.h"
#include "UIConstants.h"

#include <algorithm>
#include <random>
#include <string>

TetrisView::TetrisView(IGameController& gameController)
    : controller(gameController)
{
    randomizePalette();
    controller.registerObserver(this);
}

TetrisView::~TetrisView()
{
    controller.unregisterObserver(this);
}

Color TetrisView::getColor(EPieceType type) const
{
    int index = static_cast<int>(type) - 1;
    if (index >= 0 && index < static_cast<int>(pieceColors.size()))
    {
        return pieceColors[index];
    }
    return BLANK;
}

void TetrisView::randomizePalette()
{
    static std::mt19937 rng(std::random_device{}());
    std::shuffle(pieceColors.begin(), pieceColors.end(), rng);
}

// ---------------- IGameListener ----------------

void TetrisView::onGameStatusChanged(EGameStatus newStatus)
{
    status = newStatus;
}

void TetrisView::onBoardChanged() {}

void TetrisView::onScoreChanged(int newScore)
{
    score = newScore;
}

// ---------------- Input / update ----------------

void TetrisView::handleInput()
{
    if (IsKeyPressed(KEY_N))
    {
        randomizePalette();
        controller.startNewGame();
        tickTimer = 0.0f;
        return;
    }

    if (IsKeyPressed(KEY_P))
    {
        controller.pauseGame();
    }

    if (status != RUNNING)
    {
        return;
    }

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT) ||
        IsKeyPressed(KEY_A) || IsKeyPressedRepeat(KEY_A))
    {
        controller.move(LEFT);
    }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
        IsKeyPressed(KEY_D) || IsKeyPressedRepeat(KEY_D))
    {
        controller.move(RIGHT);
    }

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN) ||
        IsKeyPressed(KEY_S) || IsKeyPressedRepeat(KEY_S))
    {
        controller.move(DOWN);
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        controller.rotate();
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        controller.hardDrop();
    }
}

void TetrisView::update(float deltaTime)
{
    if (status != RUNNING)
    {
        return;
    }

    tickTimer += deltaTime;

    if (tickTimer >= TICK_INTERVAL)
    {
        tickTimer = 0.0f;
        controller.tick();
    }
}

// ---------------- Drawing ----------------

void TetrisView::draw() const
{
    BeginDrawing();
    ClearBackground(Color{ 20, 20, 30, 255 });

    drawBoard();
    drawSidePanel();

    if (status == PAUSED)
    {
        drawOverlay("PAUSED", "Apasa P pentru a continua");
    }
    else if (status == GAME_OVER)
    {
        drawOverlay("GAME OVER", "Apasa N pentru joc nou");
    }
    else if (status == NOT_STARTED)
    {
        drawOverlay("TETRIS", "Apasa N pentru a incepe");
    }

    EndDrawing();
}

void TetrisView::drawBoard() const
{
    const int rows = controller.getRowCount();
    const int cols = controller.getColumnCount();

    DrawRectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y,
        cols * CELL_SIZE, rows * CELL_SIZE,
        Color{ 35, 35, 50, 255 });

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            int x = BOARD_OFFSET_X + col * CELL_SIZE;
            int y = BOARD_OFFSET_Y + row * CELL_SIZE;

            EPieceType cell = controller.getCellType(row, col);

            if (cell != EMPTY)
            {
                DrawRectangle(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2,
                    getColor(cell));
            }

            DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE,
                Color{ 60, 60, 80, 255 });
        }
    }

    DrawRectangleLines(BOARD_OFFSET_X, BOARD_OFFSET_Y,
        cols * CELL_SIZE, rows * CELL_SIZE, RAYWHITE);
}

void TetrisView::drawSidePanel() const
{
    const int panelX = 400;

    DrawText("TETRIS", panelX, 30, 40, RAYWHITE);

    DrawText("Score", panelX, 110, 24, LIGHTGRAY);
    DrawText(std::to_string(score).c_str(), panelX, 140, 36, YELLOW);

    DrawText("Controls", panelX, 230, 24, LIGHTGRAY);
    DrawText("A / Left  - Move left", panelX, 265, 20, RAYWHITE);
    DrawText("D / Right - Move right", panelX, 290, 20, RAYWHITE);
    DrawText("S / Down  - Move down", panelX, 315, 20, RAYWHITE);
    DrawText("W / Up    - Rotate", panelX, 340, 20, RAYWHITE);
    DrawText("Space     - Hard drop", panelX, 365, 20, RAYWHITE);
    DrawText("P         - Pause / Resume", panelX, 390, 20, RAYWHITE);
    DrawText("N         - New game", panelX, 415, 20, RAYWHITE);
    DrawText("L         - Leaderboard", panelX, 440, 20, RAYWHITE);
    DrawText("Esc       - Logout", panelX, 465, 20, RAYWHITE);
}

void TetrisView::drawOverlay(const char* title, const char* subtitle) const
{
    const int boardWidth = controller.getColumnCount() * CELL_SIZE;
    const int boardHeight = controller.getRowCount() * CELL_SIZE;

    DrawRectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y,
        boardWidth, boardHeight, Color{ 0, 0, 0, 170 });

    int titleWidth = MeasureText(title, 40);
    int subtitleWidth = MeasureText(subtitle, 18);

    int centerX = BOARD_OFFSET_X + boardWidth / 2;
    int centerY = BOARD_OFFSET_Y + boardHeight / 2;

    DrawText(title, centerX - titleWidth / 2, centerY - 30, 40, RAYWHITE);
    DrawText(subtitle, centerX - subtitleWidth / 2, centerY + 20, 18, LIGHTGRAY);
}