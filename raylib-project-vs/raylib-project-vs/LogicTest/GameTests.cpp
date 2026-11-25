#include "GameTestFixture.h"

// ---------------------------------------------------------------
// Stare initiala
// ---------------------------------------------------------------

TEST_F(GameTest, NewGame_IsNotStartedWithZeroScore)
{
    createGameWithPieces({ I });

    EXPECT_EQ(NOT_STARTED, game->getStatus());
    EXPECT_EQ(0, game->getScore());
    EXPECT_EQ(0, countFilledCells());
}

TEST_F(GameTest, BoardSize_IsTwentyRowsByTenColumns)
{
    createGameWithPieces({ I });

    EXPECT_EQ(20, game->getRowCount());
    EXPECT_EQ(10, game->getColumnCount());
}

TEST_F(GameTest, GetCellType_OutsideTheBoard_ReturnsEmpty)
{
    createGameWithPieces({ I });
    game->startNewGame();

    EXPECT_EQ(EMPTY, game->getCellType(-1, 3));
    EXPECT_EQ(EMPTY, game->getCellType(1, -1));
    EXPECT_EQ(EMPTY, game->getCellType(20, 3));
    EXPECT_EQ(EMPTY, game->getCellType(1, 10));
}

TEST_F(GameTest, NotStartedGame_IgnoresAllCommands)
{
    createGameWithPieces({ I });

    game->move(LEFT);
    game->rotate();
    game->tick();
    game->hardDrop();
    game->pauseGame();

    EXPECT_EQ(NOT_STARTED, game->getStatus());
    EXPECT_EQ(0, countFilledCells());
}

// ---------------------------------------------------------------
// Pornirea jocului
// ---------------------------------------------------------------

TEST_F(GameTest, StartNewGame_SpawnsPieceAtTopMiddle)
{
    createGameWithPieces({ I });

    game->startNewGame();

    EXPECT_EQ(RUNNING, game->getStatus());
    EXPECT_EQ(0, game->getScore());
    expectRowFilled(1, 3, 6, I);
    EXPECT_EQ(4, countFilledCells());
}

TEST(GameWithRandomPiecesTest, StartNewGame_SpawnsAPieceMadeOfFourCells)
{
    Game realGame; // constructorul implicit foloseste generatorul aleatoriu

    realGame.startNewGame();

    int filledCells = 0;
    for (int row = 0; row < realGame.getRowCount(); row++)
    {
        for (int column = 0; column < realGame.getColumnCount(); column++)
        {
            if (realGame.getCellType(row, column) != EMPTY)
            {
                filledCells++;
            }
        }
    }

    EXPECT_EQ(RUNNING, realGame.getStatus());
    EXPECT_EQ(4, filledCells);
}

TEST_F(GameTest, StartNewGame_ResetsScoreAndBoard)
{
    createGameWithPieces({ I, I, O });
    game->startNewGame();
    clearBottomRow();
    ASSERT_EQ(100, game->getScore());

    game->startNewGame();

    EXPECT_EQ(0, game->getScore());
    EXPECT_EQ(RUNNING, game->getStatus());
    EXPECT_EQ(EMPTY, game->getCellType(19, 8));
    EXPECT_EQ(4, countFilledCells()); // doar piesa noua
}

// ---------------------------------------------------------------
// Miscare
// ---------------------------------------------------------------

TEST_F(GameTest, MoveLeft_ShiftsPieceOneColumn)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->move(LEFT);

    expectRowFilled(1, 2, 5, I);
    EXPECT_EQ(EMPTY, game->getCellType(1, 6));
}

TEST_F(GameTest, MoveRight_ShiftsPieceOneColumn)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->move(RIGHT);

    expectRowFilled(1, 4, 7, I);
    EXPECT_EQ(EMPTY, game->getCellType(1, 3));
}

TEST_F(GameTest, MoveDown_ShiftsPieceOneRow)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->move(DOWN);

    expectRowFilled(2, 3, 6, I);
    EXPECT_EQ(EMPTY, game->getCellType(1, 3));
}

TEST_F(GameTest, MoveLeft_StopsAtLeftWall)
{
    createGameWithPieces({ I });
    game->startNewGame();

    moveTimes(LEFT, 10);

    expectRowFilled(1, 0, 3, I);
    EXPECT_EQ(EMPTY, game->getCellType(1, 4));
}

TEST_F(GameTest, MoveRight_StopsAtRightWall)
{
    createGameWithPieces({ I });
    game->startNewGame();

    moveTimes(RIGHT, 10);

    expectRowFilled(1, 6, 9, I);
    EXPECT_EQ(EMPTY, game->getCellType(1, 5));
}

// ---------------------------------------------------------------
// Rotire
// ---------------------------------------------------------------

TEST_F(GameTest, Rotate_TurnsHorizontalIPieceIntoVertical)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->rotate();

    for (int row = 0; row < 4; row++)
    {
        EXPECT_EQ(I, game->getCellType(row, 5)) << "row=" << row;
    }
    EXPECT_EQ(EMPTY, game->getCellType(1, 3));
    EXPECT_EQ(4, countFilledCells());
}

TEST_F(GameTest, Rotate_IsBlockedWhenThereIsNoRoom)
{
    createGameWithPieces({ I });
    game->startNewGame();
    moveTimes(DOWN, 18); // piesa ajunge pe ultimul rand

    game->rotate();

    expectRowFilled(19, 3, 6, I);
    EXPECT_EQ(EMPTY, game->getCellType(18, 5));
    EXPECT_EQ(4, countFilledCells());
}

// ---------------------------------------------------------------
// Tick si fixarea pieselor
// ---------------------------------------------------------------

TEST_F(GameTest, Tick_MovesPieceDownOneRow)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->tick();

    expectRowFilled(2, 3, 6, I);
    EXPECT_EQ(EMPTY, game->getCellType(1, 3));
}

TEST_F(GameTest, Tick_WhenPieceCannotMoveDown_LocksItAndSpawnsANewOne)
{
    createGameWithPieces({ I });
    game->startNewGame();

    for (int i = 0; i < 19; i++)
    {
        game->tick();
    }

    expectRowFilled(19, 3, 6, I); // piesa fixata
    expectRowFilled(1, 3, 6, I);  // piesa noua
    EXPECT_EQ(8, countFilledCells());
}

TEST_F(GameTest, HardDrop_PlacesPieceOnTheBottomAndSpawnsTheNextPiece)
{
    createGameWithPieces({ I, O });
    game->startNewGame();

    game->hardDrop();

    expectRowFilled(19, 3, 6, I);
    EXPECT_EQ(O, game->getCellType(0, 4));
    EXPECT_EQ(O, game->getCellType(0, 5));
    EXPECT_EQ(O, game->getCellType(1, 4));
    EXPECT_EQ(O, game->getCellType(1, 5));
    EXPECT_EQ(8, countFilledCells());
}

TEST_F(GameTest, HardDrop_StopsOnTopOfLockedPieces)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->hardDrop();
    game->hardDrop();

    expectRowFilled(19, 3, 6, I);
    expectRowFilled(18, 3, 6, I);
}

// ---------------------------------------------------------------
// Pauza
// ---------------------------------------------------------------

TEST_F(GameTest, Pause_ChangesStatusAndKeepsThePieceVisible)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->pauseGame();

    EXPECT_EQ(PAUSED, game->getStatus());
    expectRowFilled(1, 3, 6, I);
}

TEST_F(GameTest, PauseTwice_ResumesTheGame)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->pauseGame();
    game->pauseGame();

    EXPECT_EQ(RUNNING, game->getStatus());
}

TEST_F(GameTest, PausedGame_IgnoresMovesRotationsTicksAndHardDrops)
{
    createGameWithPieces({ I });
    game->startNewGame();
    game->pauseGame();

    game->move(LEFT);
    game->move(DOWN);
    game->rotate();
    game->tick();
    game->hardDrop();

    EXPECT_EQ(PAUSED, game->getStatus());
    expectRowFilled(1, 3, 6, I);
    EXPECT_EQ(4, countFilledCells());
}

// ---------------------------------------------------------------
// Scor
// ---------------------------------------------------------------

TEST_F(GameTest, LockingAPieceWithoutCompletingALine_KeepsScoreAtZero)
{
    createGameWithPieces({ I });
    game->startNewGame();

    game->hardDrop();

    EXPECT_EQ(0, game->getScore());
}

TEST_F(GameTest, ClearingOneLine_AddsOneHundredPoints)
{
    createGameWithPieces({ I, I, O });
    game->startNewGame();

    clearBottomRow();

    EXPECT_EQ(100, game->getScore());
    EXPECT_EQ(RUNNING, game->getStatus());

    // Din piesa O ramane doar jumatatea de sus, coborata pe ultimul rand
    EXPECT_EQ(O, game->getCellType(19, 8));
    EXPECT_EQ(O, game->getCellType(19, 9));
    EXPECT_EQ(EMPTY, game->getCellType(19, 0));
    EXPECT_EQ(EMPTY, game->getCellType(19, 4));
    EXPECT_EQ(EMPTY, game->getCellType(18, 8));

    // 2 celule ramase + piesa noua (I)
    EXPECT_EQ(6, countFilledCells());
}

TEST_F(GameTest, ClearingTwoLinesAtOnce_AddsThreeHundredPoints)
{
    createGameWithPieces({ O });
    game->startNewGame();

    // Cinci piese O (cate 2 coloane) umplu randurile 18 si 19.
    // Ultima piesa completeaza ambele randuri deodata.
    moveHorizontallyThenHardDrop(-4); // coloanele 0-1
    moveHorizontallyThenHardDrop(-2); // coloanele 2-3
    moveHorizontallyThenHardDrop(0);  // coloanele 4-5
    moveHorizontallyThenHardDrop(2);  // coloanele 6-7
    ASSERT_EQ(0, game->getScore());
    moveHorizontallyThenHardDrop(4);  // coloanele 8-9

    EXPECT_EQ(300, game->getScore());
    EXPECT_EQ(4, countFilledCells()); // tabla goala + piesa noua
}

TEST_F(GameTest, ClearingThreeLinesAtOnce_AddsFiveHundredPoints)
{
    // Un J asezat vertical in coloana 1 (cu "piciorul" in coloana 0) acopera
    // randurile 17-19 in coloana 1, iar un I vertical in coloana 0 se opreste
    // deasupra piciorului. Astfel randul 16 ramane gol in coloana 1, deci
    // ultimul I (coloana 9) completeaza exact 3 randuri: 17, 18 si 19.
    createGameWithPieces({ J, I, I, I, I, I, I, I, I, I });
    game->startNewGame();

    game->rotate();
    game->rotate();
    game->rotate();
    moveHorizontally(-3);
    game->hardDrop();

    dropVerticalIPieceInColumn(0);
    for (int column = 2; column <= 9; column++)
    {
        dropVerticalIPieceInColumn(column);
    }

    EXPECT_EQ(500, game->getScore());
    EXPECT_EQ(RUNNING, game->getStatus());

    // Raman: 9 celule pe ultimul rand, 1 celula pe penultimul + piesa noua
    EXPECT_EQ(14, countFilledCells());
}

TEST_F(GameTest, ClearingFourLinesAtOnce_AddsEightHundredPoints)
{
    createGameWithPieces({ I });
    game->startNewGame();

    for (int column = 0; column < 10; column++)
    {
        dropVerticalIPieceInColumn(column);
    }

    EXPECT_EQ(800, game->getScore());
    EXPECT_EQ(4, countFilledCells()); // tabla goala + piesa noua
}

TEST_F(GameTest, ScoreAccumulatesAcrossMultipleClears)
{
    createGameWithPieces({ I, I, O });
    game->startNewGame();

    clearBottomRow();
    ASSERT_EQ(100, game->getScore());

    // Dupa prima eliminare, tabla contine 2 celule O pe ultimul rand
    // (coloanele 8-9) si urmeaza un nou ciclu I, I, O.
    // Un singur I orizontal pe coloanele 0-3 si altul pe 4-7 completeaza
    // ultimul rand fara piesa O (celulele din 8-9 exista deja).
    moveHorizontallyThenHardDrop(-3);
    moveHorizontallyThenHardDrop(1);

    EXPECT_EQ(200, game->getScore());
}

// ---------------------------------------------------------------
// Game over
// ---------------------------------------------------------------

TEST_F(GameTest, StackingPiecesUpToTheTop_EndsTheGame)
{
    createGameWithPieces({ I });
    game->startNewGame();

    for (int i = 0; i < 18; i++)
    {
        game->hardDrop();
    }
    EXPECT_EQ(RUNNING, game->getStatus());

    game->hardDrop();

    EXPECT_EQ(GAME_OVER, game->getStatus());
}

TEST_F(GameTest, AfterGameOver_NewGameCanBeStarted)
{
    createGameWithPieces({ I });
    game->startNewGame();
    stackPiecesUntilGameOver();
    ASSERT_EQ(GAME_OVER, game->getStatus());

    game->startNewGame();

    EXPECT_EQ(RUNNING, game->getStatus());
    EXPECT_EQ(0, game->getScore());
    EXPECT_EQ(4, countFilledCells());
}