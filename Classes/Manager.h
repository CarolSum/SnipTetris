#pragma once

#include <memory>
#include "TetrominoGrid.h"
#include "Block.h"
#include "ParticleManager.h"

using std::shared_ptr;
using std::unique_ptr;

class GameScene;

class Manager
{
public:
	Manager(GameScene *scene);
	~Manager();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	void update();
	const shared_ptr<TetrominoGrid>& getGrid();
private:
	shared_ptr<TetrominoGrid> _grid = nullptr;
	STATE _state = STATE::CONTINUE;
	GameScene *_scene = nullptr;
	Node *_gridnode = nullptr;
	unique_ptr<ParticleManager> _particleManager = nullptr;

private:
	void initGridNode();
	void displayGridLine();
	void displayNextTetromino();
	void displayGrid();

	void nextRound();

	// 默认窗口大小（分辨率）不能改变
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Size gridSize = Size(BLOCK_SIZE * MAX_COL, BLOCK_SIZE * MAX_ROW);
	const Vec2 gridOrigin = Vec2(MAX(visibleSize.width / 2 - gridSize.width / 2, 0), 0);
	const float gridLeft = gridOrigin.x;
	const float gridRight = gridLeft + gridSize.width;
	const float gridBottom = gridOrigin.y;
	const float gridTop = gridBottom + gridSize.height;
};

