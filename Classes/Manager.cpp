#include "Manager.h"
#include "GameScene.h"

#include <memory>
#include <algorithm>

using std::make_shared;
using std::make_unique;
using std::min;
using std::max;

Manager::Manager(GameScene *scene) : _scene(scene)
{
	_grid = make_shared<TetrominoGrid>();
	_gridnode = Node::create();
	_particleManager = make_unique<ParticleManager>(_gridnode);

	initGridNode();
	displayGridLine();

	nextRoundP1();
	nextRoundP2();
}

Manager::~Manager()
{
	_gridnode->removeAllChildrenWithCleanup(true);
	_gridnode->removeFromParent();
}

const shared_ptr<TetrominoGrid>& Manager::getGrid()
{
	return _grid;
}

// ÿ��400ms������һ�Σ�һ��Ϊ��������
void Manager::update()
{
	bool result1 = getGrid()->fallP1();
	bool result2 = getGrid()->fallP2();
	if (result1 || result2)
		displayGrid();
	if (!result1 || !result2)
	{
		if (!result1 && getGrid()->checkGameOverBeforeNextRoundP1()) {
			_scene->gameOver();
			return;
		}
		if (!result2 && getGrid()->checkGameOverBeforeNextRoundP2()) {
			_scene->gameOver();
			return;
		}

		do {
			// ��һ���Ƿ�����Ҫɾ������
			int deletedRowIndex = getGrid()->getBottomFullRowIndex();
			// û�о�����ȥ
			if (deletedRowIndex < 0) break;
			// չʾ����Ч��
			for (int i = 0; i < MAX_COL; i++)
			{
				auto& block = getGrid()->getBlockOrNull(i, deletedRowIndex);
				if (!block) continue;
				_particleManager->add(block->sprite->getPosition());
				// �ӽ������Ƴ�blockͼƬ
				block->sprite->removeFromParentAndCleanup(true);
			}
			_particleManager->show();
			// ��Grid��ά������ɾ�����е�blocks
			getGrid()->deleteRowAndFall(deletedRowIndex, result1, result2);
		} while (1);

		if (!result1) nextRoundP1();
		if (!result2) nextRoundP2();
	}
}

void Manager::initGridNode()
{
	_gridnode->setVisible(true);
	_gridnode->setContentSize(Size(MAX_COL * BLOCK_SIZE, MAX_ROW * BLOCK_SIZE));
	_gridnode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_gridnode->setPositionX(gridLeft);
	_gridnode->setPositionY(gridBottom);
	_scene->addChild(_gridnode);
}

// ���Ƹ�����
void Manager::displayGridLine()
{
	for (int i = 0; i <= MAX_ROW; i++)
	{
		auto lineY = i * BLOCK_SIZE;
		auto draw = DrawNode::create();
		draw->drawLine(
			Vec2(0, lineY), // �������
			Vec2(gridSize.width, lineY), // �����յ�
			Color4F(Color3B::WHITE, 0.2));
		_gridnode->addChild(draw);
	}

	for (int i = 0; i <= MAX_COL; i++)
	{
		auto lineX = i * BLOCK_SIZE;
		auto draw = DrawNode::create();
		draw->drawLine(
			Vec2(lineX, 0), // �������
			Vec2(lineX, gridSize.height), //�����յ�
			Color4F(Color3B::WHITE, 0.2));
		_gridnode->addChild(draw);
	}
}

// ��ʾP1����ʾ����ͼ
void Manager::displayNextTetrominoP1()
{
	// ��������һ�ε���ʾͼ
	if (auto _node = _scene->getChildByName("nextTetroNodeP1"))
	{
		_node->removeAllChildrenWithCleanup(true);
		_scene->removeChild(_node);
	}

	auto nextTetroAxisP1 = Node::create();
	nextTetroAxisP1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	nextTetroAxisP1->setVisible(true);

	// ������߿�λ�����ĵ�
	auto leftMidX = gridLeft / 2;
	auto leftMidY = visibleSize.height / 2;

	auto& tetro = _grid->nextTetroP1;

	float maxX = FLT_MIN, minX = FLT_MAX, maxY = FLT_MIN, minY = FLT_MAX;
	for (int i = 0; i < 4; i++)
	{
		auto& block = tetro->getBlocks()[i];

		auto rect = block->sprite->getSpriteFrame()->getRect();
		block->sprite->setPositionX(tetro->getShape()[i][0] * rect.size.width);
		block->sprite->setPositionY(tetro->getShape()[i][1] * rect.size.height);

		nextTetroAxisP1->addChild(block->sprite);

		maxX = max(block->sprite->getBoundingBox().getMaxX(), maxX);
		maxY = max(block->sprite->getBoundingBox().getMaxY(), maxY);
		minX = min(block->sprite->getBoundingBox().getMinX(), minX);
		minY = min(block->sprite->getBoundingBox().getMinY(), minY);
	}

	// ��һ���ĸ������ʾ���С
	float nextTetroAxisWidth = maxX - minX;
	float nextTetroAxisHeight = maxY - minY;
	// nextTetroAxisλ�õ�ƫ����
	float xOffset = -(nextTetroAxisWidth / 2);
	float yOffset = -(nextTetroAxisHeight / 2);

	float scale = 0.5;
	nextTetroAxisP1->setScale(scale);
	// λ������
	nextTetroAxisP1->setPosition(Vec2(leftMidX + xOffset * scale, leftMidY + yOffset * scale));

	_scene->addChild(nextTetroAxisP1, 0, "nextTetroNodeP1");
}

// ��ʾP2����ʾ����ͼ
void Manager::displayNextTetrominoP2()
{
	// ��������һ�ε���ʾͼ
	if (auto _node = _scene->getChildByName("nextTetroNodeP2"))
	{
		_node->removeAllChildrenWithCleanup(true);
		_scene->removeChild(_node);
	}

	auto nextTetroAxisP2 = Node::create();
	nextTetroAxisP2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	nextTetroAxisP2->setVisible(true);

	// �����ұ߿�λ�����ĵ�
	auto rightMidX = (visibleSize.width - gridRight) / 2 + gridRight;
	auto rightMidY = visibleSize.height / 2;

	auto& tetro = _grid->nextTetroP2;

	float maxX = FLT_MIN, minX = FLT_MAX, maxY = FLT_MIN, minY = FLT_MAX;
	for (int i = 0; i < 4; i++)
	{
		auto& block = tetro->getBlocks()[i];

		auto rect = block->sprite->getSpriteFrame()->getRect();
		block->sprite->setPositionX(tetro->getShape()[i][0] * rect.size.width);
		block->sprite->setPositionY(tetro->getShape()[i][1] * rect.size.height);

		nextTetroAxisP2->addChild(block->sprite);

		maxX = max(block->sprite->getBoundingBox().getMaxX(), maxX);
		maxY = max(block->sprite->getBoundingBox().getMaxY(), maxY);
		minX = min(block->sprite->getBoundingBox().getMinX(), minX);
		minY = min(block->sprite->getBoundingBox().getMinY(), minY);
	}

	// ��һ���ĸ������ʾ���С
	float nextTetroAxisWidth = maxX - minX;
	float nextTetroAxisHeight = maxY - minY;
	// nextTetroAxisλ�õ�ƫ����
	float xOffset = -(nextTetroAxisWidth / 2);
	float yOffset = -(nextTetroAxisHeight / 2);

	float scale = 0.5;
	nextTetroAxisP2->setScale(scale);
	// λ������
	nextTetroAxisP2->setPosition(Vec2(rightMidX + xOffset * scale, rightMidY + yOffset * scale));

	_scene->addChild(nextTetroAxisP2, 0, "nextTetroNodeP2");
}


// ��ʾ��ˢ�£�Gridͼ
void Manager::displayGrid()
{
	auto overlap_blocks = getGrid()->getOverlapBlocks();
	
	for (int i = 0; i < MAX_COL; i++)
	{
		for (int j = 0; j < MAX_ROW; j++)
		{
			auto& block = getGrid()->getBlockOrNull(i, j);
			if (!block) continue;

			bool is_overlap = false;
			Sprite* other_sprite = nullptr;

			// �ȿ����ǲ����ص��ĵ�block���ص�����һ��blockռ��Grid��λ�ã�
			for (auto p : overlap_blocks)
			{
				if (p.first == block)
				{
					is_overlap = true;
					other_sprite = p.second->sprite;
					break;
				}
				else if (p.second == block)
				{
					is_overlap = true;
					other_sprite = p.first->sprite;
					break;
				}
			}

			auto rect = block->sprite->getSpriteFrame()->getRect();
			block->sprite->setPosition(i * BLOCK_SIZE, j * BLOCK_SIZE);
			block->sprite->setContentSize(Size(BLOCK_SIZE, BLOCK_SIZE));

			if (is_overlap)
			{
				other_sprite->setPosition(block->sprite->getPosition());
				BlendFunc cbl = { GL_ONE, GL_ONE };
				other_sprite->setBlendFunc(cbl);
			}
			
			if (block->sprite->getParent() != _gridnode)
				_gridnode->addChild(block->sprite);
		}
	}
}

void Manager::nextRoundP1()
{
	getGrid()->nextRoundP1();
	// ��������˳�����ң���ȡ��ԭ���ӽڵ����Ϊ���ӽڵ�����¸��ڵ�
	displayNextTetrominoP1();
	displayGrid();
}

void Manager::nextRoundP2()
{
	getGrid()->nextRoundP2();
	// ��������˳�����ң���ȡ��ԭ���ӽڵ����Ϊ���ӽڵ�����¸��ڵ�
	displayNextTetrominoP2();
	displayGrid();
}

void Manager::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	switch (keyCode)
	{
	// ������P1��������(wsad)
	case EventKeyboard::KeyCode::KEY_SPACE:
		_grid->hardDropP1();
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		_grid->moveP1(DIRECTION::LEFT);
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		_grid->moveP1(DIRECTION::RIGHT);
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		_grid->rotateP1();
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		_grid->fallP1();
		displayGrid();
		break;


	// ������P2��������(ikjl)
	case EventKeyboard::KeyCode::KEY_ENTER:
		_grid->hardDropP2();
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_CAPITAL_J:
	case EventKeyboard::KeyCode::KEY_J:
		_grid->moveP2(DIRECTION::LEFT);
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_CAPITAL_L:
	case EventKeyboard::KeyCode::KEY_L:
		_grid->moveP2(DIRECTION::RIGHT);
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_CAPITAL_I:
	case EventKeyboard::KeyCode::KEY_I:
		_grid->rotateP2();
		displayGrid();
		break;

	case EventKeyboard::KeyCode::KEY_CAPITAL_K:
	case EventKeyboard::KeyCode::KEY_K:
		_grid->fallP2();
		displayGrid();
		break;
	}
}
