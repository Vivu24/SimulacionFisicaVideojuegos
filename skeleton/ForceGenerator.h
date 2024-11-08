#pragma once
#include <vector>
#include "Particle.h"


class ForceGenerator
{
public:
	ForceGenerator(double t, bool alive);
	~ForceGenerator() {};

	// Métodos Virtuales
	virtual Vector3 applyForce(Particle* p) = 0;
	virtual void update(double t) = 0;

	bool isAlive() { return alive_; }
protected:
	double time_;
	bool alive_;
};

