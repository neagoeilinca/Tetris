#include <gtest/gtest.h>

#include "Piece.h"

namespace
{
    const EPieceType ALL_PIECE_TYPES[] = { I, J, L, O, S, T, Z };

    int countCellsOfType(const Piece& piece, EPieceType type)
    {
        int count = 0;

        for (int row = 0; row < 4; row++)
        {
            for (int column = 0; column < 4; column++)
            {
                if (piece.getCell(row, column) == type)
                {
                    count++;
                }
            }
        }

        return count;
    }

    int countFilledCells(const Piece& piece)
    {
        return 16 - countCellsOfType(piece, EMPTY);
    }
}

TEST(PieceTests, DefaultPiece_IsEmptyAtOrigin)
{
    Piece piece;

    EXPECT_EQ(EMPTY, piece.getType());
    EXPECT_EQ(0, piece.getX());
    EXPECT_EQ(0, piece.getY());
    EXPECT_EQ(0, countFilledCells(piece));
}

TEST(PieceTests, Constructor_StoresTypeAndPosition)
{
    Piece piece(T, 3, 5);

    EXPECT_EQ(T, piece.getType());
    EXPECT_EQ(3, piece.getX());
    EXPECT_EQ(5, piece.getY());
}

TEST(PieceTests, EveryRealPieceType_HasFourCellsOfItsOwnType)
{
    for (EPieceType type : ALL_PIECE_TYPES)
    {
        Piece piece(type, 0, 0);

        EXPECT_EQ(4, countCellsOfType(piece, type)) << "type=" << type;
        EXPECT_EQ(4, countFilledCells(piece)) << "type=" << type;
    }
}

TEST(PieceTests, PieceOfTypeEmpty_HasNoCells)
{
    Piece piece(EMPTY, 2, 2);

    EXPECT_EQ(0, countFilledCells(piece));
}

TEST(PieceTests, IPiece_OccupiesTheSecondRow)
{
    Piece piece(I, 0, 0);

    for (int column = 0; column < 4; column++)
    {
        EXPECT_EQ(EMPTY, piece.getCell(0, column));
        EXPECT_EQ(I, piece.getCell(1, column));
        EXPECT_EQ(EMPTY, piece.getCell(2, column));
        EXPECT_EQ(EMPTY, piece.getCell(3, column));
    }
}

TEST(PieceTests, TPiece_HasExpectedShape)
{
    Piece piece(T, 0, 0);

    EXPECT_EQ(T, piece.getCell(0, 1));
    EXPECT_EQ(T, piece.getCell(1, 0));
    EXPECT_EQ(T, piece.getCell(1, 1));
    EXPECT_EQ(T, piece.getCell(1, 2));
}

TEST(PieceTests, Move_ChangesPosition)
{
    Piece piece(T, 3, 0);

    piece.move(-1, 2);
    EXPECT_EQ(2, piece.getX());
    EXPECT_EQ(2, piece.getY());

    piece.move(2, -1);
    EXPECT_EQ(4, piece.getX());
    EXPECT_EQ(1, piece.getY());
}

TEST(PieceTests, GetCell_OutsideTheShape_ReturnsEmpty)
{
    Piece piece(I, 0, 0);

    EXPECT_EQ(EMPTY, piece.getCell(-1, 0));
    EXPECT_EQ(EMPTY, piece.getCell(0, -1));
    EXPECT_EQ(EMPTY, piece.getCell(4, 0));
    EXPECT_EQ(EMPTY, piece.getCell(0, 4));
}

TEST(PieceTests, Rotate_IPiece_BecomesVerticalInThirdColumn)
{
    Piece piece(I, 0, 0);

    piece.rotate();

    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            EPieceType expected = (column == 2) ? I : EMPTY;
            EXPECT_EQ(expected, piece.getCell(row, column))
                << "row=" << row << " column=" << column;
        }
    }
}

TEST(PieceTests, Rotate_JPiece_HasExpectedShape)
{
    Piece piece(J, 0, 0);

    piece.rotate();

    EXPECT_EQ(J, piece.getCell(0, 2));
    EXPECT_EQ(J, piece.getCell(0, 3));
    EXPECT_EQ(J, piece.getCell(1, 2));
    EXPECT_EQ(J, piece.getCell(2, 2));
    EXPECT_EQ(4, countFilledCells(piece));
}

TEST(PieceTests, Rotate_KeepsTypePositionAndNumberOfCells)
{
    for (EPieceType type : ALL_PIECE_TYPES)
    {
        Piece piece(type, 4, 7);

        piece.rotate();

        EXPECT_EQ(type, piece.getType()) << "type=" << type;
        EXPECT_EQ(4, piece.getX()) << "type=" << type;
        EXPECT_EQ(7, piece.getY()) << "type=" << type;
        EXPECT_EQ(4, countFilledCells(piece)) << "type=" << type;
    }
}

TEST(PieceTests, RotateFourTimes_RestoresTheOriginalShape)
{
    for (EPieceType type : ALL_PIECE_TYPES)
    {
        Piece original(type, 0, 0);
        Piece rotated(type, 0, 0);

        for (int i = 0; i < 4; i++)
        {
            rotated.rotate();
        }

        for (int row = 0; row < 4; row++)
        {
            for (int column = 0; column < 4; column++)
            {
                EXPECT_EQ(original.getCell(row, column), rotated.getCell(row, column))
                    << "type=" << type << " row=" << row << " column=" << column;
            }
        }
    }
}