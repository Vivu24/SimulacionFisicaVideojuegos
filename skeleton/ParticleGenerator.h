#pragma once
#include "Particle.h"
#include <random>
#include "ForceGenerator.h"


// Tipo de Distribución
enum spawnDistribution {
    UNIFORM,
    NORMAL
};

class ParticleGenerator
{
public:
    ParticleGenerator(Particle* p, float rate, float sr, spawnDistribution sd);

    virtual Particle* generate() = 0;

    PxVec3 calculatePosition();

    void update(double t, ParticleSystem& pS);

    float getEmissionRate() const { return creationVelocity; }
protected:
    mt19937 myRandom;
    float creationVelocity;
    float lastTimed = 0;
    float spawnDistance;
    spawnDistribution mySpawnDistribution;
    Particle myParticle;

    list<Particle*> emittedParticles;
    vector<ForceGenerator*> linkedForces;
};
