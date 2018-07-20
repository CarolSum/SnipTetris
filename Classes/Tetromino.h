#pragma once

#include <array>
#include <memory>
#include "cocos2d.h"
#include "Block.h"
#include "TetrominoGrid.h"

class Tetromino
{
public:
	Tetromino() {}
	COLOR getColor() const { return _color; }
	float getAngle() const { return _angle; }
	virtual void init(TetrominoGrid grid);
protected:
	std::shared_ptr<TetrominoGrid> _grid;
	Block _blocks[4];
	float _angle = 0;
	COLOR _color;

	const int shape[4][2] = { 0,};
	const COLOR DEFAULT_COLOR = COLOR::RED;
};

// http://tetris.wikia.com/wiki/SRS
// rotationVector2 ָ��ת90����λ������
// ��ǰ�Ƕ�Ϊ0��  ʱΪ{ ���Ƹ��������Ƹ��� }
// ��ǰ�Ƕ�Ϊ90�� ʱΪ{ ���Ƹ��������Ƹ��� }
// ��ǰ�Ƕ�Ϊ180��ʱΪ{ ���Ƹ��������Ƹ��� }
// ��ǰ�Ƕ�Ϊ270��ʱΪ{ ���Ƹ��������Ƹ��� }

class TetrominoI : public Tetromino
{
public:
	const COLOR DEFAULT_COLOR = COLOR::SKYBLUE;
	const int shape[4][2] = { { 0, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 } };
	const int rotationVector2[4][2] = { { 2, 1 }, { 1, 0 }, { 0, -1 }, { -1, -2 } };

	TetrominoI() {}
};

class TetrominoJ : public Tetromino
{
public:
	const COLOR DEFAULT_COLOR = COLOR::BLUE;
	const int shape[4][2] = { { 0, 1 },{ 0, 0 },{ 1, 0 },{ 2, 0 } };
	const int rotationVector2[4][2] = { { 2, 0 },{ 1, -1 },{ 0, 0 },{ -1, -1 } };

	TetrominoJ() {}
};

class TetrominoL : public Tetromino
{
public:
	const COLOR DEFAULT_COLOR = COLOR::ORANGE;
	const int shape[4][2] = { { 0, 0 },{ 1, 0 },{ 2, 0 },{ 2, 1 } };
	const int rotationVector2[4][2] = { { 1, -1 },{ 0, 0 },{ -1, -1 },{ 0, -2 } };

	TetrominoL() {}
};

class TetrominoS : public Tetromino
{
public:
	const COLOR DEFAULT_COLOR = COLOR::GREEN;
	const int shape[4][2] = { { 0, 0 },{ 1, 0 },{ 1, 1 },{ 2, 1 } };
	const int rotationVector2[4][2] = { { 1, -1 },{ 0, 0 },{ 1, 1 },{ 0, -2 } };

	TetrominoS() {}
};

class TetrominoZ : public Tetromino
{
public:
	const COLOR DEFAULT_COLOR = COLOR::RED;
	const int shape[4][2] = { { 0, 1 },{ 1, 1 },{ 1, 0 },{ 2, 0 } };
	const int rotationVector2[4][2] = { { 2, 0 },{ 1, 1 },{ 0, 0 },{ -1, 1 } };

	TetrominoZ() {}
};

class TetrominoO : public Tetromino
{
public:
	const COLOR DEFAULT_COLOR = COLOR::YELLOW;
	const int shape[4][2] = { { 0, 1 },{ 1, 1 },{ 0, 0 },{ 1, 0 } };
	const int rotationVector2[4][2] = { { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } };

	TetrominoO() {}
};
