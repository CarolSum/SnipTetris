#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Manager.h"
#include "GameOver.h"
#include <memory>

using std::shared_ptr;
using std::unique_ptr;

class GameOver;

class TetrisGameScene : public cocos2d::Layer
{
public: /* constructor & destructor */
    static cocos2d::Scene* createScene();
    // implement the "static create()" method manually
    CREATE_FUNC(TetrisGameScene);

private: /* cocos2d: private member function */
    virtual bool init();
    void update(float dt) override;
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

private: /* private member function */
    void reset();
    void registerListener();

public: /* public member function */
    void gameOver();
	void setActivation(bool b);

private: /* private member */
    bool _isGameOver;
    EventListenerKeyboard *_kbListner;
    shared_ptr<Manager> _manager;
    unique_ptr<GameOver> _gameOver;
};
