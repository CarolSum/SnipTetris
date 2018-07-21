#include <algorithm>
#include <memory>
#include "TetrominoGrid.h"
#include "Tetromino.h"
#include "Util.h"

using std::all_of;
using std::begin;
using std::end;
using std::make_shared;
using std::shared_ptr;

TetrominoGrid::TetrominoGrid()
{
	if (!ALL_OF_GRID([](const shared_ptr<Block>& p) { return p == nullptr; }))
		for (int i = 0; i < MAX_ROW; i++)
			for (int j = 0; j < MAX_COL; j++)
				_grid[i][j] = nullptr;
	shared_ptr<Tetromino> curTetro = nullptr;
	shared_ptr<Tetromino> nextTetro = getRandomTetromino();
}

bool TetrominoGrid::fall()
{
	return false;
}

bool TetrominoGrid::move(DIRECTION dir)
{
	return false;
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
