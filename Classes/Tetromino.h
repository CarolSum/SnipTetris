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
// rotationVector2 指旋转90°后的位移向量
// 当前角度为0°  时为{ 右移格数，下移格数 }
// 当前角度为90° 时为{ 下移格数，左移格数 }
// 当前角度为180°时为{ 左移格数，上移格数 }
// 当前角度为270°时为{ 上移格数，右移格数 }

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
