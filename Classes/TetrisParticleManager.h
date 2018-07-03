#pragma once
#include "headers.h"

class TetrisParticleManager
{
public:
    TetrisParticleManager(Node *scene);
  
public:
    void add(const Vec2& p);
    void show();

private:
    unique_ptr<list<ParticleSystemQuad *>> _particles;
    unique_ptr<list<ParticleSystemQuad *>> _particlePool;
};
