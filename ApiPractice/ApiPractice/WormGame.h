#pragma once

class Node;

class WormGame
{
public:
	WormGame() : _boardSize(100) {}
	~WormGame();

public:
	void GenerateBoard();
	bool CanPass(int y, int x);

private:
	int _boardSize;
};

