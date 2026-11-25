#pragma once

#include "IGameController.h"
#include "IPieceTypeGenerator.h"
#include "Board.h"
#include "Piece.h"

#include <list>
#include <memory>

class Game : public IGameController
{
private:
    Board board;
    Piece current_piece;
    std::unique_ptr<IPieceTypeGenerator> pieceTypeGenerator;

    EGameStatus status;
    int score;

    std::list<IGameListener*> listeners;

    Piece generateNextPiece();

    void spawnNewPiece();

    void lockCurrentPiece();

    void notifyObservers();

public:
    Game();

    explicit Game(std::unique_ptr<IPieceTypeGenerator> generator);

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