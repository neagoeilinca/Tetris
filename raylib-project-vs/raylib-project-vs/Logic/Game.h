
#pragma once

#include "IGameController.h"
#include "Board.h"
#include "Piece.h"

#include <list>

class Game : public IGameController
{
private:
    Board board;
    Piece current_piece;

    EGameStatus status;
    int score;

    std::list<IGameListener*> listeners;

    Piece generateRandomPiece();

    void spawnNewPiece();

    void notifyObservers();

    void updateScore(int lines);

public:
    Game();

    void startNewGame() override;

    void rotate() override;

    void pauseGame() override;

    void move(EDirection direction) override;

    void registerObserver(IGameListener* listener) override;

    void unregisterObserver(IGameListener* listener) override;

    void tick() override;


    void hardDrop() override;

    int getScore() const override;

    EGameStatus getStatus() const override;

    int getRowCount() const override;

    int getColumnCount() const override;

    EPieceType getCellType(int row, int column) const override;

    const Board& getBoard() const;

    const Piece& getCurrentPiece() const;
};
