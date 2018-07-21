#pragma once

#include <memory>
#include "Block.h"

using std::shared_ptr;

class Tetromino;

typedef shared_ptr<Block> grid_t[MAX_ROW][MAX_COL];

#define ALL_OF_GRID(callback) (all_of((shared_ptr<Block>*)_grid, (shared_ptr<Block>*)_grid + MAX_ROW * MAX_COL, (callback)))

// enum GRID_STATE { ALIVE, FULL_LINE, };

class TetrominoGrid
{
public:
	TetrominoGrid();
	const grid_t& getGrid() const { return _grid; }
	bool fall();
	bool move(DIRECTION dir);
	bool rotate();
	bool hardDrop();

	bool hasFullRow();
	bool isGameOver();

	void checkState();
private:
	shared_ptr<Tetromino> getRandomTetromino();

	grid_t _grid = { nullptr, };
	shared_ptr<Tetromino> curTetro = nullptr;
	shared_ptr<Tetromino> nextTetro = nullptr;
};