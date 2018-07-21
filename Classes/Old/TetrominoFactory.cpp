#include "Headers.h"
#include "TetrominoFactory.h"
#include "TetrisBlockPool.h"
#include "TetrisGameScene.h"

void TetrominoFactory::initAxisPosition(shared_ptr<Tetromino>& p)
{
	// 暂时先这么干 但这么干是错的
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gridMapSize = Size(BLOCK_SIZE * MAX_COL, BLOCK_SIZE * MAX_ROW);
	auto gridMapOrigin = Vec2(MAX(visibleSize.width / 2 - gridMapSize.width / 2, 0), 0);

	auto gridMapLeft = gridMapOrigin.x;
	auto gridMapRight = gridMapLeft + gridMapSize.width;
	auto gridMapBottom = gridMapOrigin.y;
	auto gridMapTop = gridMapBottom + gridMapSize.height;

    float x = (p->shape->referToInicoord_tTable(0, 1) * BLOCK_SIZE) + BLOCK_HALF + gridMapLeft;
    float y = (p->shape->referToInicoord_tTable(1, 1) * BLOCK_SIZE) + BLOCK_HALF + gridMapBottom;

    p->axis->setPosition(Vec2(x, y));
}

shared_ptr<Tetromino> TetrominoFactory::createTetromino(TetrisGameScene *scene, bool visible)
{
	shared_ptr<Tetromino> newTetromino;

	if (_next == nullptr)
	{
		return _next;
	}
	return next;
}

shared_ptr<Tetromino> TetrominoFactory::createTetromino(TetrisGameScene *scene, bool visible)
{
    if (_next != nullptr)
    {
        initAxisPosition(_next);
        return _next;
    }

    auto tetromino = make_shared<Tetromino>();
    tetromino->shape = move(getRandomShape());

    tetromino->axis = Node::create();
    tetromino->axis->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tetromino->axis->setVisible(visible);

    initAxisPosition(tetromino);

    for (auto &block : tetromino->blocks)
    {
        block = TetrisBlockPool::getInstance()->getBlock();
        auto color = TetrisColorPool::getInstance()->getColor(tetromino->shape->getColor());
        block->sprite->setSpriteFrame(color);

        block->sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        block->sprite->setScale(0.5f);
        tetromino->axis->addChild(block->sprite);
    }

    for (int i = 0; i < 4; i++)
    {
		//设置新生成的方块的位置
        float x = tetromino->shape->referToInitLocationTable(0, i) * BLOCK_SIZE;
        float y = tetromino->shape->referToInitLocationTable(1, i) * BLOCK_SIZE;
        tetromino->blocks[i]->sprite->setPosition(Vec2(x, y));

        tetromino->blocks[i]->coord.cx = tetromino->shape->referToInicoord_tTable(0, i);
        tetromino->blocks[i]->coord.ry = tetromino->shape->referToInicoord_tTable(1, i);
    }

    if (!_debugUI) _debugUI = make_unique<DebugUI>(scene);

    updateUI();

    scene->addChild(tetromino->axis);
    return tetromino;
}

void TetrominoFactory::update(TetrisGameScene *scene)
{
    auto tetromino = createTetromino(scene);

    for (const auto& order : _orderers)
    {
        assert(!order.expired() && "order ptr expired");
        order.lock()->tetrominoUpdate(tetromino);
    }

    next(scene);
}

void TetrominoFactory::next(TetrisGameScene *scene)
{
    _next = nullptr;

    _next = createTetromino(scene, false);
    auto vs = Director::getInstance()->getVisibleSize();

    static const float mx = BLOCK_SIZE * MAX_COL;

    static const float x = mx + (vs.width - mx) * 0.5F;

	// 右偏移120
    _next->axis->setPositionX(vs.width * 0.77F + 120);
    _next->axis->setPositionY(BLOCK_SIZE * 16.F);

    _next->axis->setVisible(true);
}

void TetrominoFactory::updateUI()
{
    char n[20] = { 0, };
    sprintf_s(n, "BLOCK:  %d", TetrisBlockPool::getInstance()->size());
    _debugUI->changeText(n);

}

void TetrominoFactory::addOrderer(const shared_ptr<TetrominoOrderer>& orderer)
{
    _orderers.push_back(orderer);
}

shared_ptr<Tetromino> TetrominoFactory::getRandomTetromino()
{
    switch (UTIL::randomRagne(0, 6))
    {
		case 0: return make_shared<TetrominoI>(); break;
		case 1: return make_shared<TetrominoI>(); break;
		case 2: return make_shared<TetrominoI>(); break;
		case 3: return make_shared<TetrominoI>(); break;
		case 4: return make_shared<TetrominoI>(); break;
		case 5: return make_shared<TetrominoI>(); break;
		case 6: return make_shared<TetrominoI>(); break;
		default: return nullptr;
    }
}