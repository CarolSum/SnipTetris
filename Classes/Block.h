#pragma once

#include "cocos2d.h"
USING_NS_CC;

enum DIRECTION { LEFT = -1, DOWN = 0, RIGHT = 1 };
enum COLOR { RED = 0, PINK = 1, YELLOW = 2, ORANGE = 3, BLUE = 4, GREEN = 5, SKYBLUE = 6 };
enum STATE { GAMEOVER, CONTINUE, RESTART };

// Grid坐标（垂直坐标系，左下角格子为原点格子(0,0)）
struct coord_t
{
    int cx = 0, ry = 0;

    bool operator==(const coord_t& block) const
    {
        return cx == block.cx && ry == block.ry;
    }
};

// Grid的每一个单元格子
struct Block
{
	COLOR color = COLOR::RED;
    cocos2d::Sprite *sprite = cocos2d::Sprite::create(); // 游戏对象
};

const int COLOR_SIZE = 7;

// 不同颜色的块
class BlockFramePool
{
private:
	BlockFramePool()
	{
		char colorFile[20] = { 0, };
		for (unsigned int i = 0; i < COLOR_SIZE; i++)
		{
			sprintf_s(colorFile, "c%d.png", i);
			_pool[i] = SpriteFrameCache::getInstance()->getSpriteFrameByName(colorFile);
		}
	}

public:
	static BlockFramePool* getInstance()
	{
		static BlockFramePool p;
		return &p;
	}

	SpriteFrame *getBlockFrame(const int& color) const { return _pool[color]; }

private:
	SpriteFrame * _pool[COLOR_SIZE];
};

const float BLOCK_SIZE = 30.0f;
const float BLOCK_HALF = BLOCK_SIZE / 2;

const int MAX_COL = 20;
const int MAX_ROW = 24;
const int MAX_ALIVE_ROW = 22; // 存在非新生成的格子位于大于此行的位置时则游戏结束

const int INDEX_MAX_ROW = MAX_ALIVE_ROW - 1;
const int INDEX_MIN_ROW = MAX_ROW - MAX_ALIVE_ROW;

const int LEFT_LIMIT = 0;
const int RIGHT_LIMIT = MAX_COL - 1;
const int BOTTOM_LIMIT = 0;
const int TOP_LIMIT = MAX_ROW - 1;
