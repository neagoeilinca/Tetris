#pragma once

#include <gmock/gmock.h>

#include "IPieceTypeGenerator.h"

class MockPieceTypeGenerator : public IPieceTypeGenerator
{
public:
    MOCK_METHOD0(nextPieceType, EPieceType());
};