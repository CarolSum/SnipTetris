#include "Manager.h"
#include "TetrisGameScene.h"

#include <memory>
#include <algorithm>

using std::make_shared;
using std::shared_ptr;
using std::min;
using std::max;

Manager::Manager(TetrisGameScene *scene) : _scene(scene)
{
	_grid = make_shared<TetrominoGrid>();
	initGridNode();
	displayGridLine();

	nextRound();
}

Manager::~Manager()
{
}

const shared_ptr<TetrominoGrid>& Manager::getGrid()
{
	return _grid;
}

void Manager::update()
{
	bool successToFall = _grid->fall();
	if (successToFall)
		displayGrid();
	else if (isGameOver())
		_scene->gameOver();
	else
		nextRound();
}

void Manager::initGridNode()
{
	_gridnode = Node::create();
	_gridnode->setVisible(true);
	_gridnode->setContentSize(Size(MAX_COL * BLOCK_SIZE, MAX_ROW * BLOCK_SIZE));
	_gridnode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_gridnode->setPositionX(gridLeft);
	_gridnode->setPositionY(gridBottom);
	_scene->addChild(_gridnode);
}

void Manager::displayGridLine()
{
	for (int i = 0; i <= MAX_ROW; i++)
	{
		auto lineY = i * BLOCK_SIZE + gridBottom;
		auto draw = DrawNode::create();
		draw->drawLine(
			Vec2(gridLeft, lineY), // 横线起点
			Vec2(gridRight, lineY), // 横线终点
			Color4F(Color3B::WHITE, 0.2));
		_scene->addChild(draw);
	}

	for (int i = 0; i <= MAX_COL; i++)
	{
		auto lineX = i * BLOCK_SIZE + gridLeft;
		auto draw = DrawNode::create();
		draw->drawLine(
			Vec2(lineX, gridBottom), // 竖线起点
			Vec2(lineX, gridTop), //竖线终点
			Color4F(Color3B::WHITE, 0.2));
		_scene->addChild(draw);
	}
}

// 显示提示骨牌图
void Manager::displayNextTetromino()
{
	// 先清理上一次的提示图
	if (auto _node = _scene->getChildByName("nextTetroNode"))
	{
		_node->removeAllChildrenWithCleanup(true);
		_scene->removeChild(_node);
	}

	auto nextTetroAxis = Node::create();
	nextTetroAxis->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	nextTetroAxis->setVisible(true);
	
	// 网格右边空位的中心点
	auto rightMidX = (visibleSize.width - gridRight) / 2 + gridRight;
	auto rightMidY = visibleSize.height / 2;

	auto& tetro = _grid->nextTetro;

	float maxX = FLT_MIN, minX = FLT_MAX, maxY = FLT_MIN, minY = FLT_MAX;
	for (int i = 0; i < 4; i++)
	{
		auto& block = tetro->getBlocks()[i];

		auto rect = block->sprite->getSpriteFrame()->getRect();
		block->sprite->setPositionX(tetro->getShape()[i][0] * rect.size.width);
		block->sprite->setPositionY(tetro->getShape()[i][1] * rect.size.height);
		block->sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		nextTetroAxis->addChild(block->sprite);

		maxX = max(block->sprite->getBoundingBox().getMaxX(), maxX);
		maxY = max(block->sprite->getBoundingBox().getMaxY(), maxY);
		minX = min(block->sprite->getBoundingBox().getMinX(), minX);
		minY = min(block->sprite->getBoundingBox().getMinY(), minY);
	}

	// 下一个四格骨牌提示框大小
	float nextTetroAxisWidth = maxX - minX;
	float nextTetroAxisHeight = maxY - minY;
	// nextTetroAxis位置的偏移量
	float xOffset = -(nextTetroAxisWidth / 2);
	float yOffset = -(nextTetroAxisHeight / 2);

	float scale = 0.5;
	nextTetroAxis->setScale(scale);
	// 位置完美
	nextTetroAxis->setPosition(Vec2(rightMidX + xOffset * scale, rightMidY + yOffset * scale));

	_scene->addChild(nextTetroAxis, 0, "nextTetroNode");
}

void Manager::displayGrid()
{
	for (int i = 0; i < MAX_COL; i++)
	{
		for (int j = 0; j < MAX_ROW; j++)
		{
			auto& block = getGrid()->getBlockOrNull(i, j);
			if (!block) continue;

			auto rect = block->sprite->getSpriteFrame()->getRect();
			block->sprite->setPositionX(i * BLOCK_SIZE);
			block->sprite->setPositionY(j * BLOCK_SIZE);
			block->sprite->setContentSize(Size(BLOCK_SIZE, BLOCK_SIZE));
			if (block->sprite->getParent() != _gridnode)
				_gridnode->addChild(block->sprite);
		}
	}
}

void Manager::nextRound()
{
	getGrid()->nextRound();
	// 下面两个顺序不能乱，先取走原有子节点才能为此子节点添加新父节点
	displayNextTetromino();
	displayGrid();
}

bool Manager::isGameOver()
{
	for (int i = 0; i < MAX_COL; i++)
	{
		for (int j = MAX_ALIVE_ROW; j < MAX_ROW; j++)
		{
			if (getGrid()->isOccupied(i, j))
				return true;
		}
	}
	return false;
}

void Manager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_SPACE:
		_grid->hardDrop();
		displayGrid();
		update(); // 这个加快流畅度
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		_grid->move(DIRECTION::LEFT);
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		_grid->move(DIRECTION::RIGHT);
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		_grid->rotate();
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		_grid->fall();
		displayGrid();
		break;
	}
}
