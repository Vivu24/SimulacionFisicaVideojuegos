#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include <cmath>

class SpringForceGenerator : public ForceGenerator {
public:

	// MI HERENCIA
	void update(double t) override {};

	SpringForceGenerator(double k, double resting_length, Particle* other);

	PxVec3 applyForce(Particle* p) override;

	inline void setK(double k) { _k = k; }

	virtual ~SpringForceGenerator() {}

protected:
	double _k;
	double _resting_length;
	Particle* _other;
};
