#pragma once
#include "WindGenerator.h"

class WhirlwindGenerator : public WindGenerator
{

public:
    WhirlwindGenerator(PxVec3 center, PxVec3 size, float rozCoef, float intensity);

    PxVec3 applyForce(Particle* p) override;
    PxVec3 applyForce(RigidBody* rb) override;

    void calculateVelocity(PxVec3 pPos);

private:
    float intensity;
};


