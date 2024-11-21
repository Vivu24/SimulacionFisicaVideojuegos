#pragma once
#include "ForceGenerator.h"

class WindGenerator : public ForceGenerator
{

public:
    WindGenerator(PxVec3 center, PxVec3 size, PxVec3 windVelocity, double rozCoef)
        : ForceGenerator(WIND), windCenter(center), windSize(size), windVelocity(windVelocity), rozCoef(rozCoef)
    {}

    bool isInside(PxVec3 pPos);
    PxVec3 applyForce(Particle* p) override;

    void update(double t) override {}

protected:
    PxVec3 windCenter;
    PxVec3 windSize;
    PxVec3 windVelocity;
    double rozCoef;

    // SI Borro y en cpp aparece otra particula quieta
    void calculateVelocity(PxVec3 pPos) {}
};
