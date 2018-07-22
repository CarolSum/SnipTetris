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

// 获取格子指针或者为空
shared_ptr<Block> TetrominoGrid::getBlockOrNull(int cx, int ry)
{
	assert((cx >= 0 && cx < MAX_COL) && (ry >= 0 && ry < MAX_ROW));
	return _grid[cx][ry];
}

// 判断某坐标是否已有格子占用
bool TetrominoGrid::isOccupied(int cx, int ry)
{
	return getBlockOrNull(cx, ry) != nullptr;
}

// 判断某坐标能否放置格子
bool TetrominoGrid::isAccessible(int cx, int ry)
{
	return (cx >= 0 && cx < MAX_COL) && (ry >= 0 && ry < MAX_ROW) && !isOccupied(cx, ry);
}

// 一般用于P1的四格骨牌落地时，执行下一轮
void TetrominoGrid::nextRoundP1()
{
	curTetroP1 = nextTetroP1;
	nextTetroP1 = getRandomTetromino();
	putTetroP1(curTetroP1);
}

// 一般用于P2的四格骨牌落地时，执行下一轮
void TetrominoGrid::nextRoundP2()
{
	curTetroP2 = nextTetroP2;
	nextTetroP2 = getRandomTetromino();
	putTetroP2(curTetroP2);
}

// 放置一个Player的新四格骨牌到Grid顶部，一般用于新的Player的一轮开始
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
		/* 插入块 */ \
		_grid[cx][ry] = block; \
		/* 更新Player正在操作的四格骨牌的坐标数据 */ \
		curTetroCoord##P[i][0] = cx; \
		curTetroCoord##P[i][1] = ry; \
	} \
}

IMPLEMENT_PUT_TETRO(P1, 1, 3);
IMPLEMENT_PUT_TETRO(P2, 2, 3);

#define SET_VAR_CX_RY(P, i, cx, ry) \
int cx = curTetroCoord##P[i][0]; \
int ry = curTetroCoord##P[i][1];

#define SET_REF_CX_RY(P, i, cx, ry) \
int& cx = curTetroCoord##P[i][0]; \
int& ry = curTetroCoord##P[i][1];

#define TAKE_UP_TETRO(P, i) \
for (int i = 0; i < 4; i++) \
{ \
	if (curTetro##P->hasBeenCutOff(i)) continue; \
	SET_VAR_CX_RY(P, i, cx, ry); \
	_grid[cx][ry] = nullptr; \
}

#define PUT_TETRO_BY_COORD(P, i) \
for (int i = 0; i < 4; i++) \
{ \
	if (curTetro##P->hasBeenCutOff(i)) continue; \
	auto& block = curTetro##P->getBlocks()[i]; \
	SET_VAR_CX_RY(P, i, cx, ry); \
	_grid[cx][ry] = block; \
}

#define PUT_BACK_TETRO PUT_TETRO_BY_COORD

#define IMPLEMENT_CAN_FALL(P, OtherP) \
bool TetrominoGrid::canFall##P() \
{ \
	/* 默认可以下落 */ \
	bool result = true; \
	/* 先从Grid上拿走所有玩家四格骨牌的格子 */ \
	TAKE_UP_TETRO(P, i); \
	TAKE_UP_TETRO(OtherP, i); \
	/* 检查是否存在不能踏足的格子 */ \
	for (int i = 0; i < 4; i++) \
	{ \
		/* 忽略被裁剪掉了的block */ \
		if (curTetro##P->hasBeenCutOff(i)) continue; \
		SET_VAR_CX_RY(P, i, cx, ry); \
		if (!isAccessible(cx, ry - 1)) \
		{ \
			result = false; \
			break; \
		} \
	} \
	/* 再放回所有玩家的四格骨牌 */ \
	PUT_BACK_TETRO(P, i); \
	PUT_BACK_TETRO(OtherP, i); \
	/* 返回结果 */ \
	return result; \
}

IMPLEMENT_CAN_FALL(P1, P2);
IMPLEMENT_CAN_FALL(P2, P1);

// 可以的话，下落一格
#define IMPLEMENT_FALL(P, OtherP) \
bool TetrominoGrid::fall##P() \
{ \
	if (!canFall##P()) return false; \
	TAKE_UP_TETRO(P, i); \
	TAKE_UP_TETRO(OtherP, i); \
	for (int i = 0; i < 4; i++) \
	{ \
		int& ry = curTetroCoord##P[i][1]; \
		ry -= 1; \
	} \
	PUT_TETRO_BY_COORD(P, i); \
	PUT_BACK_TETRO(OtherP, i); \
	return true; \
}

IMPLEMENT_FALL(P1, P2);
IMPLEMENT_FALL(P2, P1);

#define IMPLEMENT_CAN_MOVE(P, OtherP) \
bool TetrominoGrid::canMove##P(DIRECTION dir) \
{ \
	/* 默认可以移动 */ \
	bool result = true; \
	/* 不同方向移动uindex偏移不同 */ \
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1; \
	/* 先从Grid上拿走所有玩家四格骨牌的格子 */ \
	TAKE_UP_TETRO(P, i); \
	TAKE_UP_TETRO(OtherP, i); \
	/* 检查是否存在不能踏足的格子 */ \
	for (int i = 0; i < 4; i++) \
	{ \
		/* 忽略被裁剪掉了的block */ \
		if (curTetro##P->hasBeenCutOff(i)) continue; \
		SET_VAR_CX_RY(P, i, cx, ry); \
		if (!isAccessible(cx + offset, ry)) \
		{ \
			result = false; \
			break; \
		} \
	} \
	/* 再放回所有玩家的四格骨牌 */ \
	PUT_BACK_TETRO(P, i); \
	PUT_BACK_TETRO(OtherP, i); \
	/* 返回结果 */ \
	return result; \
}

IMPLEMENT_CAN_MOVE(P1, P2);
IMPLEMENT_CAN_MOVE(P2, P1);

// 可以的话，左或右移动一格
#define IMPLEMENT_MOVE(P, OtherP) \
bool TetrominoGrid::move##P(DIRECTION dir) \
{ \
	if (!canMove##P(dir)) return false; \
	TAKE_UP_TETRO(P, i); \
	TAKE_UP_TETRO(OtherP, i); \
	int offset = (dir == DIRECTION::LEFT) ? -1 : 1; \
	for (int i = 0; i < 4; i++) \
	{ \
		int& cx = curTetroCoord##P[i][0]; \
		cx += offset; \
	} \
	PUT_TETRO_BY_COORD(P, i); \
	PUT_BACK_TETRO(OtherP, i); \
	return true; \
}

IMPLEMENT_MOVE(P1, P2);
IMPLEMENT_MOVE(P2, P1);

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

#define IMPLEMENT_CAN_ROTATE(P, OtherP) \
bool TetrominoGrid::canRotate##P() \
{ \
	/* 默认可以旋转 */ \
	bool result = true; \
	/* 先从Grid上拿走所有玩家四格骨牌的格子 */ \
	TAKE_UP_TETRO(P, i); \
	TAKE_UP_TETRO(OtherP, i); \
	/* 检查是否存在不能踏足的格子 */ \
	for (int i = 0; i < 4; i++) \
	{ \
		/* 忽略被裁剪掉了的block */ \
		if (curTetro##P->hasBeenCutOff(i)) continue; \
		/* cx, ry都是新临时变量 */ \
		SET_VAR_CX_RY(P, i, cx, ry); \
		/* { a, b } 定义参考 Tetromino.h */ \
		SET_CX_RY_AFTER_ROTATION(P, i, cx, ry, a, b); \
		if (!isAccessible(cx, ry)) \
		{ \
			result = false; \
			break; \
		} \
	} \
	/* 再放回所有玩家的四格骨牌 */ \
	PUT_BACK_TETRO(P, i); \
	PUT_BACK_TETRO(OtherP, i); \
	/* 返回结果 */ \
	return result; \
}

IMPLEMENT_CAN_ROTATE(P1, P2);
IMPLEMENT_CAN_ROTATE(P2, P1);

// 可以的话，旋转九十度
#define IMPLEMENT_ROTATE(P, OtherP) \
bool TetrominoGrid::rotate##P() \
{ \
	if (!canRotate##P()) return false; \
	TAKE_UP_TETRO(P, i); \
	TAKE_UP_TETRO(OtherP, i); \
	for (int i = 0; i < 4; i++) \
	{ \
		SET_REF_CX_RY(P, i, cx, ry); \
		/* { a, b } 定义参考 Tetromino.h */ \
		SET_CX_RY_AFTER_ROTATION(P, i, cx, ry, a, b); \
	} \
	curTetro##P->rotate(); \
	PUT_TETRO_BY_COORD(P, i); \
	PUT_BACK_TETRO(OtherP, i); \
	return true; \
}

IMPLEMENT_ROTATE(P1, P2);
IMPLEMENT_ROTATE(P2, P1);

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


bool TetrominoGrid::snipP1()
{
	int cut_count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (curTetroP1->hasBeenCutOff(i)) continue;
		for (int j = 0; j < 4; j++)
		{
			if (curTetroP2->hasBeenCutOff(j)) continue;
			SET_VAR_CX_RY(P1, i, p1_cx, p1_ry);
			SET_VAR_CX_RY(P2, j, p2_cx, p2_ry);
			if (p1_cx == p2_cx && p1_ry == p2_ry)
			{
				if (curTetroP2->cutOff(j))
				{
					// 从Grid中移除
					_grid[p2_cx][p2_ry] = nullptr;
					// 放置剪切者
					_grid[p1_cx][p1_ry] = curTetroP1->getBlocks()[i];
					cut_count++;
					break;
				}
			}
		}
	}
	return cut_count > 0;
}

bool TetrominoGrid::snipP2()
{
	int cut_count = 0;
	for (int j = 0; j < 4; j++)
	{
		if (curTetroP2->hasBeenCutOff(j)) continue;
		for (int i = 0; i < 4; i++)
		{
			if (curTetroP1->hasBeenCutOff(i)) continue;
			SET_VAR_CX_RY(P2, j, p2_cx, p2_ry);
			SET_VAR_CX_RY(P1, i, p1_cx, p1_ry);
			if (p1_cx == p2_cx && p1_ry == p2_ry)
			{
				if (curTetroP1->cutOff(i))
				{
					// 从Grid中移除
					_grid[p1_cx][p1_ry] = nullptr;
					// 放置剪切者
					_grid[p2_cx][p2_ry] = curTetroP2->getBlocks()[i];
					cut_count++;
					break;
				}
			}
		}
	}
	return cut_count > 0;
}

#define IMPLEMENT_CHECK_GAME_OVER_BEFORE_NEXT_ROUND(P, OtherP) \
bool TetrominoGrid::checkGameOverBeforeNextRound##P() \
{ \
	/* 先判断落地时是否属于重叠状态，是则游戏结束 */ \
	if (!getOverlapBlocks().empty()) return true; \
	/* 先拿开另一个碍事的骨牌 */ \
	TAKE_UP_TETRO(OtherP, i); \
	/* 判断*/ \
	bool result = isSomeBlocksOverAliveRow(); \
	/* 判断完再放原位 */ \
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

// 获取所有重叠的格子 List { Pair{ P1's block, P2's block }, ... }
list<pair<shared_ptr<Block>, shared_ptr<Block>>> TetrominoGrid::getOverlapBlocks()
{
	list<pair<shared_ptr<Block>, shared_ptr<Block>>> overlap_blocks;
	if (!curTetroP1 || !curTetroP2) return overlap_blocks;
	// 对于每一个P1的block
	for (int i = 0; i < 4; i++)
	{
		// 忽略P1中被裁剪掉了的block
		if (curTetroP1->hasBeenCutOff(i)) continue;
		// 对于每一个P2的block
		for (int j = 0; j < 4; j++)
		{
			// 忽略P2中被裁剪掉了的block
			if (curTetroP2->hasBeenCutOff(j)) continue;
			SET_VAR_CX_RY(P1, i, p1_cx, p1_ry);
			SET_VAR_CX_RY(P2, j, p2_cx, p2_ry);
			if (p1_cx == p2_cx && p1_ry == p2_ry)
			{
				overlap_blocks.push_back(
					make_pair(curTetroP1->getBlocks()[i], curTetroP2->getBlocks()[j])
				);
			}
		}
	}
	return overlap_blocks;
}

void TetrominoGrid::deleteRowAndFall(int row, bool is_p1_running, bool is_p2_running)
{
	if (is_p1_running)
		TAKE_UP_TETRO(P1, i);
	if (is_p2_running)
		TAKE_UP_TETRO(P2, i);
	// 删除整行
	for (int i = 0; i < MAX_COL; i++)
	{
		_grid[i][row] = nullptr;
	}
	// 该行以上的全部往下移一行
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
