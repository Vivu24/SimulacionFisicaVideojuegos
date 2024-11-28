#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include <cmath>

class SpringForceGenerator : public ForceGenerator {
public:
    SpringForceGenerator(float k, float resting_length, Particle* other)
        : ForceGenerator(SPRING), _k(k), _resting_length(resting_length), _other(other) {}

    void update(double t) override {}

    PxVec3 applyForce(Particle* p) override {
        if (!p || !_other || p->getMass() == 0.0 || _other->getMass() == 0.0) {
            return PxVec3(0.0f, 0.0f, 0.0f);
        }

        PxVec3 rel_pos = _other->getPosition() - p->getPosition();

        float length = rel_pos.magnitude();

        if (length == 0.0f) {
            return PxVec3(0.0f, 0.0f, 0.0f);
        }

        PxVec3 direction = rel_pos / length;

        float delta_x = length - _resting_length;

        PxVec3 force = direction * (-_k * delta_x);

        p->ApplyForce(force);

        return force;
    }

    inline void setK(float k) { _k = k; }
    inline float getK() const { return _k; }

    inline void setRestingLength(float resting_length) { _resting_length = resting_length; }
    inline float getRestingLength() const { return _resting_length; }

    // Destructora
    virtual ~SpringForceGenerator() {}

private:
    float _k;
    float _resting_length;
    Particle* _other;
};
