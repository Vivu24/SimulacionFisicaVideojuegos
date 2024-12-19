#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float h, float V, float d);

	virtual PxVec3 applyForce(Particle* p) override;
	PxVec3 applyForce(RigidBody* p) override { return PxVec3(0, 0, 0); };


	virtual void update(double t) override {};

	virtual ~BuoyancyForceGenerator() {};

	void setLiquidParticle(Particle* p) { _liquid_particle = p; }

protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle;	// For representation
};