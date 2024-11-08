#include "UniformGenerator.h"

UniformGenerator::UniformGenerator(Particle* p, float rate, float range, float spawnRange, SpawnDistribution sp) :
    ParticleGenerator(p, rate, spawnRange, sp)
{
    PxVec3 vel = p->getVelocity();
    PxVec3 dev = PxVec3(range, range, range);
    minVelocity = vel - dev;
    maxVelocity = vel + dev;
}

Particle* UniformGenerator::generate()
{
    uniform_real_distribution<float> distX(minVelocity.x, maxVelocity.x);
    uniform_real_distribution<float> distY(minVelocity.y, maxVelocity.y);
    uniform_real_distribution<float> distZ(minVelocity.z, maxVelocity.z);

    PxVec3 randomVelocity(distX(myRandom), distY(myRandom), distZ(myRandom));

    Particle* particle = new Particle(myParticle);

    particle->setVelocity(randomVelocity);
    particle->setPosition(calculatePosition());
    particle->setMass(10);

    return particle;
}
