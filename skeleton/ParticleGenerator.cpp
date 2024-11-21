#include "ParticleGenerator.h"
#include "ParticleSystem.h"

ParticleGenerator::ParticleGenerator(Particle* p, float rate, float sr, spawnDistribution sd) :
    myParticle(*p), creationVelocity(rate), spawnDistance(sr), mySpawnDistribution(sd)
{
    random_device rd;
    myRandom.seed(rd());
}

PxVec3 ParticleGenerator::calculatePosition()
{
    PxVec3 initialPosition = myParticle.getPosition();
    PxVec3 endPosition;

    // UNIFORME
    if (mySpawnDistribution == UNIFORM) {
        uniform_real_distribution<float> distX(initialPosition.x - spawnDistance, initialPosition.x + spawnDistance);
        uniform_real_distribution<float> distY(initialPosition.y - spawnDistance, initialPosition.y + spawnDistance);
        uniform_real_distribution<float> distZ(initialPosition.z - spawnDistance, initialPosition.z + spawnDistance);

        endPosition = PxVec3(distX(myRandom), distY(myRandom), distZ(myRandom));
    }
    // NORMAL
    else if (mySpawnDistribution == NORMAL) {
        normal_distribution<float> distX(initialPosition.x, spawnDistance);
        normal_distribution<float> distY(initialPosition.y, spawnDistance);
        normal_distribution<float> distZ(initialPosition.z, spawnDistance);

        endPosition = PxVec3(distX(myRandom), distY(myRandom), distZ(myRandom));
    }

    return endPosition;
}


void ParticleGenerator::update(double t, ParticleSystem& pS) {
    lastTimed += t;

    int auxParticles = static_cast<int>(lastTimed * creationVelocity);

    for (int i = 0; i < auxParticles; ++i) {
        Particle* newParticle = generate();
        if (newParticle) {
            pS.AddParticle(newParticle);
        }
    }

    lastTimed -= auxParticles / creationVelocity;
}
