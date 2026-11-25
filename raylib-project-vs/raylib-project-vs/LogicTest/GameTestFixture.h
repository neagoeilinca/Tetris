#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "Game.h"
#include "MockPieceTypeGenerator.h"

class GameTest : public ::testing::Test
{
protected:
    std::unique_ptr<Game> game;

    // Creeaza un joc care primeste piesele in ordinea data, ciclic.
    // Exemplu: {I, O} => I, O, I, O, ...
    void createGameWithPieces(const std::vector<EPieceType>& pieces)
    {
        pieceSequence = pieces;
        nextPieceIndex = 0;

        auto generator = std::make_unique<::testing::NiceMock<MockPieceTypeGenerator>>();

        ON_CALL(*generator, nextPieceType())
            .WillByDefault(::testing::Invoke([this]() { return takeNextPiece(); }));

        game = std::make_unique<Game>(std::move(generator));
    }

    void moveTimes(EDirection direction, int times)
    {
        for (int i = 0; i < times; i++)
        {
            game->move(direction);
        }
    }

    // Valoare negativa = stanga, pozitiva = dreapta
    void moveHorizontally(int columns)
    {
        if (columns < 0)
        {
            moveTimes(LEFT, -columns);
        }
        else
        {
            moveTimes(RIGHT, columns);
        }
    }

    void moveHorizontallyThenHardDrop(int columns)
    {
        moveHorizontally(columns);
        game->hardDrop();
    }

    // Necesita secventa de piese {I, I, O}: doua I orizontale (coloanele 0-3 si 4-7)
    // si un O (coloanele 8-9) completeaza linia de jos => 1 linie eliminata.
    void clearBottomRow()
    {
        moveHorizontallyThenHardDrop(-3);
        moveHorizontallyThenHardDrop(1);
        moveHorizontallyThenHardDrop(4);
    }

    // Necesita piese de tip I. Roteste piesa (devine verticala, pe a treia coloana
    // a formei), o duce pe coloana ceruta si o lasa sa cada.
    void dropVerticalIPieceInColumn(int column)
    {
        game->rotate();
        moveHorizontally(column - 5);
        game->hardDrop();
    }

    // Necesita piese de tip I. Dupa 19 piese asezate una peste alta,
    // piesa noua nu mai are loc la aparitie => GAME_OVER.
    void stackPiecesUntilGameOver()
    {
        for (int i = 0; i < 19; i++)
        {
            game->hardDrop();
        }
    }

    int countFilledCells() const
    {
        int count = 0;

        for (int row = 0; row < game->getRowCount(); row++)
        {
            for (int column = 0; column < game->getColumnCount(); column++)
            {
                if (game->getCellType(row, column) != EMPTY)
                {
                    count++;
                }
            }
        }

        return count;
    }

    void expectRowFilled(int row, int firstColumn, int lastColumn, EPieceType type) const
    {
        for (int column = firstColumn; column <= lastColumn; column++)
        {
            EXPECT_EQ(type, game->getCellType(row, column))
                << "row=" << row << " column=" << column;
        }
    }

private:
    std::vector<EPieceType> pieceSequence;
    std::size_t nextPieceIndex = 0;

    EPieceType takeNextPiece()
    {
        EPieceType type = pieceSequence[nextPieceIndex % pieceSequence.size()];
        nextPieceIndex++;
        return type;
    }
};