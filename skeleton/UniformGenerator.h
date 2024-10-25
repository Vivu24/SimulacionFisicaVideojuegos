#pragma once
#include "ParticleGenerator.h"
#include <random>

class UniformGenerator : public ParticleGenerator
{
public:
    UniformGenerator(Particle* p, float rate, float range, float spawnRange, SpawnDistribution sp);

    Particle* generate() override;

private:
    PxVec3 minVelocity;
    PxVec3 maxVelocity;
};

