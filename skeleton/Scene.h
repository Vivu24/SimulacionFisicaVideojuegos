#pragma once
#include <vector>
#include "RigidBody.h"
#include "ForceGenerator.h"

class Scene {
public:
    Scene(PxPhysics* physics, PxScene* scene);
    virtual ~Scene();

    virtual void initScene();
    virtual void update(double t);
    virtual void keyPressed(unsigned char key, const PxTransform& camera);


protected:
    std::vector<RigidBody*> rigidBodies;
    std::vector<ForceGenerator*> forces;

    PxPhysics* gPhysics;
    PxScene* gScene;
};


