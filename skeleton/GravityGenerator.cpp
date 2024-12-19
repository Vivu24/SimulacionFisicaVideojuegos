#include "GravityGenerator.h"

GravityGenerator::GravityGenerator(double g)
    : gravityForce_(g), ForceGenerator(GRAVITY, true) {}

PxVec3 GravityGenerator::applyForce(Particle* p)
{
    return p->getMass() * PxVec3(0, -gravityForce_, 0);
}

PxVec3 GravityGenerator::applyForce(RigidBody* rb)
{
    return rb->getActor()->getMass() * PxVec3(0, -gravityForce_, 0);

}
