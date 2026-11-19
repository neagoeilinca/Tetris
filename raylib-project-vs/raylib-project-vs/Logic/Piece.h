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
    Piece();
    Piece(EPieceType type, int startX, int startY);

    void rotate();
    void move(int dx, int dy);

    EPieceType getType() const;

    int getX() const;
    int getY() const;

    EPieceType getCell(int row, int column) const;
};
