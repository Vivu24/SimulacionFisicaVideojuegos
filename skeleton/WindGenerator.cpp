#include "WindGenerator.h"

bool WindGenerator::isInside(PxVec3 pPos)
{
    return !(pPos.x < windCenter.x - windSize.x || pPos.x > windCenter.x + windSize.x ||
        pPos.y < windCenter.y - windSize.y || pPos.y > windCenter.y + windSize.y ||
        pPos.z < windCenter.z - windSize.z || pPos.z > windCenter.z + windSize.z);
}

PxVec3 WindGenerator::applyForce(Particle* p)
{
    if (isInside(p->getPosition())) {
        calculateVelocity(p->getPosition());
        return rozCoef * (windVelocity - p->getVelocity());
    }
    return PxVec3(0, 0, 0);
}

PxVec3 WindGenerator::applyForce(RigidBody* rb)
{
    if (!rb || !rb->getActor()) return PxVec3(0, 0, 0);

    if (!isInside(rb->getActor()->getGlobalPose().p)) {
        return PxVec3(0, 0, 0);
    }

    calculateVelocity(rb->getActor()->getGlobalPose().p);
    PxVec3 relativeVelocity = windVelocity - rb->getActor()->getLinearVelocity();
    PxVec3 force = rozCoef * relativeVelocity;

    return force;
}
