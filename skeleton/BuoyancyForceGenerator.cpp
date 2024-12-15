#include "BuoyancyForceGenerator.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float V, float d) :
	ForceGenerator(BUOYANCY, true)
{
	_height = h;
	_volume = V;
	_liquid_density = d;
}

PxVec3 BuoyancyForceGenerator::applyForce(Particle* p)
{
	float h = p->getPosition().y;
	float h0 = _liquid_particle->getPosition().y;

	PxVec3 f(0, 0, 0);
	float inmersed = 0.0;
	if (h - h0 > _height * 0.5) {
		inmersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		// Totally Immersed
		inmersed = 1.0;
	}
	else {
		inmersed = (h0 - h) / _height + 0.5;
	}

	f.y = _liquid_density * _volume * inmersed * 9.8;

	return f;
}