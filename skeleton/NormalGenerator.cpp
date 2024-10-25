#include "NormalGenerator.h"

NormalGenerator::NormalGenerator(Particle* p, float rate, PxVec3 devV, float spawnR, SpawnDistribution sp) :
	ParticleGenerator(p, rate, spawnR, sp), mediumVel(p->getVelocity()), desviationVel(devV) {}

Particle* NormalGenerator::generate()
{
    normal_distribution<float> distX(mediumVel.x, desviationVel.x);
    normal_distribution<float> distY(mediumVel.y, desviationVel.y);
    normal_distribution<float> distZ(mediumVel.z, desviationVel.z);

    PxVec3 randomVelocity(distX(random_engine), distY(random_engine), distZ(random_engine));
    Particle* particle = new Particle(model_particle);

    particle->setVelocity(randomVelocity);
    particle->setPosition(calculatePosition());

    return particle;
}
