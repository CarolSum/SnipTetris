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
	// ������
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
		// �����
		_grid[cx][ry] = block;
		// �������ڲ������ĸ���Ƶ���������
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
	// Ĭ�Ͽ�������
	bool result = true;
	// �ȴ�Grid�������ĸ���Ƶĸ���
	TAKE_UP_TETRO();
	// ����Ƿ���ڲ���̤��ĸ���
	for (int i = 0; i < 4; i++)
	{
		SET_VAR_CX_RY(i, cx, ry);
		if (!isAccessible(cx, ry - 1))
		{
			result = false;
			break;
		}
	}
	// �ٷŻ�ȥ
	PUT_BACK_TETRO();
	// ���ؽ��
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
	// Ĭ�Ͽ����ƶ�
	bool result = true;
	// ��ͬ�����ƶ�indexƫ�Ʋ�ͬ
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1;
	// �ȴ�Grid�������ĸ���Ƶĸ���
	TAKE_UP_TETRO();
	// ����Ƿ���ڲ���̤��ĸ���
	for (int i = 0; i < 4; i++)
	{
		SET_VAR_CX_RY(i, cx, ry);
		if (!isAccessible(cx + offset, ry))
		{
			result = false;
			break;
		}
	}
	// �ٷŻ�ȥ
	PUT_BACK_TETRO();
	// ���ؽ��
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
	// Ĭ�Ͽ�����ת
	bool result = true;
	// �ȴ�Grid�������ĸ���Ƶĸ���
	TAKE_UP_TETRO();
	// ����Ƿ���ڲ���̤��ĸ���
	for (int i = 0; i < 4; i++)
	{
		// cx, ry��������ʱ����
		SET_VAR_CX_RY(i, cx, ry);
		// { a, b } ����ο� Tetromino.h
		SET_CX_RY_AFTER_ROTATION(i, cx, ry, a, b);
		if (!isAccessible(cx, ry))
		{
			result = false;
			break;
		}
	}
	// �ٷŻ�ȥ
	PUT_BACK_TETRO();
	// ���ؽ��
	return result;
}

// ���ԵĻ�����ת��ʮ��
bool TetrominoGrid::rotate()
{
	if (!canRotate()) return false;
	TAKE_UP_TETRO();
	for (int i = 0; i < 4; i++)
	{
		SET_REF_CX_RY(i, cx, ry);
		// { a, b } ����ο� Tetromino.h
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
