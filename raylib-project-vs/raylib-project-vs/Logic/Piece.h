#pragma once
#include <array>
#include "EPieceType.h"
class Piece
{
private:
	EPieceType type;
	std::array<std::array<EPieceType, 4>, 4> shape;
	int x;
	int y;
public:
	void rotate();
	void move(int dx, int dy);
};

