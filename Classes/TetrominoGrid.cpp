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

#define TAKE_UP_TETRO() \
for (int i = 0; i < 4; i++) \
{ \
	SET_VAR_CX_RY(i, cx, ry); \
	_grid[cx][ry] = nullptr; \
}

#define PUT_BACK_TETRO() \
for (int i = 0; i < 4; i++) \
{ \
	auto& block = curTetro->getBlocks()[i]; \
	SET_VAR_CX_RY(i, cx, ry); \
	_grid[cx][ry] = block; \
}

bool TetrominoGrid::fall()
{
	// 先从Grid上拿走四格骨牌的格子
	TAKE_UP_TETRO();

	// 检查可行性
	for (int i = 0; i < 4; i++)
	{
		SET_VAR_CX_RY(i, cx, ry);
		if (!isAccessible(cx, ry - 1))
			return false;
	}

	// 执行
	for (int i = 0; i < 4; i++)
	{
		int& ry = curTetroCoord[i][1];
		ry -= 1;
	}

	// 再放回去
	PUT_BACK_TETRO();

	return true;
}

bool TetrominoGrid::move(DIRECTION dir)
{
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1;

	// 先从Grid上拿走四格骨牌的格子
	TAKE_UP_TETRO();

	// 检查可行性
	for (int i = 0; i < 4; i++)
	{
		SET_VAR_CX_RY(i, cx, ry);
		if (!isAccessible(cx + offset, ry))
			return false;
	}

	// 执行
	for (int i = 0; i < 4; i++)
	{
		int& cx = curTetroCoord[i][0];
		cx += offset;
	}

	// 再放回去
	PUT_BACK_TETRO();

	return true;
}

bool TetrominoGrid::rotate()
{
	return false;
}

bool TetrominoGrid::hardDrop()
{
	return false;
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
		switch (UTIL::randomRagne(0, 6))
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
