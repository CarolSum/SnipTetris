#include "Headers.h"
#include "GameManager.h"
#include "TetrominoFactory.h"
#include "TetrisGameScene.h"
#include "TetrisMap.h"
#include "TetrominoAction.h"

void GameManager::init(TetrisGameScene *scene)
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


void GameManager::request(const int& code)
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

const shared_ptr<TetrisMap>& GameManager::getTetrisMap() const
{
    return _gridMap;
}

const shared_ptr<TetrominoAction>& GameManager::getTetrominoAction() const
{
    return _action;
}

