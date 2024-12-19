#pragma once
#include "ForceGenerator.h"

class GravityGenerator : public ForceGenerator
{
public:
    GravityGenerator(double g);

    PxVec3 applyForce(Particle* p) override;
    PxVec3 applyForce(RigidBody* rb) override;


    void update(double t) override {}       // Explota si no esta

private:
    double gravityForce_;

};
