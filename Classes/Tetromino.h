#pragma once

#include <array>
#include <memory>
#include "cocos2d.h"
#include "Block.h"

using std::shared_ptr;
using std::make_shared;

class TetrominoGrid;

class Tetromino
{
public:
//	Tetromino(const std::shared_ptr<TetrominoGrid>& grid) : _grid(grid) {}
	Tetromino() = default;
	float getAngle() const { return _angle; }
protected:
//	shared_ptr<TetrominoGrid> _grid;
	shared_ptr<Block>  _blocks[4];
	float _angle = 0;
};

#define TETROMINO_CONSTRUCTOR(_T) \
_T()/* : Tetromino(grid) */\
{ \
	for (int i = 0; i < 4; i++) \
	{ \
		_blocks[i] = make_shared<Block>(); \
		_blocks[i]->color = _T::DEFAULT_COLOR; \
	} \
}

// http://tetris.wikia.com/wiki/SRS
// rotationVector2 ָ��ת90����λ������
// ��ǰ�Ƕ�Ϊ0��  ʱΪ{ ���Ƹ��������Ƹ��� }
// ��ǰ�Ƕ�Ϊ90�� ʱΪ{ ���Ƹ��������Ƹ��� }
// ��ǰ�Ƕ�Ϊ180��ʱΪ{ ���Ƹ��������Ƹ��� }
// ��ǰ�Ƕ�Ϊ270��ʱΪ{ ���Ƹ��������Ƹ��� }

class TetrominoI : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::SKYBLUE;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_CONSTRUCTOR(TetrominoI)
};

class TetrominoJ : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::BLUE;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_CONSTRUCTOR(TetrominoJ)
};

class TetrominoL : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::ORANGE;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_CONSTRUCTOR(TetrominoL)
};

class TetrominoO : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::YELLOW;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_CONSTRUCTOR(TetrominoO)
};

class TetrominoS : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::GREEN;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_CONSTRUCTOR(TetrominoS)
};

class TetrominoT : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::PINK;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_CONSTRUCTOR(TetrominoT)
};

class TetrominoZ : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::RED;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_CONSTRUCTOR(TetrominoZ)
};
