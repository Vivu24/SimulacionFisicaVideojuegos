#pragma once
#include <vector>
#include "Particle.h"

enum forceType {
    GRAVITY,
    WIND,
    TORBELLINO,
    EXPLOSION,
    NONE
};

class ForceGenerator
{
public:
    ForceGenerator(forceType forcT, bool alive = true)
        : type(forcT), alive(alive) {}

    virtual ~ForceGenerator() {}

    // Métodos Virtuales para herencia
    virtual PxVec3 applyForce(Particle* p) = 0;
    virtual void update(double t) = 0;

    bool isAlive() const { return alive; }
    forceType getType() const { return type; }

protected:
    bool alive;
    forceType type;
};
