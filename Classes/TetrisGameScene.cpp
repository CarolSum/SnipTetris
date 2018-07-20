﻿#include "headers.h"
#include "TetrisGameScene.h"
#include "TetrisParticleManager.h"
#include "TetrisManager.h"
#include "TetrominoFactory.h"
//#include <vld.h>

Scene* TetrisGameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = TetrisGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
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

	// 注册键盘监听事件
    registerListener();

    _gameOver = make_unique<GameOver>(this);
    _manager = make_shared<TetrisManager>();
    _manager->init(this);

    _tetromino = _manager->getTetromino();

    _oldTime = 0;
    _newTime = clock();

    return true;
}

void TetrisGameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void TetrisGameScene::update(float dt)
{
    if (_newTime - _oldTime < 400)
    {
        _newTime = clock();
        return;
    }

    if (!_tetromino->fall())
    {
        return;
    }

    _oldTime = _newTime;
    _newTime = clock();
}

void TetrisGameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
    switch (keyCode) 
    {
    case EventKeyboard::KeyCode::KEY_SPACE:
        if (!_eventDispatcher->isEnabled()) return;
        setActivation(false);
        _tetromino->hardDrop();
        break;

    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
        _tetromino->move(DIR::LEFT);
        break;

    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
        _tetromino->move(DIR::RIGHT);
        break;

    case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
        _tetromino->rotate();
        break;

    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
        _tetromino->fall();
        break;
    }
}

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

	//// 坐标向右偏移240单位
 //   for (int i = 0; i <= MAX_MAP_ROW; i++)
 //   {
 //       auto draw = DrawNode::create();
 //       draw->drawLine(
 //           Vec2(0 + 240, i * BLOCK_SIZE), 
 //           Vec2(BLOCK_SIZE * MAX_COL + 240, i * BLOCK_SIZE), 
 //           Color4F(Color3B::WHITE, 0.2));

 //       this->addChild(draw);
 //   }

 //   for (int i = 0; i <= MAX_COL; i++)
 //   {
 //       auto draw = DrawNode::create();
 //       draw->drawLine(Vec2(i * BLOCK_SIZE + 240, 0), Vec2(i * BLOCK_SIZE + 240, BLOCK_SIZE * 20), Color4F(Color3B::WHITE, 0.2));
 //       this->addChild(draw);
 //   }

#ifdef _DEBUG
    /*for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            char t[10];
            sprintf(t, "%d,%d", i, j);
            auto label = Label::createWithSystemFont(t, "Ariel", 11);
            label->setPosition(j * BLOCK_SIZE + BLOCK_SIZE/2 + 240, i * BLOCK_SIZE + BLOCK_SIZE / 2);
            label->setColor(cocos2d::Color3B::RED);
            label->setAlignment(TextHAlignment::CENTER);
            this->addChild(label, 1);
        }
    }*/
#endif // DEBUG
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
                _oldTime = 0;
                _newTime = clock();

                _isGameOver = false;
                _gameOver->show(false);
                _manager->request(RQ_RESTART);
                _eventDispatcher->removeEventListener(keyboard_listner);
                return;
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard_listner, this);
}


