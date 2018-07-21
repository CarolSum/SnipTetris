#include "Headers.h"
#include "TetrisMap.h"
#include "TetrisBlockPool.h"
#include "TetrisGameScene.h"
#include "GameManager.h"

TetrisMap::TetrisMap(const shared_ptr<GameManager>& manager)
    :_manager(manager)
{
}

TetrisMap::~TetrisMap()
{
}

void TetrisMap::init(TetrisGameScene * scene)
{
    this->reset();
    _particleManager = make_unique<TetrisParticleManager>(scene);
    _collector = bind(&TetrisBlockPool::collect, TetrisBlockPool::getInstance(), std::placeholders::_1);
}

bool TetrisMap::isAccessible(const coord_t& _coord)
{
    array<bool, 4> checkList = 
    {
        _coord.cx >= 0,
		_coord.ry >= 0,
        _coord.cx <= (MAX_COL - 1),
        _coord.ry <= (MAX_ROW - 1)
    };

    if (all_of(checkList.begin(), checkList.end(), [](bool x) { return x; })) 
        return _conceptualMap[rowIndex(_coord.ry)][colIndex(_coord.cx)] == false;

	return false;
}

void TetrisMap::reset()
{
    for (auto &blocks : _concreteMap)
    {
        for (auto &block : blocks)
        {
            if (!block) continue;

            _collector(block);
            block->sprite->retain();
            block->sprite->removeFromParent();
            block = nullptr;
        }
    }

    for (auto& row : _conceptualMap)
        row.fill(false);
}

void TetrisMap::setManager(const shared_ptr<GameManager>& manager)
{
    _manager = manager;
}

bool TetrisMap::update()
{
    auto scene = _tetromino->blocks[0]->sprite->getParent()->getParent();
	// 暂时先这么干 但这么干是错的
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gridMapSize = Size(BLOCK_SIZE * MAX_COL, BLOCK_SIZE * MAX_ROW);
	auto gridMapOrigin = Vec2(MAX(visibleSize.width / 2 - gridMapSize.width / 2, 0), 0);

	auto gridMapLeft = gridMapOrigin.x;
	auto gridMapRight = gridMapLeft + gridMapSize.width;
	auto gridMapBottom = gridMapOrigin.y;
	auto gridMapTop = gridMapBottom + gridMapSize.height;

    for (auto &block : _tetromino->blocks)
    {
        _concreteMap[rowIndex(block->coord.ry)][colIndex(block->coord.cx)] = block;
        _conceptualMap[rowIndex(block->coord.ry)][colIndex(block->coord.cx)] = true;

        block->sprite->retain();
        block->sprite->removeFromParent();
		// 右偏移120
        block->sprite->setPositionX(gridMapLeft + BLOCK_SIZE * block->coord.cx + BLOCK_HALF);
        block->sprite->setPositionY(BLOCK_SIZE * block->coord.ry + BLOCK_HALF);
        scene->addChild(block->sprite);
        block->sprite->release();
    }

    findBingo();
    return true;
}

void TetrisMap::findBingo()
{
    int bingo = 0;

    for (bingo = INDEX_MIN_ROW; bingo < MAX_ROW; bingo++)
    {
        if (all_of(_conceptualMap[bingo].begin(), _conceptualMap[bingo].end(), 
            [](const bool& b) { return b; })) break;
    }

    if (bingo >= MAX_ROW)
    {
        assert(!_manager.expired() && "_manager ptr expired");
        if (any_of(_tetromino->blocks.begin(), _tetromino->blocks.end(), 
            [](const shared_ptr<Block>& block) { return block->coord.ry >= INDEX_MAX_ROW; }))
        {
            _manager.lock()->request(RQ_GAMEOVER);
        }
        else
        {
            _manager.lock()->request(RQ_CONTINUE);
        }
        return;
    }

    deleteLine(bingo);
}

void TetrisMap::deleteLine(const int& line)
{
    for_each(_concreteMap[line].begin(), _concreteMap[line].end(), [this](shared_ptr<Block>& block)
    {
        _particleManager->add(block->sprite->getPosition());
        _collector(block);
        block->sprite->retain();
        block->sprite->removeFromParent();
        block = nullptr;
    });

    _particleManager->show();

    _conceptualMap[line].fill(false);

    fall(line);
}

// 删除行之后下落
void TetrisMap::fall(const int& line)
{
    for (int i = line-1; i >= INDEX_MIN_ROW; i--)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            if (_conceptualMap[i][j] == false) continue;

            // 빈칸 처리
            auto block = _concreteMap[i][j];
            _conceptualMap[i][j] = false;
            block->coord.ry -= 1;

            block->sprite->setPositionY(block->sprite->getPositionY() - BLOCK_SIZE);

            // 실질적 행번호 얻기
            const int &row = rowIndex(block->coord.ry);
            
            // 이동한 행 true, block 가리키기
            _conceptualMap[row][j] = true;
            _concreteMap[row][j] = block;

            // 아래 행으로 이사후 현재 행 nullptr
            _concreteMap[i][j] = nullptr;
        }
    }

    findBingo();
}

int TetrisMap::rowIndex(const int & i) const
{
    assert(i >= 0 && (MAX_ROW - i - 1) >= 0&& "row index out of range");
    return MAX_ROW - i - 1;
}

int TetrisMap::colIndex(const int & i) const
{
	assert(i >= 0 && (MAX_COL - i - 1) >= 0 && "column index out of range");
	return i;
}
