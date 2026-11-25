#pragma once

#include <gmock/gmock.h>

#include "IGameListener.h"

class MockGameListener : public IGameListener
{
public:
    MOCK_METHOD1(onGameStatusChanged, void(EGameStatus));
    MOCK_METHOD0(onBoardChanged, void());
    MOCK_METHOD1(onScoreChanged, void(int));
};