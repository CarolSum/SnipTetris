#include "headers.h"
#include "TetrisMap.h"
#include "TetrisBlockPool.h"
#include "TetrisGameScene.h"
#include "TetrisManager.h"

TetrisMap::TetrisMap(const shared_ptr<TetrisManager>& manager)
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

bool TetrisMap::isAccessible(const TCoord& _coord)
{
    array<bool, 4> checkList = 
    {
        _coord.cx < 0, _coord.ry < 0,
        _coord.cx >(MAX_COL - 1),
        _coord.ry >(MAX_ROW - 1)
    };

    if (!all_of(checkList.begin(), checkList.end(), [](const bool& _b) { return !_b; })) 
        return false;

    return _conceptualMap[rowIndex(_coord.ry)][_coord.cx] == false;
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

void TetrisMap::setManager(const shared_ptr<TetrisManager>& manager)
{
    _manager = manager;
}

bool TetrisMap::update()
{
    auto scene = _tetromino->blocks[0]->sprite->getParent()->getParent();

    for (auto &block : _tetromino->blocks)
    {
        _concreteMap[rowIndex(block->coord.ry)][block->coord.cx] = block;
        _conceptualMap[rowIndex(block->coord.ry)][block->coord.cx] = true;

        block->sprite->retain();
        block->sprite->removeFromParent();
		// 右偏移120
        block->sprite->setPositionX(BLOCK_SIZE * block->coord.cx + BLOCK_HALF + 240);
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
    assert((MAX_ROW - i - 1) >= 0 && "index out of range");
    return MAX_ROW - i - 1;
}
