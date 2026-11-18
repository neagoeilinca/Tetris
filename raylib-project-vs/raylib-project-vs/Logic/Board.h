#pragma once
#include "EPieceType.h"
#include "Piece.h"

class Board
{
private:
	EPieceType grid[20][10];
public:
	bool checkCollision(Piece piece, int x, int y);
	void mergePiece(Piece piece);
	int clearLines();
	EPieceType (*getGrid())[20][10];

};
