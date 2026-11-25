#include "pch.h"
#include "RandomPieceTypeGenerator.h"

RandomPieceTypeGenerator::RandomPieceTypeGenerator()
    : generator(std::random_device{}()), distribution(I, Z)
{}

EPieceType RandomPieceTypeGenerator::nextPieceType()
{
    return static_cast<EPieceType>(distribution(generator));
}