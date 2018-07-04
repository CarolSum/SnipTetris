#include "headers.h"
#include "TetrisManager.h"
#include "TetrominoFactory.h"
#include "TetrisGameScene.h"
#include "TetrisMap.h"
#include "TetrominoAction.h"

void TetrisManager::init(TetrisGameScene *scene)
{
    _scene = scene;
    _action = make_shared<TetrominoAction>(shared_from_this());
    _gridMap = make_shared<TetrisMap>(shared_from_this());

    _action->init(_scene);
    _gridMap->init(_scene);

    _factory = make_unique<TetrominoFactory>();
    _factory->addOrderer(_action);
    _factory->addOrderer(_gridMap);
    this->request(RQ_CONTINUE);
}


void TetrisManager::request(const int& code)
{
    switch (code)
    {
    case RQ_CONTINUE:
        _factory->update(_scene);
        _scene->setActivation(true);
        break;

    case RQ_GAMEOVER: 
        _scene->gameOver(); 
        break;

    case RQ_RESTART:
        _gridMap->reset();
        this->request(RQ_CONTINUE);
        break;
    }
}

const shared_ptr<TetrisMap>& TetrisManager::getTetrisMap() const
{
    // （在这里插入返回语法）
    return _gridMap;
}

const shared_ptr<TetrominoAction>& TetrisManager::getTetromino() const
{
    // （在这里插入返回语法）
    return _action;
}

