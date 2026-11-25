#pragma once

#include "IPieceTypeGenerator.h"

#include <random>

class RandomPieceTypeGenerator : public IPieceTypeGenerator
{
private:
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

public:
    RandomPieceTypeGenerator();

    EPieceType nextPieceType() override;
};