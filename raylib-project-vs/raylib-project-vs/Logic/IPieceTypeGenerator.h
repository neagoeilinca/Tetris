#pragma once

#include "EPieceType.h"

class IPieceTypeGenerator
{
public:
    virtual EPieceType nextPieceType() = 0;

    virtual ~IPieceTypeGenerator() = default;
};
