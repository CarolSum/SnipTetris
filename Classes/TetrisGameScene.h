#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "TetrisMap.h"
#include "TetrominoAction.h"

//class GameOver;

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
    void drawGridMap();
    void registerListener();

public: /* public member function */
    void gameOver();
    void setActivation(bool b)
    {
        if (b) this->scheduleUpdate();
        else this->unscheduleUpdate();

        _kbListner->setEnabled(b);
    }

private: /* private member */
    bool _isGameOver;
    clock_t _newTime, _oldTime;
    EventListenerKeyboard *_kbListner;
    shared_ptr<TetrisManager> _manager;
    shared_ptr<TetrominoAction> _tetromino;
    unique_ptr<GameOver> _gameOver;
};



#endif // __HELLOWORLD_SCENE_H__
