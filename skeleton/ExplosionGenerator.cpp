#include "ExplosionGenerator.h"

ExplosionGenerator::ExplosionGenerator(ParticleSystem* pS, PxVec3 pos, PxVec3 a, double k, double t, double tau)
    : ForceGenerator(EXPLOSION), particleSystem(pS), position(pos), influenceArea(a), _k(k), currentTime(t), _tau(tau) {}

void ExplosionGenerator::update(double t)
{
    if (exploding) {
        // Calcula el factor de tiempo para el decaimiento de la explosión
        double factor_tiempo = exp(-currentTime / _tau);


        for (auto& particle : particleSystem->particles) {
            PxVec3 auxPos = particle->getPosition();
            PxVec3 distance = auxPos - position;  // Distancia hasta el centro de la explosion
            double r = distance.magnitude();

            double factor_fuerza = (_k / (r * r)) * factor_tiempo;

            // Normaliza la dir
            PxVec3 direccion = {
                distance.x / distance.magnitude(),
                distance.y / distance.magnitude(),
                distance.z / distance.magnitude()
            };

            // Fuerza Final
            force = direccion * factor_fuerza;
            particle->ApplyForce(force);
        }

        currentTime += t;
    }

    
}

void ExplosionGenerator::resetExplosion(PxVec3 pos) {
    exploding = true; // Empezar la explosión
    currentTime = 0;
    position = pos;
}

