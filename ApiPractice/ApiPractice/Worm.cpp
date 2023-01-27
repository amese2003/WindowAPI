#include "pch.h"
#include "Worm.h"


int Worm::GetPosX()
{
	return _posX;
}

int Worm::GetPosY()
{
	return _posY;
}

void Worm::SetPosX(int posX)
{
	_posX = posX;
}

void Worm::SetPosY(int posY)
{
	_posY = posY;
}

int Worm::GetMoveDir()
{
	return _moveDir;
}

void Worm::SetMoveDir(int dir)
{
	if (IsMoving() == false)
		SetMoving(true);

	if (dir == VK_RIGHT)
	{
		_moveDir = 0;
	}
	else if (dir == VK_UP)
	{
		_moveDir = 1;
	}
	else if (dir == VK_LEFT)
	{
		_moveDir = 2;
	}
	else if (dir == VK_DOWN)
	{
		_moveDir = 3;
	}
	else
	{

	}
}

void Worm::SetMoving(bool move)
{
	_moving = move;
}

bool Worm::IsMoving()
{
	return _moving;
}


pair<int, int> Worm::ExpectNext()
{
	return { _posX + DirX[_moveDir], _posY + DirY[_moveDir] };
}

void Worm::MoveNext()
{
	SetPosX(_posX + DirX[_moveDir]);
	SetPosY(_posY + DirY[_moveDir]);
}