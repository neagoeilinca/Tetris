#include "GameTestFixture.h"
#include "MockGameListener.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::StrictMock;

class GameObserverTest : public GameTest
{
protected:
    // NiceMock: apelurile pentru care nu am scris EXPECT_CALL sunt ignorate
    NiceMock<MockGameListener> listener;

    void SetUp() override
    {
        createGameWithPieces({ I });
    }
};

TEST_F(GameObserverTest, StartNewGame_NotifiesStatusBoardAndScore)
{
    game->registerObserver(&listener);

    EXPECT_CALL(listener, onGameStatusChanged(RUNNING)).Times(1);
    EXPECT_CALL(listener, onBoardChanged()).Times(1);
    EXPECT_CALL(listener, onScoreChanged(0)).Times(1);

    game->startNewGame();
}

TEST_F(GameObserverTest, SuccessfulMove_NotifiesBoardChanged)
{
    game->startNewGame();
    game->registerObserver(&listener);

    EXPECT_CALL(listener, onBoardChanged()).Times(1);

    game->move(LEFT);
}

TEST_F(GameObserverTest, BlockedMove_DoesNotNotify)
{
    game->startNewGame();
    moveTimes(LEFT, 3); // piesa ajunge la peretele stang
    game->registerObserver(&listener);

    EXPECT_CALL(listener, onBoardChanged()).Times(0);

    game->move(LEFT);
}

TEST_F(GameObserverTest, SuccessfulRotation_NotifiesBoardChanged)
{
    game->startNewGame();
    game->registerObserver(&listener);

    EXPECT_CALL(listener, onBoardChanged()).Times(1);

    game->rotate();
}

TEST_F(GameObserverTest, BlockedRotation_DoesNotNotify)
{
    game->startNewGame();
    moveTimes(DOWN, 18);
    game->registerObserver(&listener);

    EXPECT_CALL(listener, onBoardChanged()).Times(0);

    game->rotate();
}

TEST_F(GameObserverTest, Pause_NotifiesPausedThenRunning)
{
    game->startNewGame();
    game->registerObserver(&listener);

    InSequence sequence;
    EXPECT_CALL(listener, onGameStatusChanged(PAUSED)).Times(1);
    EXPECT_CALL(listener, onGameStatusChanged(RUNNING)).Times(1);

    game->pauseGame();
    game->pauseGame();
}

TEST_F(GameObserverTest, ClearingALine_NotifiesTheNewScore)
{
    createGameWithPieces({ I, I, O });
    game->registerObserver(&listener);

    // Prima expectatie accepta orice scor; a doua (declarata ulterior,
    // deci verificata prima) cere ca scorul 100 sa fie anuntat.
    EXPECT_CALL(listener, onScoreChanged(_)).Times(AnyNumber());
    EXPECT_CALL(listener, onScoreChanged(100)).Times(AtLeast(1));

    game->startNewGame();
    clearBottomRow();
}

TEST_F(GameObserverTest, GameOver_NotifiesGameOverStatus)
{
    game->registerObserver(&listener);

    EXPECT_CALL(listener, onGameStatusChanged(_)).Times(AnyNumber());
    EXPECT_CALL(listener, onGameStatusChanged(GAME_OVER)).Times(AtLeast(1));

    game->startNewGame();
    stackPiecesUntilGameOver();
}

TEST_F(GameObserverTest, UnregisteredObserver_IsNoLongerNotified)
{
    StrictMock<MockGameListener> strictListener; // orice apel nedeclarat = eroare
    game->registerObserver(&strictListener);
    game->unregisterObserver(&strictListener);

    game->startNewGame();
    game->move(LEFT);
    game->pauseGame();
}

TEST_F(GameObserverTest, RegisteringTheSameObserverTwice_NotifiesItOnlyOnce)
{
    game->startNewGame();
    game->registerObserver(&listener);
    game->registerObserver(&listener);

    EXPECT_CALL(listener, onBoardChanged()).Times(1);

    game->move(LEFT);
}

TEST_F(GameObserverTest, AllRegisteredObservers_AreNotified)
{
    NiceMock<MockGameListener> secondListener;
    game->startNewGame();
    game->registerObserver(&listener);
    game->registerObserver(&secondListener);

    EXPECT_CALL(listener, onBoardChanged()).Times(1);
    EXPECT_CALL(secondListener, onBoardChanged()).Times(1);

    game->move(LEFT);
}

TEST_F(GameObserverTest, RegisteringNullObserver_IsIgnored)
{
    game->registerObserver(nullptr);

    game->startNewGame();

    EXPECT_EQ(RUNNING, game->getStatus());
}

TEST_F(GameObserverTest, UnregisteringAnUnknownObserver_IsHarmless)
{
    game->unregisterObserver(&listener);

    game->startNewGame();

    EXPECT_EQ(RUNNING, game->getStatus());
}

TEST_F(GameObserverTest, NotStartedGame_IgnoredCommandsDoNotNotify)
{
    StrictMock<MockGameListener> strictListener;
    game->registerObserver(&strictListener);

    game->move(LEFT);
    game->rotate();
    game->tick();
    game->hardDrop();
    game->pauseGame();
}

TEST_F(GameObserverTest, PausedGame_IgnoredCommandsDoNotNotify)
{
    game->startNewGame();
    game->pauseGame();

    StrictMock<MockGameListener> strictListener;
    game->registerObserver(&strictListener);

    game->move(LEFT);
    game->rotate();
    game->tick();
    game->hardDrop();
}

TEST_F(GameObserverTest, GameOver_IgnoredCommandsDoNotNotify)
{
    game->startNewGame();
    stackPiecesUntilGameOver();

    StrictMock<MockGameListener> strictListener;
    game->registerObserver(&strictListener);

    game->move(LEFT);
    game->rotate();
    game->tick();
    game->hardDrop();
    game->pauseGame();
}