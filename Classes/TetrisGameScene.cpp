#include "TetrisGameScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

#include <iostream>
using namespace std;

#include <memory>
#include <array>
#include <list>
#include <time.h>
#include <vector>
#include <algorithm>
#include <functional>

#include "Util.h"
#include "Block.h"
#include "GameOver.h"
#include "Manager.h"

Scene* TetrisGameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TetrisGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool TetrisGameScene::init()
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
        CC_CALLBACK_1(TetrisGameScene::menuCloseCallback, this));
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

	// 画方块网格图
    drawGridMap();

	// 注册键盘监听事件，在里面初始化_kbListner
    registerListener();

	// 初始化各部件
    _gameOver = make_unique<GameOver>(this);
    _manager = make_shared<Manager>(this);
	
	this->unscheduleUpdate(); // 关闭默认每帧update
	this->setActivation(true); // 开启自定义每400ms做一次update

    return true;
}

void TetrisGameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void TetrisGameScene::setActivation(bool b)
{
	if (b) this->schedule(schedule_selector(TetrisGameScene::update), 0.4f);
	else this->unschedule(schedule_selector(TetrisGameScene::update));
	this->_kbListner->setEnabled(b);
}

void TetrisGameScene::update(float dt)
{
	_manager->update(dt);
}

void TetrisGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		if (!_eventDispatcher->isEnabled()) return;
		setActivation(false);
	}
	_manager->onKeyPressed(keyCode, event);
}


// 绘制格子线框
void TetrisGameScene::drawGridMap()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gridMapSize = Size(BLOCK_SIZE * MAX_COL, BLOCK_SIZE * MAX_ROW);
	auto gridMapOrigin = Vec2(MAX(visibleSize.width / 2 - gridMapSize.width / 2, 0), 0);

	auto gridMapLeft = gridMapOrigin.x;
	auto gridMapRight = gridMapLeft + gridMapSize.width;
	auto gridMapBottom = gridMapOrigin.y;
	auto gridMapTop = gridMapBottom + gridMapSize.height;

	for (int i = 0; i <= MAX_ROW; i++)
	{
		auto lineY = i * BLOCK_SIZE + gridMapBottom;
		auto draw = DrawNode::create();
		draw->drawLine(
			Vec2(gridMapLeft, lineY), // 横线起点
			Vec2(gridMapRight, lineY), // 横线终点
			Color4F(Color3B::WHITE, 0.2));
		this->addChild(draw);
	}

	for (int i = 0; i <= MAX_COL; i++)
	{
		auto lineX = i * BLOCK_SIZE + gridMapLeft;
		auto draw = DrawNode::create();
		draw->drawLine(
			Vec2(lineX, gridMapBottom), // 竖线起点
			Vec2(lineX, gridMapTop), //竖线终点
			Color4F(Color3B::WHITE, 0.2));
		this->addChild(draw);
	}
}

void TetrisGameScene::registerListener()
{
    _kbListner = EventListenerKeyboard::create();
    _kbListner->onKeyPressed = CC_CALLBACK_2(TetrisGameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_kbListner, this);
}

void TetrisGameScene::gameOver()
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
                // _manager->request(RQ_RESTART);
                _eventDispatcher->removeEventListener(keyboard_listner);
                return;
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard_listner, this);
}


