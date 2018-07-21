#pragma once

#include <memory>
#include "TetrominoGrid.h"
#include "Block.h"

using std::shared_ptr;

class TetrisGameScene;

class Manager
{
public:
	Manager(TetrisGameScene *scene);
	~Manager();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event);
	void update(float dt);
private:
	shared_ptr<TetrominoGrid> _grid = nullptr;
	STATE _state = STATE::CONTINUE;
	TetrisGameScene *_scene = nullptr;
};

