#pragma once

#include "MyVector3.h"
#include "RenderUtils.hpp"
#include "PxPhysicsAPI.h"
#include <cmath>

using namespace physx;
using namespace std;

class Particle
{
public:
	Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double newMass, double time);
	~Particle();

	void Integrate(double t);

	void SetAcceleration(PxVec3 dir);
	void SetVelocity(PxVec3 vel);
	void UpdateLifeTime(double t);

	PxVec3 getPose() { return pose; }

private:
	PxVec3 velocity;
	PxVec3 acceleration;
	PxVec3 pose;

	double mass;
	double lifeTime;

	PxTransform* transform = nullptr;

	double damping;

	RenderItem* renderItem = nullptr;
};

