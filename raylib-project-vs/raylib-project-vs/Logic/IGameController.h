
#pragma once
#include "EDirection.h"
#include "EGameStatus.h"
#include "EPieceType.h"
#include "IGameListener.h"

__interface IGameController
{
public:
	void startNewGame();
	void rotate();
	void pauseGame();
	void move(EDirection direction);
	void registerObserver(IGameListener* listener);
	void unregisterObserver(IGameListener* listener);
	void tick();
	int getScore();
	EGameStatus getStatus();
	int getRowCount();
	int getColumnCount();
	EPieceType getCellType(int row, int column);
};

