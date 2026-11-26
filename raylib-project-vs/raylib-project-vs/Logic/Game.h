#pragma once

#include "IGameController.h"
#include "IPieceTypeGenerator.h"
#include "Board.h"
#include "Piece.h"
#include "Player.h"
#include <nlohmann/json.hpp>

#include <list>
#include <memory>

using json = nlohmann::json;

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

    std::shared_ptr<Player> player;

    void spawnNewPiece();

    void lockCurrentPiece();

    void notifyObservers();

    void updatePlayerHighScore();

public:
    Game();

    explicit Game(std::unique_ptr<IPieceTypeGenerator> generator);

    explicit Game(std::shared_ptr<Player> playerPtr,
        std::unique_ptr<IPieceTypeGenerator> generator = nullptr);

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

    const Player* getPlayer() const;
};