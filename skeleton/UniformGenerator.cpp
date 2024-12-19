#include "UniformGenerator.h"

UniformGenerator::UniformGenerator(Particle* p, float rate, float range, float spawnRange, spawnDistribution sp) :
    ParticleGenerator(p, rate, spawnRange, sp)
{
    PxVec3 vel = p->getVelocity();    // Velocidad de la partícula base
    PxVec3 dev = PxVec3(range, range, range);  // Rango de dispersión de velocidad
    minVelocity = vel - dev;
    maxVelocity = vel + dev;
}

Particle* UniformGenerator::generate()
{
    // Generar velocidades aleatorias dentro del rango
    uniform_real_distribution<float> distX(minVelocity.x, maxVelocity.x);
    uniform_real_distribution<float> distY(minVelocity.y, maxVelocity.y);
    uniform_real_distribution<float> distZ(minVelocity.z, maxVelocity.z);

    PxVec3 randomVelocity(distX(myRandom), distY(myRandom), distZ(myRandom));

    // Crear la partícula y establecer sus propiedades
    Particle* particle = new Particle(myParticle);

    // Establecer la velocidad y la posición
    particle->setVelocity(randomVelocity);
    particle->setPosition(calculatePosition());

    // Establecer la masa aleatoria dentro del rango (±3 unidades de la masa de referencia)
    float baseMass = 10.0f;  // Masa de referencia
    uniform_real_distribution<float> distMass(baseMass - 3.0f, baseMass + 3.0f); // Rango de masa de ±3 unidades
    float randomMass = distMass(myRandom);  // Generar la masa aleatoria dentro del rango
    particle->setMass(randomMass);  // Establecer la masa aleatoria de la partícula

    return particle;
}
