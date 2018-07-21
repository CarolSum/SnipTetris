#pragma once

class TetrominoOrderer
{
public:
    virtual void tetrominoUpdate(const shared_ptr<Tetromino>& tetromino) = 0;
};