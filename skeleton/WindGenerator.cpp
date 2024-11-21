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
