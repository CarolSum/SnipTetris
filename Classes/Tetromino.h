#pragma once

#include <array>
#include <memory>
#include "Block.h"
#include "cocos2d.h"
USING_NS_CC;

using std::shared_ptr;
using std::make_shared;

class TetrominoGrid;

class Tetromino
{
public:
//	Tetromino(const std::shared_ptr<TetrominoGrid>& grid) : _grid(grid) {}
	Tetromino() = default;
	const shared_ptr<Block> *getBlocks() const { return _blocks; }
	float getAngle() const { return _angle; }
	float setAngle(float angle) { _angle = angle; }
	virtual COLOR getColor() const = 0;
	virtual const int(*getShape())[2] = 0;
	virtual const int(*getRotation())[2] = 0;
protected:
	shared_ptr<Block> _blocks[4];
	float _angle = 0;
};

#define TETROMINO_CONSTRUCTOR(_T) \
_T()/* : Tetromino(grid) */\
{ \
	for (int i = 0; i < 4; i++) \
	{ \
		_blocks[i] = make_shared<Block>(); \
		_blocks[i]->color = _T::DEFAULT_COLOR; \
		_blocks[i]->sprite->setSpriteFrame(BlockFramePool::getInstance()->getBlockFrame(_T::DEFAULT_COLOR)); \
		_blocks[i]->sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT); \
	} \
}

#define TETROMINO_GET_COLOR(_T) \
COLOR getColor() const { return _T::DEFAULT_COLOR; }

#define TETROMINO_GET_SHAPE(_T) \
const int (*getShape())[2] { return _T::shape; }

#define TETROMINO_GET_ROTATION(_T) \
const int (*getRotation())[2] { return _T::rotationVector2; }

#define TETROMINO_IMPLEMENT(_T) \
TETROMINO_CONSTRUCTOR(_T) \
TETROMINO_GET_COLOR(_T) \
TETROMINO_GET_SHAPE(_T) \
TETROMINO_GET_ROTATION(_T)


// http://tetris.wikia.com/wiki/SRS
// rotationVector2 指旋转90°后的位移向量
// 当前角度为0°  时为{ 右移格数，下移格数 }
// 当前角度为90° 时为{ 下移格数，左移格数 }
// 当前角度为180°时为{ 左移格数，上移格数 }
// 当前角度为270°时为{ 上移格数，右移格数 }

class TetrominoI : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::SKYBLUE;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_IMPLEMENT(TetrominoI)
};

class TetrominoJ : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::BLUE;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_IMPLEMENT(TetrominoJ)
};

class TetrominoL : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::ORANGE;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_IMPLEMENT(TetrominoL)
};

class TetrominoO : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::YELLOW;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_IMPLEMENT(TetrominoO)
};

class TetrominoS : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::GREEN;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_IMPLEMENT(TetrominoS)
};

class TetrominoT : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::PINK;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_IMPLEMENT(TetrominoT)
};

class TetrominoZ : public Tetromino
{
public:
	static const COLOR DEFAULT_COLOR = COLOR::RED;
	static const int shape[4][2];
	static const int rotationVector2[4][2];

	TETROMINO_IMPLEMENT(TetrominoZ)
};
