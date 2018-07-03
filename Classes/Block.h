#pragma once
#include "define.h"

struct TCoord
{
    int cx, ry;

    TCoord() : cx(0), ry(0) {};
    TCoord(int x, int y) : cx(x), ry(y) {}

    void operator=(const TCoord& block)
    {
        this->cx = block.cx;
        this->ry = block.ry;
    }

    bool operator==(const TCoord& block) const
    {
        return (this->cx == block.cx && this->ry == block.ry);
    }
};

struct Block
{
    TCoord coord;
    cocos2d::Sprite *sprite;

    Block() 
    { 
        sprite = Sprite::create();
    }
};

typedef std::array<std::shared_ptr<Block>, 4> Blocks;
