#include "Tetromino.h"

void Tetromino::init(TetrominoGrid grid)
{
	_grid = std::make_shared<TetrominoGrid>(grid);
	_angle = 0;
	_color = DEFAULT_COLOR;
	for (int i = 0; i < 4; i++)
	{
		_blocks[i].coord.cx = shape[i][0];
		_blocks[i].coord.ry = shape[i][1];
	}
}