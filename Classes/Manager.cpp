#include "Manager.h"
#include "cocos2d.h"
USING_NS_CC;

#include <memory>

using std::make_shared;
using std::shared_ptr;

Manager::Manager(TetrisGameScene *scene) : _scene(scene)
{
	_grid = make_shared<TetrominoGrid>();
}


Manager::~Manager()
{
}

void Manager::update(float dt)
{
	_grid->fall();
}

void Manager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_SPACE:
		_grid->hardDrop();
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		_grid->move(DIRECTION::LEFT);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		_grid->move(DIRECTION::RIGHT);
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		_grid->rotate();
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		_grid->fall();
		break;
	}
}
