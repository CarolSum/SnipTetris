#include "headers.h"
#include "TetrisParticleManager.h"

TetrisParticleManager::TetrisParticleManager(Node *scene)
{
    _particles = make_unique<list<ParticleSystemQuad *>>();
    _particlePool = make_unique< list<ParticleSystemQuad * >>();

    for (int i = 0; i < 40; i++)
    {
        auto particle = ParticleSystemQuad::create("TetrisRes/particle.plist");
        particle->setTexture(Director::getInstance()->getTextureCache()->addImage("TetrisRes/particle_texture.png"));
        particle->stopSystem();
        scene->addChild(particle);
        _particlePool->emplace_back(particle);
    }
}

void TetrisParticleManager::add(const Vec2 & p)
{
    auto particle = *_particlePool->begin();
    _particlePool->pop_front();

    particle->setPosition(p);
    _particles->emplace_back(particle);
}

void TetrisParticleManager::show()
{
    for (const auto &particle : *_particles)
        particle->resetSystem();

    _particlePool->splice(_particlePool->end(), *_particles, _particles->begin(), _particles->end());
}