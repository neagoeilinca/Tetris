
#pragma once
#include "IGameController.h"
#include "Board.h"
#include <list>
class Game
{
private:
	Board board;
	Piece current_piece;
	EGameStatus status;
	int score;
	std::list<IGameListener*> listeners;
public:
	void notifyObservers();
	void Tick(); //implementeaza Tick() din IGameController

};

