#include "pch.h"
#include "Game.h"
#include "RandomPieceTypeGenerator.h"

#include <utility>

namespace
{
    // Punctaj in functie de numarul de linii eliminate simultan
    int calculateScore(int clearedLines)
    {
        switch (clearedLines)
        {
        case 1: return 100;
        case 2: return 300;
        case 3: return 500;
        case 4: return 800;
        default: return 0;
        }
    }
}

Game::Game()
    : Game(std::make_unique<RandomPieceTypeGenerator>())
{}

Game::Game(std::unique_ptr<IPieceTypeGenerator> generator)
    : pieceTypeGenerator(std::move(generator)), status(NOT_STARTED), score(0)
{}

Piece Game::generateNextPiece()
{
    return Piece(pieceTypeGenerator->nextPieceType(), 3, 0);
}

void Game::startNewGame()
{
    board.reset();
    score = 0;
    status = RUNNING;

    spawnNewPiece();
}

void Game::spawnNewPiece()
{
    current_piece = generateNextPiece();

    // Daca piesa noua nu poate fi pozitionata, jocul s-a terminat
    if (board.checkCollision(current_piece, current_piece.getX(), current_piece.getY()))
    {
        status = GAME_OVER;
    }

    notifyObservers();
}

void Game::lockCurrentPiece()
{
    board.mergePiece(current_piece);

    const int clearedLines = board.clearLines();
    score += calculateScore(clearedLines);

    spawnNewPiece();
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

    if (!board.checkCollision(rotatedPiece, rotatedPiece.getX(), rotatedPiece.getY()))
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

    if (!board.checkCollision(current_piece, current_piece.getX() + dx, current_piece.getY() + dy))
    {
        current_piece.move(dx, dy);
        notifyObservers();
    }
    else if (direction == DOWN)
    {
        // Piesa nu mai poate cobori: o fixam pe tabla
        lockCurrentPiece();
    }
}

void Game::tick()
{
    if (status != RUNNING)
    {
        return;
    }

    move(DOWN);
}

void Game::hardDrop()
{
    if (status != RUNNING)
    {
        return;
    }

    while (!board.checkCollision(current_piece, current_piece.getX(), current_piece.getY() + 1))
    {
        current_piece.move(0, 1);
    }

    lockCurrentPiece();
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
    if (row < 0 || row >= 20 || column < 0 || column >= 10)
    {
        return EMPTY;
    }

    // Piesa curenta are prioritate fata de continutul tablei
    const EPieceType pieceCell = current_piece.getCell(
        row - current_piece.getY(),
        column - current_piece.getX());

    if (pieceCell != EMPTY)
    {
        return pieceCell;
    }

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