#include "WhirlwindGenerator.h"

WhirlwindGenerator::WhirlwindGenerator(PxVec3 center, PxVec3 size, float rozCoef, float intensity)
    : WindGenerator(center, size, PxVec3(0, 0, 0), rozCoef), intensity(intensity)
{
    type = TORBELLINO;
}

void WhirlwindGenerator::calculateVelocity(PxVec3 pPos)
{
    PxVec3 rotationDir = 
        PxVec3(-(pPos.z - windCenter.z),   // Dirección en el eje X (dependiendo de la diferencia en Z)
        0,                       // Mantenemos Normal el Y
        pPos.x - windCenter.x        // Dirección en el eje Z (dependiendo de la diferencia en X)
    );

    // Velocidad final
    windVelocity = intensity * rotationDir;
}

PxVec3 WhirlwindGenerator::applyForce(Particle* p)
{
    // Si está en el torbellino
    if (isInside(p->getPosition())) {
        calculateVelocity(p->getPosition());
        return rozCoef * (windVelocity - p->getVelocity());
    }
    else return PxVec3(0, 0, 0);    // Fuera no hay fuerza
}

PxVec3 WhirlwindGenerator::applyForce(RigidBody* rb) {
    if (!rb || !rb->getActor()) return PxVec3(0, 0, 0);

    if (!isInside(rb->getActor()->getGlobalPose().p)) {
        return PxVec3(0, 0, 0);
    }

    calculateVelocity(rb->getActor()->getGlobalPose().p);
    PxVec3 relativeVelocity = windVelocity - rb->getActor()->getLinearVelocity();
    PxVec3 force = rozCoef * relativeVelocity;

    return force;
}

