#include "raylib.h"

#include "Game.h"
#include "PlayerManager.h"
#include "EAppState.h"

#include "UIConstants.h"
#include "StartPage.h"
#include "LoginView.h"
#include "RegisterView.h"
#include "LeaderboardView.h"
#include "TetrisView.h"

#include <memory>

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