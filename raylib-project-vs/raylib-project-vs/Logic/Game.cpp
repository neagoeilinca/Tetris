#include "pch.h"
#include "Game.h"

#include <random>

Game::Game()
    : status(NOT_STARTED), score(0)
{
}

Piece Game::generateRandomPiece()
{
    // Generator pentru numere aleatorii
    static std::mt19937 generator(std::random_device{}());

    // Alegem un tip de piesa intre I si Z
    std::uniform_int_distribution<int> distribution(I, Z);

    EPieceType randomType =
        static_cast<EPieceType>(distribution(generator));

    return Piece(randomType, 3, 0);
}

void Game::startNewGame()
{
    board.reset();

    score = 0;

    status = RUNNING;

    spawnNewPiece();

    notifyObservers();
}

void Game::spawnNewPiece()
{
    current_piece = generateRandomPiece();

    // Daca piesa noua nu poate fi pozitionata,
    // jocul s-a terminat
    if (board.checkCollision(
        current_piece,
        current_piece.getX(),
        current_piece.getY()))
    {
        status = GAME_OVER;
    }

    notifyObservers();
}

void Game::rotate()
{
    if (status != RUNNING)
    {
        return;
    }

    // Lucram pe o copie pentru a nu pierde forma initiala
    Piece rotatedPiece = current_piece;

    rotatedPiece.rotate();

    // Aplicam rotirea doar daca nu apare o coliziune
    if (!board.checkCollision(
        rotatedPiece,
        rotatedPiece.getX(),
        rotatedPiece.getY()))
    {
        current_piece = rotatedPiece;

        notifyObservers();
    }
}

void Game::pauseGame()
{
    if (status == RUNNING)
    {
        status = PAUSED;
    }
    else if (status == PAUSED)
    {
        status = RUNNING;
    }
    else
    {
        return;
    }

    notifyObservers();
}

void Game::move(EDirection direction)
{
    if (status != RUNNING)
    {
        return;
    }

    int dx = 0;
    int dy = 0;

    switch (direction)
    {
    case LEFT:
        dx = -1;
        break;

    case RIGHT:
        dx = 1;
        break;

    case DOWN:
        dy = 1;
        break;
    }

    int newX = current_piece.getX() + dx;
    int newY = current_piece.getY() + dy;

    if (!board.checkCollision(current_piece, newX, newY))
    {
        current_piece.move(dx, dy);

        notifyObservers();
    }
    else if (direction == DOWN)
    {
        // Piesa nu mai poate cobori.
        // O fixam pe tabla.
        board.mergePiece(current_piece);

        // Eliminam liniile complete
        int clearedLines = board.clearLines();

        // Actualizam scorul
        updateScore(clearedLines);

        notifyObservers();

        // Generam urmatoarea piesa
        spawnNewPiece();
    }
}

void Game::tick()
{
    if (status != RUNNING)
    {
        return;
    }

    // La fiecare tick, piesa coboara un rand
    move(DOWN);
}

void Game::Tick()
{
    tick();
}

void Game::hardDrop()
{
    if (status != RUNNING)
    {
        return;
    }

    // Coboram piesa pana cand nu mai poate avansa
    while (!board.checkCollision(
        current_piece,
        current_piece.getX(),
        current_piece.getY() + 1))
    {
        current_piece.move(0, 1);
    }

    // Fixam piesa pe tabla
    board.mergePiece(current_piece);

    int clearedLines = board.clearLines();

    updateScore(clearedLines);

    notifyObservers();

    spawnNewPiece();
}

void Game::updateScore(int lines)
{
    // Sistem de punctaj in functie de numarul
    // de linii eliminate simultan
    switch (lines)
    {
    case 1:
        score += 100;
        break;

    case 2:
        score += 300;
        break;

    case 3:
        score += 500;
        break;

    case 4:
        score += 800;
        break;

    default:
        break;
    }

    if (lines > 0)
    {
        for (IGameListener* listener : listeners)
        {
            listener->onScoreChanged(score);
        }
    }
}

void Game::registerObserver(IGameListener* listener)
{
    if (listener == nullptr)
    {
        return;
    }

    // Nu inregistram acelasi observer de doua ori
    for (IGameListener* existing : listeners)
    {
        if (existing == listener)
        {
            return;
        }
    }

    listeners.push_back(listener);
}

void Game::unregisterObserver(IGameListener* listener)
{
    listeners.remove(listener);
}

void Game::notifyObservers()
{
    for (IGameListener* listener : listeners)
    {
        listener->onBoardChanged();

        listener->onGameStatusChanged(status);

        listener->onScoreChanged(score);
    }
}

int Game::getScore() const
{
    return score;
}

EGameStatus Game::getStatus() const
{
    return status;
}

int Game::getRowCount() const
{
    return 20;
}

int Game::getColumnCount() const
{
    return 10;
}

EPieceType Game::getCellType(int row, int column) const
{
    if (row < 0 || row >= 20 ||
        column < 0 || column >= 10)
    {
        return EMPTY;
    }

    // Verificam mai intai daca celula apartine piesei curente
    for (int pieceRow = 0; pieceRow < 4; pieceRow++)
    {
        for (int pieceCol = 0; pieceCol < 4; pieceCol++)
        {
            if (current_piece.getCell(pieceRow, pieceCol) == EMPTY)
            {
                continue;
            }

            int currentX = current_piece.getX() + pieceCol;
            int currentY = current_piece.getY() + pieceRow;

            if (currentX == column && currentY == row &&
                status == RUNNING)
            {
                return current_piece.getType();
            }
        }
    }

    // Daca nu apartine piesei curente,
    // returnam continutul tablei
    return board.getGrid()[row][column];
}

const Board& Game::getBoard() const
{
    return board;
}

const Piece& Game::getCurrentPiece() const
{
    return current_piece;
}
