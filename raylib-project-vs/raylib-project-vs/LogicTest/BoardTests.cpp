#include <gtest/gtest.h>

#include "Board.h"
#include "Piece.h"

namespace
{
    // Umple complet un rand folosind trei piese I orizontale
    // (coloanele 0-3, 4-7 si 8-9; celulele din afara tablei sunt ignorate).
    // Piesa I are celulele pe al doilea rand al formei, deci y = row - 1.
    void fillRow(Board& board, int row)
    {
        board.mergePiece(Piece(I, 0, row - 1));
        board.mergePiece(Piece(I, 4, row - 1));
        board.mergePiece(Piece(I, 8, row - 1));
    }

    int countFilledCells(const Board& board)
    {
        int count = 0;

        for (int row = 0; row < 20; row++)
        {
            for (int column = 0; column < 10; column++)
            {
                if (board.getGrid()[row][column] != EMPTY)
                {
                    count++;
                }
            }
        }

        return count;
    }

    bool isRowEmpty(const Board& board, int row)
    {
        for (int column = 0; column < 10; column++)
        {
            if (board.getGrid()[row][column] != EMPTY)
            {
                return false;
            }
        }

        return true;
    }
}

TEST(BoardTests, NewBoard_IsEmpty)
{
    Board board;

    EXPECT_EQ(0, countFilledCells(board));
}

TEST(BoardTests, CheckCollision_FreePosition_ReturnsFalse)
{
    Board board;
    Piece piece(I, 0, 0);

    EXPECT_FALSE(board.checkCollision(piece, 3, 0));
}

TEST(BoardTests, CheckCollision_LeftWall)
{
    Board board;
    Piece piece(I, 0, 0);

    EXPECT_FALSE(board.checkCollision(piece, 0, 5));
    EXPECT_TRUE(board.checkCollision(piece, -1, 5));
}

TEST(BoardTests, CheckCollision_RightWall)
{
    Board board;
    Piece piece(I, 0, 0);

    EXPECT_FALSE(board.checkCollision(piece, 6, 5));
    EXPECT_TRUE(board.checkCollision(piece, 7, 5));
}

TEST(BoardTests, CheckCollision_Floor)
{
    Board board;
    Piece piece(I, 0, 0);

    EXPECT_FALSE(board.checkCollision(piece, 3, 18));
    EXPECT_TRUE(board.checkCollision(piece, 3, 19));
}

TEST(BoardTests, CheckCollision_Ceiling)
{
    Board board;
    Piece piece(I, 0, 0);

    EXPECT_FALSE(board.checkCollision(piece, 3, -1));
    EXPECT_TRUE(board.checkCollision(piece, 3, -2));
}

TEST(BoardTests, CheckCollision_WithOccupiedCells)
{
    Board board;
    board.mergePiece(Piece(I, 0, 18)); // ocupa randul 19, coloanele 0-3

    Piece piece(I, 0, 0);

    EXPECT_TRUE(board.checkCollision(piece, 0, 18));
    EXPECT_TRUE(board.checkCollision(piece, 3, 18));
    EXPECT_FALSE(board.checkCollision(piece, 4, 18));
    EXPECT_FALSE(board.checkCollision(piece, 0, 17));
}

TEST(BoardTests, MergePiece_WritesTheCellsOfThePiece)
{
    Board board;

    board.mergePiece(Piece(T, 3, 10));

    EXPECT_EQ(T, board.getGrid()[10][4]);
    EXPECT_EQ(T, board.getGrid()[11][3]);
    EXPECT_EQ(T, board.getGrid()[11][4]);
    EXPECT_EQ(T, board.getGrid()[11][5]);
    EXPECT_EQ(4, countFilledCells(board));
}

TEST(BoardTests, MergePiece_IgnoresCellsOutsideTheBoard)
{
    Board board;

    board.mergePiece(Piece(I, 8, 18)); // coloanele 10 si 11 nu exista

    EXPECT_EQ(I, board.getGrid()[19][8]);
    EXPECT_EQ(I, board.getGrid()[19][9]);
    EXPECT_EQ(2, countFilledCells(board));
}

TEST(BoardTests, Reset_ClearsTheBoard)
{
    Board board;
    fillRow(board, 19);

    board.reset();

    EXPECT_EQ(0, countFilledCells(board));
}

TEST(BoardTests, ClearLines_EmptyBoard_ReturnsZero)
{
    Board board;

    EXPECT_EQ(0, board.clearLines());
}

TEST(BoardTests, ClearLines_IncompleteRow_IsNotCleared)
{
    Board board;
    board.mergePiece(Piece(I, 0, 18));
    board.mergePiece(Piece(I, 4, 18)); // randul 19 are doar coloanele 0-7

    EXPECT_EQ(0, board.clearLines());
    EXPECT_EQ(8, countFilledCells(board));
}

TEST(BoardTests, ClearLines_FullBottomRow_ReturnsOneAndEmptiesTheRow)
{
    Board board;
    fillRow(board, 19);

    EXPECT_EQ(1, board.clearLines());
    EXPECT_EQ(0, countFilledCells(board));
}

TEST(BoardTests, ClearLines_RowsAboveMoveDown)
{
    Board board;
    fillRow(board, 19);
    board.mergePiece(Piece(I, 0, 17)); // randul 18, coloanele 0-3

    EXPECT_EQ(1, board.clearLines());

    for (int column = 0; column < 4; column++)
    {
        EXPECT_EQ(I, board.getGrid()[19][column]);
    }
    EXPECT_EQ(EMPTY, board.getGrid()[19][4]);
    EXPECT_TRUE(isRowEmpty(board, 18));
    EXPECT_EQ(4, countFilledCells(board));
}

TEST(BoardTests, ClearLines_FullMiddleRow_OnlyRowsAboveMove)
{
    Board board;
    fillRow(board, 10);
    board.mergePiece(Piece(I, 0, 8));  // randul 9, deasupra liniei complete
    board.mergePiece(Piece(I, 0, 10)); // randul 11, sub linia completa

    EXPECT_EQ(1, board.clearLines());

    EXPECT_EQ(I, board.getGrid()[11][0]); // randul de jos ramane pe loc
    EXPECT_EQ(I, board.getGrid()[10][0]); // randul de sus a coborat
    EXPECT_EQ(EMPTY, board.getGrid()[10][4]);
    EXPECT_TRUE(isRowEmpty(board, 9));
    EXPECT_EQ(8, countFilledCells(board));
}

TEST(BoardTests, ClearLines_TwoAdjacentFullRows_ReturnsTwo)
{
    Board board;
    fillRow(board, 18);
    fillRow(board, 19);

    EXPECT_EQ(2, board.clearLines());
    EXPECT_EQ(0, countFilledCells(board));
}

TEST(BoardTests, ClearLines_TwoSeparatedFullRows_ReturnsTwo)
{
    Board board;
    fillRow(board, 19);
    fillRow(board, 17);
    board.mergePiece(Piece(I, 0, 17)); // randul 18 (incomplet) intre ele

    EXPECT_EQ(2, board.clearLines());

    for (int column = 0; column < 4; column++)
    {
        EXPECT_EQ(I, board.getGrid()[19][column]);
    }
    EXPECT_EQ(4, countFilledCells(board));
}

TEST(BoardTests, ClearLines_FourFullRows_ReturnsFour)
{
    Board board;
    for (int row = 16; row <= 19; row++)
    {
        fillRow(board, row);
    }

    EXPECT_EQ(4, board.clearLines());
    EXPECT_EQ(0, countFilledCells(board));
}

TEST(BoardTests, ClearLines_TopRow_IsCleared)
{
    Board board;
    fillRow(board, 0);

    EXPECT_EQ(1, board.clearLines());
    EXPECT_EQ(0, countFilledCells(board));
}