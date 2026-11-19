#pragma once

#include "EPieceType.h"
#include "Piece.h"

class Board
{
private:
    EPieceType grid[20][10];

public:
    Board();

    void reset();

    bool checkCollision(const Piece& piece, int x, int y) const;

    void mergePiece(const Piece& piece);

    int clearLines();

    const EPieceType(&getGrid() const)[20][10];
};
