#include <iostream>
#include <cstdlib>
#include "Game.h"

using namespace std;

void displayGame(const Game& game)
{
    system("cls");

    cout << "========== TETRIS ==========\n\n";
    cout << "Score: " << game.getScore() << "\n\n";

    if (game.getStatus() == GAME_OVER)
    {
        cout << "GAME OVER!\n";
        cout << "Final score: " << game.getScore() << "\n";
        return;
    }

    if (game.getStatus() == PAUSED)
    {
        cout << "PAUSED\n\n";
    }

    cout << "+--------------------+\n";

    for (int row = 0; row < game.getRowCount(); row++)
    {
        cout << "|";

        for (int col = 0; col < game.getColumnCount(); col++)
        {
            EPieceType cell = game.getCellType(row, col);

            if (cell == EMPTY)
                cout << " .";
            else
                cout << " X";
        }

        cout << " |\n";
    }

    cout << "+--------------------+\n";

    cout << "\nControls:\n";
    cout << "A - Move left\n";
    cout << "D - Move right\n";
    cout << "S - Move down\n";
    cout << "W - Rotate\n";
    cout << "H - Hard drop\n";
    cout << "T - Tick\n";
    cout << "P - Pause / Resume\n";
    cout << "N - New game\n";
    cout << "Q - Quit\n";
}

int main()
{
    Game game;

    game.startNewGame();

    char command;

    while (true)
    {
        displayGame(game);

        cout << "\nCommand: ";
        cin >> command;

        if (command == 'q' || command == 'Q')
            break;

        if (command == 'n' || command == 'N')
        {
            game.startNewGame();
            continue;
        }

        if (game.getStatus() == GAME_OVER)
            continue;

        switch (command)
        {
        case 'a':
        case 'A':
            game.move(LEFT);
            break;

        case 'd':
        case 'D':
            game.move(RIGHT);
            break;

        case 's':
        case 'S':
            game.move(DOWN);
            break;

        case 'w':
        case 'W':
            game.rotate();
            break;

        case 'h':
        case 'H':
            game.hardDrop();
            break;

        case 't':
        case 'T':
            game.tick();
            break;

        case 'p':
        case 'P':
            game.pauseGame();
            break;
        }
    }

    cout << "\nThanks for playing!\n";

    return 0;
}
