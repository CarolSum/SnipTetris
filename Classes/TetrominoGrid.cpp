#include <algorithm>
#include <memory>
#include "TetrominoGrid.h"
#include "Util.h"

using std::all_of;
using std::begin;
using std::end;
using std::make_shared;
using std::shared_ptr;

TetrominoGrid::TetrominoGrid()
{
	assert(ALL_OF_GRID([](const shared_ptr<Block>& p) { return p == nullptr; }));
	// 测试用
	//for (int i = 0; i < MAX_COL; i++)
	//	for (int j = 0; j < MAX_ROW; j++)
	//	{
	//		auto& block = _grid[i][j];
	//		if (!block)
	//		{
	//			block = make_shared<Block>();
	//			block->color = COLOR(UTIL::randomRagne(0, 6));
	//			block->sprite->setSpriteFrame(BlockFramePool::getInstance()->getBlockFrame(block->color));
	//			block->sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//		}
	//	}

	curTetro = nullptr;
	nextTetro = getRandomTetromino();
}

shared_ptr<Block> TetrominoGrid::getBlockOrNull(int cx, int ry)
{
	assert((cx >= 0 && cx < MAX_COL) && (ry >= 0 && ry < MAX_ROW));
	return _grid[cx][ry];
}

bool TetrominoGrid::isOccupied(int cx, int ry)
{
	return getBlockOrNull(cx, ry) != nullptr;
}

bool TetrominoGrid::isAccessible(int cx, int ry)
{
	return (cx >= 0 && cx < MAX_COL) && (ry >= 0 && ry < MAX_ROW) && !isOccupied(cx, ry);
}

void TetrominoGrid::nextRound()
{
	curTetro = nextTetro;
	nextTetro = getRandomTetromino();
	putTetro(curTetro);
}

void TetrominoGrid::putTetro(const shared_ptr<Tetromino>& tetro)
{
	int maxIndex = INT_MIN, minIndex = INT_MAX;
	for (int i = 0; i < 4; i++)
	{
		maxIndex = max(tetro->getShape()[i][0], maxIndex);
		minIndex = min(tetro->getShape()[i][0], minIndex);
	}

	int colStart = (MAX_COL - (maxIndex - minIndex + 1)) / 2;
	int rowStart = MAX_ALIVE_ROW;
	for (int i = 0; i < 4; i++)
	{
		auto& block = tetro->getBlocks()[i];
		auto cx = colStart + tetro->getShape()[i][0];
		auto ry = rowStart + tetro->getShape()[i][1];
		// 插入块
		_grid[cx][ry] = block;
		// 更新正在操作的四格骨牌的坐标数据
		curTetroCoord[i][0] = cx;
		curTetroCoord[i][1] = ry;
	}
}

#define SET_VAR_CX_RY(i, cx, ry) \
int cx = curTetroCoord[i][0]; \
int ry = curTetroCoord[i][1];

#define SET_REF_CX_RY(i, cx, ry) \
int& cx = curTetroCoord[i][0]; \
int& ry = curTetroCoord[i][1];

#define TAKE_UP_TETRO() \
for (int i = 0; i < 4; i++) \
{ \
	SET_VAR_CX_RY(i, cx, ry); \
	_grid[cx][ry] = nullptr; \
}

#define PUT_TETRO_BY_COORD() \
for (int i = 0; i < 4; i++) \
{ \
	auto& block = curTetro->getBlocks()[i]; \
	SET_VAR_CX_RY(i, cx, ry); \
	_grid[cx][ry] = block; \
}

#define PUT_BACK_TETRO PUT_TETRO_BY_COORD

bool TetrominoGrid::canFall()
{
	// 默认可以下落
	bool result = true;
	// 先从Grid上拿走四格骨牌的格子
	TAKE_UP_TETRO();
	// 检查是否存在不能踏足的格子
	for (int i = 0; i < 4; i++)
	{
		SET_VAR_CX_RY(i, cx, ry);
		if (!isAccessible(cx, ry - 1))
		{
			result = false;
			break;
		}
	}
	// 再放回去
	PUT_BACK_TETRO();
	// 返回结果
	return result;
}

bool TetrominoGrid::fall()
{
	if (!canFall()) return false;
	TAKE_UP_TETRO();
	for (int i = 0; i < 4; i++)
	{
		int& ry = curTetroCoord[i][1];
		ry -= 1;
	}
	PUT_TETRO_BY_COORD();
	return true;
}

bool TetrominoGrid::canMove(DIRECTION dir)
{
	// 默认可以移动
	bool result = true;
	// 不同方向移动index偏移不同
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1;
	// 先从Grid上拿走四格骨牌的格子
	TAKE_UP_TETRO();
	// 检查是否存在不能踏足的格子
	for (int i = 0; i < 4; i++)
	{
		SET_VAR_CX_RY(i, cx, ry);
		if (!isAccessible(cx + offset, ry))
		{
			result = false;
			break;
		}
	}
	// 再放回去
	PUT_BACK_TETRO();
	// 返回结果
	return result;
}

bool TetrominoGrid::move(DIRECTION dir)
{
	if (!canMove(dir)) return false;
	TAKE_UP_TETRO();
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1;
	for (int i = 0; i < 4; i++)
	{
		int& cx = curTetroCoord[i][0];
		cx += offset;
	}
	PUT_TETRO_BY_COORD();
	return true;
}

#define SET_CX_RY_AFTER_ROTATION(i, cx, ry, a, b) \
do { \
	int a = curTetro->getRotation()[i][0]; \
	int b = curTetro->getRotation()[i][1]; \
	int angle = curTetro->getAngle(); \
	if (angle == 0) \
	{ \
		cx += a; \
		ry -= b; \
	} \
	else if (angle == 90) \
	{ \
		cx -= b; \
		ry -= a; \
	} \
	else if (angle == 180) \
	{ \
		cx -= a; \
		ry += b; \
	} \
	else if (angle == 270) \
	{ \
		cx += b; \
		ry += a; \
	} \
} while (0);

bool TetrominoGrid::canRotate()
{
	// 默认可以旋转
	bool result = true;
	// 先从Grid上拿走四格骨牌的格子
	TAKE_UP_TETRO();
	// 检查是否存在不能踏足的格子
	for (int i = 0; i < 4; i++)
	{
		// cx, ry都是新临时变量
		SET_VAR_CX_RY(i, cx, ry);
		// { a, b } 定义参考 Tetromino.h
		SET_CX_RY_AFTER_ROTATION(i, cx, ry, a, b);
		if (!isAccessible(cx, ry))
		{
			result = false;
			break;
		}
	}
	// 再放回去
	PUT_BACK_TETRO();
	// 返回结果
	return result;
}

// 可以的话，旋转九十度
bool TetrominoGrid::rotate()
{
	if (!canRotate()) return false;
	TAKE_UP_TETRO();
	for (int i = 0; i < 4; i++)
	{
		SET_REF_CX_RY(i, cx, ry);
		// { a, b } 定义参考 Tetromino.h
		SET_CX_RY_AFTER_ROTATION(i, cx, ry, a, b);
	}
	curTetro->rotate();
	PUT_TETRO_BY_COORD();
	return true;
}

bool TetrominoGrid::hardDrop()
{
	int fallCount = 0;
	while (fall())
		fallCount++;
	return fallCount > 0;
}

bool TetrominoGrid::hasFullRow()
{
	return false;
}

bool TetrominoGrid::isGameOver()
{
	return false;
}

void TetrominoGrid::checkState()
{

}

shared_ptr<Tetromino> TetrominoGrid::getRandomTetromino()
{
	shared_ptr<Tetromino> newTetromino;
	if (_grid != nullptr)
		switch (UTIL::randomRagne(0, 6) & 0)
		{
		case 0: newTetromino = make_shared<TetrominoI>(); break;
		case 1: newTetromino = make_shared<TetrominoJ>(); break;
		case 2: newTetromino = make_shared<TetrominoL>(); break;
		case 3: newTetromino = make_shared<TetrominoO>(); break;
		case 4: newTetromino = make_shared<TetrominoS>(); break;
		case 5: newTetromino = make_shared<TetrominoT>(); break;
		case 6: newTetromino = make_shared<TetrominoZ>(); break;
		}
	return newTetromino;
}
