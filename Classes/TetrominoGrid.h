#pragma once

#include <memory>
#include "Tetromino.h"
#include "Block.h"

using std::shared_ptr;

class Tetromino;

typedef shared_ptr<Block> grid_t[MAX_COL][MAX_ROW];

class TetrominoGrid
{
public:
	TetrominoGrid();
	const grid_t& getGrid() const { return _grid; }

	shared_ptr<Tetromino> curTetroP1 = nullptr;
	shared_ptr<Tetromino> curTetroP2 = nullptr;
	shared_ptr<Tetromino> nextTetroP1 = nullptr;
	shared_ptr<Tetromino> nextTetroP2 = nullptr;
	int curTetroCoordP1[4][2] = { 0 };
	int curTetroCoordP2[4][2] = { 0 };

	void nextRoundP1();
	void nextRoundP2();
	void putTetroP1(const shared_ptr<Tetromino>& tetro);
	void putTetroP2(const shared_ptr<Tetromino>& tetro);

	bool canFallP1();
	bool canFallP2();
	bool canMoveP1(DIRECTION dir);
	bool canMoveP2(DIRECTION dir);
	bool canRotateP1();
	bool canRotateP2();
	bool fallP1();
	bool fallP2();
	bool moveP1(DIRECTION dir);
	bool moveP2(DIRECTION dir);
	bool rotateP1();
	bool rotateP2();
	bool hardDropP1();
	bool hardDropP2();

public:
	bool isGameOver();
	bool isSomeBlocksOverAliveRow();
	bool checkGameOverBeforeNextRoundP1();
	bool checkGameOverBeforeNextRoundP2();
	// 返回值表示行索引，返回-1表示无满行，没有执行删除
	int getBottomFullRowIndex();
	void deleteRowAndFall(int row);

	shared_ptr<Tetromino> getRandomTetromino();
	shared_ptr<Block> getBlockOrNull(int cx, int ry);
	bool isOccupied(int cx, int ry);
	bool isAccessible(int cx, int ry);

private:
	grid_t _grid = { nullptr };
};