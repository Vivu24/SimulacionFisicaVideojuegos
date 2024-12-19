#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include "ParticleSystem.h"

class ExplosionGenerator : public ForceGenerator
{
public:
    ExplosionGenerator(ParticleSystem* pS, PxVec3 pos, PxVec3 a, double k, double t, double tau);

    void update(double t) override;
    PxVec3 applyForce(Particle* p) override { return PxVec3(0, 0, 0); };        // A (0,0,0) para que no pete
    PxVec3 applyForce(RigidBody* p) override { return PxVec3(0, 0, 0); };

    //void explosion() { exploding = true; }  

    void resetExplosion(PxVec3 pos);


    ParticleSystem* particleSystem;
    bool exploding = false;
    float _k;  // Constante de la explosión
    double currentTime;
    double explosionTime;
    PxVec3 force;
    PxVec3 position;
    PxVec3 influenceArea;
};
