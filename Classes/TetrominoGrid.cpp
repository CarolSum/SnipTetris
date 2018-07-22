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
	curTetroP1 = nullptr;
	curTetroP2 = nullptr;
	nextTetroP1 = getRandomTetromino();
	nextTetroP2 = getRandomTetromino();
}

// ��ȡ����ָ�����Ϊ��
shared_ptr<Block> TetrominoGrid::getBlockOrNull(int cx, int ry)
{
	assert((cx >= 0 && cx < MAX_COL) && (ry >= 0 && ry < MAX_ROW));
	return _grid[cx][ry];
}

// �ж�ĳ�����Ƿ����и���ռ��
bool TetrominoGrid::isOccupied(int cx, int ry)
{
	return getBlockOrNull(cx, ry) != nullptr;
}

// �ж�ĳ�����ܷ���ø���
bool TetrominoGrid::isAccessible(int cx, int ry)
{
	return (cx >= 0 && cx < MAX_COL) && (ry >= 0 && ry < MAX_ROW) && !isOccupied(cx, ry);
}

// һ������P1���ĸ�������ʱ��ִ����һ��
void TetrominoGrid::nextRoundP1()
{
	curTetroP1 = nextTetroP1;
	nextTetroP1 = getRandomTetromino();
	putTetroP1(curTetroP1);
}

// һ������P2���ĸ�������ʱ��ִ����һ��
void TetrominoGrid::nextRoundP2()
{
	curTetroP2 = nextTetroP2;
	nextTetroP2 = getRandomTetromino();
	putTetroP2(curTetroP2);
}

// ����һ��Player�����ĸ���Ƶ�Grid������һ�������µ�Player��һ�ֿ�ʼ
#define IMPLEMENT_PUT_TETRO(P, Numerator, Denominator) \
void TetrominoGrid::putTetro##P(const shared_ptr<Tetromino>& tetro) \
{ \
	int maxIndex = INT_MIN, minIndex = INT_MAX; \
	for (int i = 0; i < 4; i++) \
	{ \
		maxIndex = max(tetro->getShape()[i][0], maxIndex); \
		minIndex = min(tetro->getShape()[i][0], minIndex); \
	} \
	int colStart = MAX_COL * Numerator / Denominator - (maxIndex - minIndex + 1) / 2; \
	int rowStart = MAX_ALIVE_ROW; \
	for (int i = 0; i < 4; i++) \
	{ \
		auto& block = tetro->getBlocks()[i]; \
		auto cx = colStart + tetro->getShape()[i][0]; \
		auto ry = rowStart + tetro->getShape()[i][1]; \
		/* ����� */ \
		_grid[cx][ry] = block; \
		/* ����Player���ڲ������ĸ���Ƶ��������� */ \
		curTetroCoord##P[i][0] = cx; \
		curTetroCoord##P[i][1] = ry; \
	} \
}

IMPLEMENT_PUT_TETRO(P1, 1, 3);
IMPLEMENT_PUT_TETRO(P2, 2, 3);

//// ����һ��P1�����ĸ���Ƶ�Grid������һ�������µ�P1һ�ֿ�ʼ
//void TetrominoGrid::putTetroP1(const shared_ptr<Tetromino>& tetro)
//{
//	int maxIndex = INT_MIN, minIndex = INT_MAX;
//	for (int i = 0; i < 4; i++)
//	{
//		maxIndex = max(tetro->getShape()[i][0], maxIndex);
//		minIndex = min(tetro->getShape()[i][0], minIndex);
//	}
//
//	int colStart = MAX_COL * 1 / 3 - (maxIndex - minIndex + 1) / 2;
//	int rowStart = MAX_ALIVE_ROW;
//	for (int i = 0; i < 4; i++)
//	{
//		auto& block = tetro->getBlocks()[i];
//		auto cx = colStart + tetro->getShape()[i][0];
//		auto ry = rowStart + tetro->getShape()[i][1];
//		// �����
//		_grid[cx][ry] = block;
//		// ����P1���ڲ������ĸ���Ƶ���������
//		curTetroCoordP1[i][0] = cx;
//		curTetroCoordP1[i][1] = ry;
//	}
//}
//
//// ����һ��P2�����ĸ���Ƶ�Grid������һ�������µ�P2һ�ֿ�ʼ
//void TetrominoGrid::putTetroP2(const shared_ptr<Tetromino>& tetro)
//{
//	int maxIndex = INT_MIN, minIndex = INT_MAX;
//	for (int i = 0; i < 4; i++)
//	{
//		maxIndex = max(tetro->getShape()[i][0], maxIndex);
//		minIndex = min(tetro->getShape()[i][0], minIndex);
//	}
//
//	int colStart = MAX_COL * 2 / 3 - (maxIndex - minIndex + 1) / 2;
//	int rowStart = MAX_ALIVE_ROW;
//	for (int i = 0; i < 4; i++)
//	{
//		auto& block = tetro->getBlocks()[i];
//		auto cx = colStart + tetro->getShape()[i][0];
//		auto ry = rowStart + tetro->getShape()[i][1];
//		// �����
//		_grid[cx][ry] = block;
//		// ����P2���ڲ������ĸ���Ƶ���������
//		curTetroCoordP2[i][0] = cx;
//		curTetroCoordP2[i][1] = ry;
//	}
//}

#define SET_VAR_CX_RY(P, i, cx, ry) \
int cx = curTetroCoord##P[i][0]; \
int ry = curTetroCoord##P[i][1];

#define SET_REF_CX_RY(P, i, cx, ry) \
int& cx = curTetroCoord##P[i][0]; \
int& ry = curTetroCoord##P[i][1];

#define TAKE_UP_TETRO(P, i) \
for (int i = 0; i < 4; i++) \
{ \
	SET_VAR_CX_RY(P, i, cx, ry); \
	_grid[cx][ry] = nullptr; \
}

#define PUT_TETRO_BY_COORD(P, i) \
for (int i = 0; i < 4; i++) \
{ \
	auto& block = curTetro##P->getBlocks()[i]; \
	SET_VAR_CX_RY(P, i, cx, ry); \
	_grid[cx][ry] = block; \
}

#define PUT_BACK_TETRO PUT_TETRO_BY_COORD

#define IMPLEMENT_CAN_FALL(P) \
bool TetrominoGrid::canFall##P() \
{ \
	/* Ĭ�Ͽ������� */ \
	bool result = true; \
	/* �ȴ�Grid�������ĸ���Ƶĸ��� */ \
	TAKE_UP_TETRO(P, i); \
	/* ����Ƿ���ڲ���̤��ĸ��� */ \
	for (int i = 0; i < 4; i++) \
	{ \
		SET_VAR_CX_RY(P, i, cx, ry); \
		if (!isAccessible(cx, ry - 1)) \
		{ \
			result = false; \
			break; \
		} \
	} \
	/* �ٷŻ�ȥ */ \
	PUT_BACK_TETRO(P, i); \
	/* ���ؽ�� */ \
	return result; \
}

IMPLEMENT_CAN_FALL(P1);
IMPLEMENT_CAN_FALL(P2);

// ���ԵĻ�������һ��
#define IMPLEMENT_FALL(P) \
bool TetrominoGrid::fall##P() \
{ \
	if (!canFall##P()) return false; \
	TAKE_UP_TETRO(P, i); \
	for (int i = 0; i < 4; i++) \
	{ \
		int& ry = curTetroCoord##P[i][1]; \
		ry -= 1; \
	} \
	PUT_TETRO_BY_COORD(P, i); \
	return true; \
}

IMPLEMENT_FALL(P1);
IMPLEMENT_FALL(P2);

#define IMPLEMENT_CAN_MOVE(P) \
bool TetrominoGrid::canMove##P(DIRECTION dir) \
{ \
	/* Ĭ�Ͽ����ƶ� */ \
	bool result = true; \
	/* ��ͬ�����ƶ�uindexƫ�Ʋ�ͬ */ \
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1; \
	/* �ȴ�Grid�������ĸ���Ƶĸ��� */ \
	TAKE_UP_TETRO(P, i); \
	/* ����Ƿ���ڲ���̤��ĸ��� */ \
	for (int i = 0; i < 4; i++) \
	{ \
		SET_VAR_CX_RY(P, i, cx, ry); \
		if (!isAccessible(cx + offset, ry)) \
		{ \
			result = false; \
			break; \
		} \
	} \
	/* �ٷŻ�ȥ */ \
	PUT_BACK_TETRO(P, i); \
	/* ���ؽ�� */ \
	return result; \
}

IMPLEMENT_CAN_MOVE(P1);
IMPLEMENT_CAN_MOVE(P2);

// ���ԵĻ���������ƶ�һ��
#define IMPLEMENT_MOVE(P) \
bool TetrominoGrid::move##P(DIRECTION dir) \
{ \
	if (!canMove##P(dir)) return false; \
	TAKE_UP_TETRO(P, i); \
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1; \
	for (int i = 0; i < 4; i++) \
	{ \
		int& cx = curTetroCoord##P[i][0]; \
		cx += offset; \
	} \
	PUT_TETRO_BY_COORD(P, i); \
	return true; \
}

IMPLEMENT_MOVE(P1);
IMPLEMENT_MOVE(P2);

#define SET_CX_RY_AFTER_ROTATION(P, i, cx, ry, a, b) \
do { \
	int a = curTetro##P->getRotation()[i][0]; \
	int b = curTetro##P->getRotation()[i][1]; \
	int angle = curTetro##P->getAngle(); \
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

#define IMPLEMENT_CAN_ROTATE(P) \
bool TetrominoGrid::canRotate##P() \
{ \
	/* Ĭ�Ͽ�����ת */ \
	bool result = true; \
	/* �ȴ�Grid�������ĸ���Ƶĸ��� */ \
	TAKE_UP_TETRO(P, i); \
	/* ����Ƿ���ڲ���̤��ĸ��� */ \
	for (int i = 0; i < 4; i++) \
	{ \
		/* cx, ry��������ʱ���� */ \
		SET_VAR_CX_RY(P, i, cx, ry); \
		/* { a, b } ����ο� Tetromino.h */ \
		SET_CX_RY_AFTER_ROTATION(P, i, cx, ry, a, b); \
		if (!isAccessible(cx, ry)) \
		{ \
			result = false; \
			break; \
		} \
	} \
	/* �ٷŻ�ȥ */ \
	PUT_BACK_TETRO(P, i); \
	/* ���ؽ�� */ \
	return result; \
}

IMPLEMENT_CAN_ROTATE(P1);
IMPLEMENT_CAN_ROTATE(P2);

// ���ԵĻ�����ת��ʮ��
#define IMPLEMENT_ROTATE(P) \
bool TetrominoGrid::rotate##P() \
{ \
	if (!canRotate##P()) return false; \
	TAKE_UP_TETRO(P, i); \
	for (int i = 0; i < 4; i++) \
	{ \
		SET_REF_CX_RY(P, i, cx, ry); \
		/* { a, b } ����ο� Tetromino.h */ \
		SET_CX_RY_AFTER_ROTATION(P, i, cx, ry, a, b); \
	} \
	curTetro##P->rotate(); \
	PUT_TETRO_BY_COORD(P, i); \
	return true; \
}

IMPLEMENT_ROTATE(P1);
IMPLEMENT_ROTATE(P2);

#define IMPLEMENT_HARD_DROP(P) \
bool TetrominoGrid::hardDrop##P() \
{ \
	int fallCount = 0; \
	while (fall##P()) \
		fallCount++; \
	return fallCount > 0; \
}

IMPLEMENT_HARD_DROP(P1);
IMPLEMENT_HARD_DROP(P2);

#define IMPLEMENT_CHECK_GAME_OVER_BEFORE_NEXT_ROUND(P, OtherP) \
bool TetrominoGrid::checkGameOverBeforeNextRound##P() \
{ \
	/* ���ÿ���һ�����µĹ��� */ \
	TAKE_UP_TETRO(OtherP, i); \
	/* �ж�*/ \
	bool result = isSomeBlocksOverAliveRow(); \
	/* �ж����ٷ�ԭλ */ \
	PUT_BACK_TETRO(OtherP, i); \
	return result; \
}

IMPLEMENT_CHECK_GAME_OVER_BEFORE_NEXT_ROUND(P1, P2);
IMPLEMENT_CHECK_GAME_OVER_BEFORE_NEXT_ROUND(P2, P1);

bool TetrominoGrid::isSomeBlocksOverAliveRow()
{
	for (int i = 0; i < MAX_COL; i++)
	{
		for (int j = MAX_ALIVE_ROW; j < MAX_ROW; j++)
		{
			if (isOccupied(i, j))
				return true;
		}
	}
	return false;
}

bool TetrominoGrid::isGameOver()
{
	return false;
}

int TetrominoGrid::getBottomFullRowIndex()
{
	for (int j = 0; j < MAX_ALIVE_ROW; j++)
	{
		bool isFull = true;
		for (int i = 0; i < MAX_COL; i++)
		{
			if (!isOccupied(i, j))
			{
				isFull = false;
				break;
			}
		}
		if (isFull)
			return j;
	}
	return -1;
}

void TetrominoGrid::deleteRowAndFall(int row, bool is_p1_running, bool is_p2_running)
{
	if (is_p1_running)
		TAKE_UP_TETRO(P1, i);
	if (is_p2_running)
		TAKE_UP_TETRO(P2, i);
	// ɾ������
	for (int i = 0; i < MAX_COL; i++)
	{
		_grid[i][row] = nullptr;
	}
	// �������ϵ�ȫ��������һ��
	for (int i = 0; i < MAX_COL; i++)
	{
		for (int j = row + 1; j < MAX_ALIVE_ROW; j++)
		{
			swap(_grid[i][j - 1], _grid[i][j]);
		}
	}
	if (is_p1_running)
		PUT_BACK_TETRO(P1, i);
	if (is_p2_running)
		PUT_BACK_TETRO(P2, i);
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
