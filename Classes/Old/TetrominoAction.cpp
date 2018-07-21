#include "Headers.h"
#include "TetrisMap.h"
#include "TetrominoFactory.h"
#include "TetrominoAction.h"
#include "TetrisGameScene.h"
#include "GameManager.h"

TetrominoAction::~TetrominoAction()
{
}

TetrominoAction::TetrominoAction(const shared_ptr<GameManager>& manager)
    :_manager(manager)
{
}

void TetrominoAction::init(TetrisGameScene * scene)
{
    assert(!_manager.expired() && "_manager ptr expired");
    _tetrisMap = _manager.lock()->getTetrisMap();
}

void TetrominoAction::setManager(const shared_ptr<GameManager>& manager)
{
    _manager = manager;
}

void TetrominoAction::requestUpdate()
{
    _tetrisMap->update();
}

bool TetrominoAction::fall()
{
    if (!mapCheck(DIR::DOWN))
    {
        requestUpdate();
        return false;
    }

    _tetromino->axis->setPositionY(_tetromino->axis->getPositionY() - BLOCK_SIZE);

    std::for_each(_tetromino->blocks.begin(), _tetromino->blocks.end(), 
        [](const shared_ptr<Block>& block) { block->coord.ry -= 1; });

    return true;
}

void TetrominoAction::move(const int & direct)
{
    if (!mapCheck(direct))
		return;

	auto vec = 0;
	if (direct == DIR::LEFT) vec = -1;
	if (direct == DIR::RIGHT) vec = 1;

    _tetromino->axis->setPositionX(_tetromino->axis->getPositionX() + BLOCK_SIZE * vec);

    std::for_each(_tetromino->blocks.begin(), _tetromino->blocks.end(), 
        [vec](const shared_ptr<Block>& block) { block->coord.cx += vec; });
}

void TetrominoAction::rotate()
{
    float &&targetDegree = _tetromino->axis->getRotation() - 90.0f;
    const auto degree = wrap(targetDegree, _tetromino->shape->getMaxAngle(), 0.f);
    const auto index = static_cast<int>(fabs(degree) / 90.0f);

    auto k = 0;

    array<coord_t, 4> simulArray;

    for (int i = 0; i < 4; i++)
    {
        auto block = _tetromino->blocks[i];
        simulArray[i].cx = block->coord.cx + _tetromino->shape->referToRotationTable(index, k++);
        simulArray[i].ry = block->coord.ry + _tetromino->shape->referToRotationTable(index, k++);
    }

    if (rotationSimulate(simulArray))
        _tetromino->axis->setRotation(degree);
}

bool TetrominoAction::rotationSimulate(array<coord_t, 4>& _simulArray)
{
    coord_t corrVal;
    getLocationCorrectionValue(_simulArray, corrVal);

    if (any_of(_simulArray.begin(), _simulArray.end(), [&corrVal, this](coord_t& _coord) 
    {
        _coord.cx += corrVal.cx; _coord.ry += corrVal.ry;
        return !_tetrisMap->isAccessible(_coord);
    })) return false;

    for (int i = 0; i < 4; i++)
        _tetromino->blocks[i]->coord = _simulArray[i];

    _tetromino->axis->setPositionX(_tetromino->axis->getPositionX() + BLOCK_SIZE * corrVal.cx);
    _tetromino->axis->setPositionY(_tetromino->axis->getPositionY() + BLOCK_SIZE * corrVal.ry);
    return true;
}

void TetrominoAction::getLocationCorrectionValue(const array<coord_t, 4>& _simulArray, coord_t & _correctionVal)
{
    auto distance = 0;
    for (const auto &coord : _simulArray)
    {
        if (coord.cx < TETRIS::LEFT_LIMIT)
        {
            distance = TETRIS::LEFT_LIMIT - coord.cx;
            _correctionVal.cx = _correctionVal.cx < distance ? distance : _correctionVal.cx;
        }

        if (coord.cx > TETRIS::RIGHT_LIMIT)
        {
            distance = TETRIS::RIGHT_LIMIT - coord.cx;
            _correctionVal.cx = _correctionVal.cx > distance ? distance : _correctionVal.cx;
        }

        if (coord.ry < TETRIS::BOTTOM_LIMIT)
        {
            distance = TETRIS::BOTTOM_LIMIT - coord.ry;
            _correctionVal.ry = _correctionVal.ry < distance ? distance : _correctionVal.ry;
        }
    }
}

bool TetrominoAction::hardDrop()
{
    list<shared_ptr<Block>> subjects;

    for (auto &subject : _tetromino->blocks)
    {
        auto count = count_if(_tetromino->blocks.begin(), _tetromino->blocks.end(),
            [&subject](const shared_ptr<Block>& block) {
            if (subject->coord == block->coord) return false;
            return subject->coord.cx == block->coord.cx && (subject->coord.ry - 1) == block->coord.ry;
        });

        if (count == 0) subjects.push_back(subject);
    }

    coord_t tmp;
    vector<int> distance;

    for (const auto& subject : subjects) {
        int d = 0;

        for (int row = subject->coord.ry - 1; row >= 0; row--)
        {
            tmp.cx = subject->coord.cx; tmp.ry = row;
            if (_tetrisMap->isAccessible(tmp) == false) break;
            d++;
        }
        distance.push_back(d);
    }

    auto minDistance = min_element(distance.begin(), distance.end());

    Vec2 pos(_tetromino->axis->getPositionX(),
        _tetromino->axis->getPositionY() - BLOCK_SIZE * (*minDistance));

    auto duration = *minDistance * 0.003f + 0.016f;
    auto drop = MoveTo::create(duration, pos);
    auto requestor = CallFunc::create(CC_CALLBACK_0(TetrominoAction::requestUpdate, this));
    auto seq = Sequence::create(drop, requestor, NULL);

    std::for_each(_tetromino->blocks.begin(), _tetromino->blocks.end(),
        [&minDistance](const shared_ptr<Block>& block) 
    { 
        block->coord.ry -= (*minDistance);
    });

    _tetromino->axis->runAction(seq);
    return true;
}

bool TetrominoAction::mapCheck(const int& dir)
{
    return all_of(_tetromino->blocks.begin(), _tetromino->blocks.end(),
        [&dir, this](const shared_ptr<Block>& block) {
        coord_t coord;

        if (dir == DIR::DOWN) {
            coord.cx = block->coord.cx;
            coord.ry = block->coord.ry - 1;
        }
        else if (dir == DIR::LEFT)
        {
            coord.cx = block->coord.cx - 1;
            coord.ry = block->coord.ry;
        }
		else if (dir == DIR::RIGHT)
		{
			coord.cx = block->coord.cx + 1;
			coord.ry = block->coord.ry;
		}

        return _tetrisMap->isAccessible(coord);
    });
}
