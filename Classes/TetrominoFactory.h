#pragma once

#include "Tetromino.h"

class TetrisGameScene;
class TetrominoOrderer;

class TetrominoFactory
{
public:
    void update(TetrisGameScene *);
    void addOrderer(const shared_ptr<TetrominoOrderer>& orderer);

private:
    void initAxisPosition(shared_ptr<Tetromino>& p);

    shared_ptr<Tetromino> createTetromino(TetrisGameScene *, bool visible = true);
    unique_ptr<TetrominoShape> getRandomShape();

    void updateUI();
    void next(TetrisGameScene *);

private:
    list<weak_ptr<TetrominoOrderer>> _orderers;
    unique_ptr<DebugUI> _debugUI;
    shared_ptr<Tetromino> _next;
};
