#include "pch.h"
#include "Piece.h"

Piece::Piece()
    : type(EMPTY), x(0), y(0)
{
    for (auto& row : shape)
    {
        row.fill(EMPTY);
    }
}

Piece::Piece(EPieceType pieceType, int startX, int startY)
    : type(pieceType), x(startX), y(startY)
{
    for (auto& row : shape)
    {
        row.fill(EMPTY);
    }

    switch (type)
    {
    case I:
        shape[1] = { I, I, I, I };
        break;

    case J:
        shape[0] = { J, EMPTY, EMPTY, EMPTY };
        shape[1] = { J, J, J, EMPTY };
        break;

    case L:
        shape[0] = { EMPTY, EMPTY, L, EMPTY };
        shape[1] = { L, L, L, EMPTY };
        break;

    case O:
        shape[0] = { EMPTY, O, O, EMPTY };
        shape[1] = { EMPTY, O, O, EMPTY };
        break;

    case S:
        shape[0] = { EMPTY, S, S, EMPTY };
        shape[1] = { S, S, EMPTY, EMPTY };
        break;

    case T:
        shape[0] = { EMPTY, T, EMPTY, EMPTY };
        shape[1] = { T, T, T, EMPTY };
        break;

    case Z:
        shape[0] = { Z, Z, EMPTY, EMPTY };
        shape[1] = { EMPTY, Z, Z, EMPTY };
        break;

    default:
        break;
    }
}

void Piece::rotate()
{
    std::array<std::array<EPieceType, 4>, 4> rotated;

    for (auto& row : rotated)
    {
        row.fill(EMPTY);
    }

    // Rotire la 90 de grade in sensul acelor de ceasornic
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            rotated[col][3 - row] = shape[row][col];
        }
    }

    shape = rotated;
}

void Piece::move(int dx, int dy)
{
    x += dx;
    y += dy;
}

EPieceType Piece::getType() const
{
    return type;
}

int Piece::getX() const
{
    return x;
}

int Piece::getY() const
{
    return y;
}

EPieceType Piece::getCell(int row, int column) const
{
    if (row < 0 || row >= 4 || column < 0 || column >= 4)
    {
        return EMPTY;
    }

    return shape[row][column];
}
