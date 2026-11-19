#pragma once

#include "EGameStatus.h"

class IGameListener
{
public:
    virtual void onGameStatusChanged(EGameStatus status) = 0;
    virtual void onBoardChanged() = 0;
    virtual void onScoreChanged(int score) = 0;

    virtual ~IGameListener() = default;
};
