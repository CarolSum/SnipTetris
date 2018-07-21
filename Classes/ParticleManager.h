#pragma once

#include <memory>
#include <list>

#include "cocos2d.h"
USING_NS_CC;

using std::unique_ptr;
using std::make_unique;
using std::list;

class ParticleManager
{
public:
    ParticleManager(Node *node);
  
public:
    void add(const Vec2& p);
    void show();

private:
    unique_ptr<list<ParticleSystemQuad *>> _particles;
    unique_ptr<list<ParticleSystemQuad *>> _particlePool;
};
