#pragma once

#include "EDirection.h"
#include "EGameStatus.h"
#include "EPieceType.h"
#include "IGameListener.h"

class IGameController
{
public:
    virtual void startNewGame() = 0;
    virtual void rotate() = 0;
    virtual void pauseGame() = 0;
    virtual void move(EDirection direction) = 0;

    virtual void registerObserver(IGameListener* listener) = 0;
    virtual void unregisterObserver(IGameListener* listener) = 0;

    virtual void tick() = 0;

    virtual int getScore() const = 0;
    virtual EGameStatus getStatus() const = 0;

    virtual int getRowCount() const = 0;
    virtual int getColumnCount() const = 0;

    virtual EPieceType getCellType(int row, int column) const = 0;

    virtual void hardDrop() = 0;

    virtual ~IGameController() = default;
};
