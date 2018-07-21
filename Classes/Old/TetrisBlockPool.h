#pragma once
#include "Headers.h"

class TetrisBlockPool
{
private:
    TetrisBlockPool() 
    {
        _pool = make_unique<list<shared_ptr<Block>>>();
    }

public:
    static TetrisBlockPool* getInstance()
    {
        static TetrisBlockPool p;
        return &p;
    }

    int size() const { return  _pool->size(); }
    void collect(const shared_ptr<Block>& source)
    {
        _pool->emplace_back(source);
    }

    shared_ptr<Block> getBlock()
    {
        if (_pool->size() == 0) return make_shared<Block>();
		
        auto block = _pool->front();
        _pool->pop_front();

        return block;
    }

private:
    unique_ptr<list<shared_ptr<Block>>> _pool;
};

class TetrisColorPool
{
private:
    TetrisColorPool()
    {
        char colorFile[10] = { 0, };
        for (unsigned int i=0; i < _pool.size(); i++)
        {
            sprintf_s(colorFile, "c%d.png", i);
            _pool[i] = SpriteFrameCache::getInstance()->getSpriteFrameByName(colorFile);
        }
    }

public:
    static TetrisColorPool* getInstance()
    {
        static TetrisColorPool p;
        return &p;
    }

    SpriteFrame *getColor(const int& color) const { return _pool[color]; }

private:
    array <SpriteFrame *, 7> _pool;
};