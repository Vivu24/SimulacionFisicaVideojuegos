#include "NormalGenerator.h"

NormalGenerator::NormalGenerator(Particle* p, float rate, PxVec3 des, float sr, SpawnDistribution sp) :
	ParticleGenerator(p, rate, sr, sp), mediumVel(p->getVelocity()), desviationVel(des) {}

Particle* NormalGenerator::generate()
{
    normal_distribution<float> distX(mediumVel.x, desviationVel.x);
    normal_distribution<float> distY(mediumVel.y, desviationVel.y);
    normal_distribution<float> distZ(mediumVel.z, desviationVel.z);

    PxVec3 randomVelocity(distX(myRandom), distY(myRandom), distZ(myRandom));
    Particle* particle = new Particle(myParticle);

    particle->setVelocity(randomVelocity);
    particle->setPosition(calculatePosition());

    return particle;
}
