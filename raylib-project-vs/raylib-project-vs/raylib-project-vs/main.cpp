#include "raylib.h"

#include "IGameController.h"
#include "IGameListener.h"
#include "Game.h"
#include "PlayerManager.h"
#include "EAppState.h"

#include <string>
#include <array>
#include <random>
#include <algorithm>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

const int CELL_SIZE = 30;
const int BOARD_OFFSET_X = 50;
const int BOARD_OFFSET_Y = 20;

const float TICK_INTERVAL = 0.5f;



// ---------------------------------------------------------------
// START PAGE - Main Menu
// ---------------------------------------------------------------
class StartPage
{
private:
    int selectedOption = 0;  // 0 = Login, 1 = Register, 2 = Leaderboard

public:
    void handleInput()
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

    int getSelectedOption() const
    {
        return selectedOption;
    }

    void draw() const
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
};

// ---------------------------------------------------------------
// LOGIN VIEW
// ---------------------------------------------------------------
class LoginView
{
private:
    std::string username;
    std::string password;
    std::string errorMessage;
    bool inputUsername = true;
    PlayerManager& playerManager;

public:
    explicit LoginView(PlayerManager& pm) : playerManager(pm) {}

    void reset()
    {
        username.clear();
        password.clear();
        errorMessage.clear();
        inputUsername = true;
    }

    void handleInput()
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

    std::shared_ptr<Player> attemptLogin()
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

    void draw() const
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
};

// ---------------------------------------------------------------
// REGISTER VIEW (Sign Up)
// ---------------------------------------------------------------
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
    explicit RegisterView(PlayerManager& pm) : playerManager(pm) {}

    void reset()
    {
        username.clear();
        password.clear();
        confirmPassword.clear();
        message.clear();
        inputField = 0;
    }

    void handleInput()
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

    bool attemptRegister()
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

    void draw() const
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
};
// ---------------------------------------------------------------
// LEADERBOARD VIEW
// ---------------------------------------------------------------
class LeaderboardView
{
private:
    PlayerManager& playerManager;
    std::vector<std::pair<std::string, int>> leaderboard;

    void loadLeaderboard()
    {
        leaderboard = playerManager.getTopPlayers(10);  // Top 10 players
    }

public:
    explicit LeaderboardView(PlayerManager& pm) : playerManager(pm)
    {
        loadLeaderboard();
    }

    void refresh()
    {
        loadLeaderboard();
    }

    void draw() const
    {
        BeginDrawing();
        ClearBackground(Color{ 20, 20, 30, 255 });

        int centerX = SCREEN_WIDTH / 2;
        int startY = 60;

        DrawText("LEADERBOARD", centerX - 150, startY, 50, RAYWHITE);

        // Column headers
        DrawText("Rank", 100, startY + 70, 24, YELLOW);
        DrawText("Player Name", 200, startY + 70, 24, YELLOW);
        DrawText("High Score", 550, startY + 70, 24, YELLOW);

        DrawLine(80, startY + 100, SCREEN_WIDTH - 80, startY + 100, RAYWHITE);

        // Display leaderboard entries
        int yOffset = startY + 120;
        const int lineHeight = 40;

        if (leaderboard.empty())
        {
            DrawText("No players yet", centerX - 80, yOffset, 24, LIGHTGRAY);
        }
        else
        {
            for (size_t i = 0; i < leaderboard.size(); ++i)
            {
                int yPos = yOffset + (i * lineHeight);

                // Rank
                std::string rank = std::to_string(i + 1);
                Color rankColor = i == 0 ? GOLD : (i == 1 ? LIGHTGRAY : (i == 2 ? Color{ 205, 127, 50, 255 } : LIGHTGRAY));
                DrawText(rank.c_str(), 120, yPos, 24, rankColor);

                // Player name
                DrawText(leaderboard[i].first.c_str(), 200, yPos, 24, RAYWHITE);

                // High score
                std::string score = std::to_string(leaderboard[i].second);
                DrawText(score.c_str(), 550, yPos, 24, YELLOW);

                // Medal for top 3
                if (i == 0)
                {
                    DrawText("🥇", 50, yPos, 28, GOLD);
                }
                else if (i == 1)
                {
                    DrawText("🥈", 50, yPos, 28, LIGHTGRAY);
                }
                else if (i == 2)
                {
                    DrawText("🥉", 50, yPos, 28, Color{ 205, 127, 50, 255 });
                }
            }
        }

        // Instructions
        DrawText("Press ESC to go back", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT - 50, 20, LIGHTGRAY);

        EndDrawing();
    }
};
// ---------------------------------------------------------------
// TETRIS VIEW (Game UI)
// ---------------------------------------------------------------
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

    Color getColor(EPieceType type) const
    {
        int index = static_cast<int>(type) - 1;
        if (index >= 0 && index < static_cast<int>(pieceColors.size()))
        {
            return pieceColors[index];
        }
        return BLANK;
    }

    void randomizePalette()
    {
        static std::mt19937 rng(std::random_device{}());
        std::shuffle(pieceColors.begin(), pieceColors.end(), rng);
    }

public:
    explicit TetrisView(IGameController& gameController)
        : controller(gameController)
    {
        randomizePalette();
        controller.registerObserver(this);
    }

    ~TetrisView() override
    {
        controller.unregisterObserver(this);
    }

    void onGameStatusChanged(EGameStatus newStatus) override
    {
        status = newStatus;
    }

    void onBoardChanged() override {}

    void onScoreChanged(int newScore) override
    {
        score = newScore;
    }

    void handleInput()
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

    void update(float deltaTime)
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

    void draw() const
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

private:
    void drawBoard() const
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

    void drawSidePanel() const
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
        DrawText("L         - Leaderboard", panelX, 440, 20, RAYWHITE);  // NEW
        DrawText("Esc       - Logout", panelX, 465, 20, RAYWHITE);
    }
    void drawOverlay(const char* title, const char* subtitle) const
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
};

// ---------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------
int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");
    SetTargetFPS(60);

    PlayerManager playerManager("players.json");

    EAppState appState = EAppState::START_PAGE;
    std::shared_ptr<Player> currentPlayer;
    std::unique_ptr<Game> game;
    std::unique_ptr<TetrisView> view;

    StartPage startPage;
    LoginView loginView(playerManager);
    RegisterView registerView(playerManager);
    LeaderboardView leaderboardView(playerManager);

    while (!WindowShouldClose() && appState != EAppState::QUIT)
    {
        if (appState == EAppState::START_PAGE)
        {
            startPage.handleInput();

            if (IsKeyPressed(KEY_ENTER))
            {
                if (startPage.getSelectedOption() == 0)
                {
                    appState = EAppState::LOGIN;
                    loginView.reset();
                }
                else if (startPage.getSelectedOption() == 1)
                {
                    appState = EAppState::REGISTER;
                    registerView.reset();
                }
                
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                appState = EAppState::QUIT;
            }

            startPage.draw();
        }
        else if (appState == EAppState::LOGIN)
        {
            loginView.handleInput();
            currentPlayer = loginView.attemptLogin();

            if (currentPlayer)
            {
                game = std::make_unique<Game>(currentPlayer);
                view = std::make_unique<TetrisView>(*game);
                game->startNewGame();
                appState = EAppState::GAME;
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                appState = EAppState::START_PAGE;
            }

            loginView.draw();
        }
        else if (appState == EAppState::REGISTER)
        {
            registerView.handleInput();
            registerView.attemptRegister();

            if (IsKeyPressed(KEY_ESCAPE))
            {
                appState = EAppState::START_PAGE;
            }

            registerView.draw();
        }
        else if (appState == EAppState::LEADERBOARD)
        {
            if (IsKeyPressed(KEY_ESCAPE))
            {
                appState = EAppState::GAME;
            }

            leaderboardView.draw();
        }
        else if (appState == EAppState::GAME)
        {
            if (IsKeyPressed(KEY_N))
            {
                playerManager.savePlayer(currentPlayer);
                view->handleInput();
            }
            if (IsKeyPressed(KEY_L))
            {
                leaderboardView.refresh();
                appState = EAppState::LEADERBOARD;
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {

                playerManager.savePlayer(currentPlayer);

                appState = EAppState::START_PAGE;
                view.reset();
                game.reset();
                currentPlayer.reset();
            }
            else
            {
                view->handleInput();
                view->update(GetFrameTime());
                view->draw();
            }
        }
    }

    if (currentPlayer)
    {
        playerManager.savePlayer(currentPlayer);
    }

    CloseWindow();
    return 0;
}