#include "Scene.h"

Scene::Scene(PxPhysics* physics, PxScene* scene)
    : gPhysics(physics), gScene(scene) {}

Scene::~Scene() {
    for (auto rb : rigidBodies) {
        delete rb;
    }
    for (auto f : forces) {
        delete f;
    }
}

void Scene::initScene()
{
}

void Scene::update(double t) {
    for (RigidBody* rb : rigidBodies) {
        if (rb!= nullptr && rb->getActor()) {
            rb->getActor()->clearForce(PxForceMode::eFORCE);

            for (ForceGenerator* force : forces) {
                if (force && force->isAlive()) {
                    PxVec3 forceVec = force->applyForce(rb);
                    rb->getActor()->addForce(forceVec);
                }
            }
        }
    }
}

void Scene::keyPressed(unsigned char key, const PxTransform& camera)
{
}
