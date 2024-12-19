#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <list>

using namespace std;
using namespace physx;

class RigidBody
{
public:

    RigidBody(PxPhysics* gPhysics, PxScene* gScene);

    // Constructor
    RigidBody(PxPhysics* gPhysics, PxScene* gScene, const PxVec3& position, const PxVec3& linearVelocity,
        const PxVec3& angularVelocity, const PxVec3& dimensions, double masa, Vector4 color, double t);

    RigidBody(PxPhysics* gPhysics, PxScene* gScene, const PxVec3& position, 
        float radius, double masa, Vector4 color, double t);
    // Destructor
    ~RigidBody();

    PxRigidDynamic* getActor() { return actor; }
    PxVec3 getPos() { return actor->getGlobalPose().p; }

    void setIterator(list<RigidBody*>::iterator it) { myIt = it; }
    list<RigidBody*>::iterator getIterator() const { return myIt; }

    void moveRight(float velocidad);
    void moveLeft(float velocidad);
    void stop();

    PxVec3 calculateTensor(float m);

    bool isDestroyable(double t) {
        lifeTime -= t;
        return lifeTime <= 0;

    }


    double lifeTime;

private:

    PxRigidDynamic* actor;   // Actor dinámico de PhysX
    RenderItem* renderItem; // Item de renderizado
    PxShape* shape;
    list<RigidBody*>::iterator myIt;

};