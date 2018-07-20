#pragma once

#include <memory>
#include "Block.h"

class Tetromino;

typedef std::shared_ptr<Tetromino> grid_t[MAX_ROW][MAX_COL];
typedef bool bool_grid_t[MAX_ROW][MAX_COL];

class TetrominoGrid
{
public:
	TetrominoGrid();
	const grid_t& getGrid() const { return _grid; }
private:
	grid_t _grid = { nullptr, };
	bool_grid_t _occupied_map = { 0, };
};