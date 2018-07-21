#include "GameScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

#include <iostream>
#include <memory>
#include <array>
#include <list>
#include <time.h>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

#include "Util.h"
#include "Block.h"
#include "GameOver.h"
#include "Manager.h"

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Director::getInstance()->setClearColor(Color4F(0, 0, 0, 0));

	// 设置标题字样SnipTetris
	auto titleLabel = Label::create("SnipTetris", "fonts/Fredoka.ttf", 34);
	titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - titleLabel->getContentSize().height - 30));
	this->addChild(titleLabel, 1);

	// 设置右下角关闭按钮
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
        origin.y + closeItem->getContentSize().height / 2));
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	// 设置帧Cache
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("TetrisRes/sprites.plist", "TetrisRes/sprites.png");

	// 设置背景
    auto background = Sprite::create("TetrisRes/background.jpg");
    background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(background);

	// 注册键盘监听事件，在里面初始化_kbListner
    registerListener();

	// 初始化各部件
    _gameOver = make_unique<GameOver>(this);
    _manager = make_shared<Manager>(this);
	
	this->unscheduleUpdate(); // 关闭默认每帧update
	this->setActivation(true); // 开启自定义每400ms做一次update

    return true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::setActivation(bool _active)
{
	if (_active) this->schedule(schedule_selector(GameScene::update), 0.4f);
	else this->unschedule(schedule_selector(GameScene::update));
	this->_kbListner->setEnabled(_active);
}

void GameScene::update(float dt)
{
	_manager->update();
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	_manager->onKeyPressed(keyCode, event);
}

void GameScene::registerListener()
{
    _kbListner = EventListenerKeyboard::create();
    _kbListner->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_kbListner, this);
}

void GameScene::reset()
{
	_gameOver = make_unique<GameOver>(this);
	_manager = make_shared<Manager>(this);
	this->setActivation(true);
}

void GameScene::gameOver()
{
    _isGameOver = true;
    setActivation(false);
    _gameOver->show(true);

    auto keyboard_listner = EventListenerKeyboard::create();
    keyboard_listner->onKeyPressed = [&keyboard_listner, this](EventKeyboard::KeyCode keyCode, Event *event)
    {
        if (_isGameOver)
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_R)
            {
                _isGameOver = false;
                _gameOver->show(false);
				this->reset();
                _eventDispatcher->removeEventListener(keyboard_listner);
                return;
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard_listner, this);
}


