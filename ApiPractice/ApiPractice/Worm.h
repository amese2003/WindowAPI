#pragma once
class Worm
{
public:
	Worm() : _radius(10), _size(1), _posX(1), _posY(1), _moveDir(0), _moving(false) {};

public:
	int GetPosX();
	int GetPosY();
	void SetPosX(int posX);
	void SetPosY(int posY);

	int GetMoveDir();
	void SetMoveDir(int dir);
	void SetMoving(bool move);
	bool IsMoving();

	pair<int, int> ExpectNext();
	void MoveNext();

public:
	int DirY[4] = { 0, -1, 0, 1 };
	int DirX[4] = { 1, 0, -1, 0 };

private:
	int _radius;
	int _size;
	int _posX, _posY;
	int _moveDir;
	bool _moving;
};

