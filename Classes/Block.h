#pragma once

#include "cocos2d.h"

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
    coord_t coord; // 在grid中的坐标
    cocos2d::Sprite *sprite = cocos2d::Sprite::create(); // 游戏对象
};

// typedef std::array<std::shared_ptr<Block>, 5> FiveBlocks; // 五格骨牌

const float BLOCK_SIZE = 25.0f;
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
