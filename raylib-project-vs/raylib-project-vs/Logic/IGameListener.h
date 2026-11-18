#pragma once
__interface IGameListener
{
public:
	void onGameStatusChanged(EGameStatus status);
	void onBoardChanged();
	void onScoreChanged(int score);
};