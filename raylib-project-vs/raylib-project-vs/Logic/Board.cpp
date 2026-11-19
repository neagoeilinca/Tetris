#include "pch.h"
#include "Board.h"

Board::Board()
{
    reset();
}

void Board::reset()
{
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            grid[row][col] = EMPTY;
        }
    }
}

bool Board::checkCollision(const Piece& piece, int x, int y) const
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            EPieceType cell = piece.getCell(row, col);

            // Ignoram celulele goale ale piesei
            if (cell == EMPTY)
            {
                continue;
            }

            int boardX = x + col;
            int boardY = y + row;

            // Verificam marginile tablei
            if (boardX < 0 || boardX >= 10 ||
                boardY < 0 || boardY >= 20)
            {
                return true;
            }

            // Verificam daca celula este deja ocupata
            if (grid[boardY][boardX] != EMPTY)
            {
                return true;
            }
        }
    }

    return false;
}

void Board::mergePiece(const Piece& piece)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            EPieceType cell = piece.getCell(row, col);

            if (cell == EMPTY)
            {
                continue;
            }

            int boardX = piece.getX() + col;
            int boardY = piece.getY() + row;

            if (boardX >= 0 && boardX < 10 &&
                boardY >= 0 && boardY < 20)
            {
                grid[boardY][boardX] = cell;
            }
        }
    }
}

int Board::clearLines()
{
    int clearedLines = 0;

    // Parcurgem tabla de jos in sus
    for (int row = 19; row >= 0; row--)
    {
        bool fullLine = true;

        for (int col = 0; col < 10; col++)
        {
            if (grid[row][col] == EMPTY)
            {
                fullLine = false;
                break;
            }
        }

        if (fullLine)
        {
            clearedLines++;

            // Mutam toate liniile de deasupra cu un rand in jos
            for (int moveRow = row; moveRow > 0; moveRow--)
            {
                for (int col = 0; col < 10; col++)
                {
                    grid[moveRow][col] = grid[moveRow - 1][col];
                }
            }

            // Prima linie devine goala
            for (int col = 0; col < 10; col++)
            {
                grid[0][col] = EMPTY;
            }

            // Verificam din nou acelasi rand
            row++;
        }
    }

    return clearedLines;
}

const EPieceType(&Board::getGrid() const)[20][10]
{
    return grid;
}
