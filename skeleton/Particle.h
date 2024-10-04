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
	Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double newMass);
	~Particle();

	void Integrate(double t);
	void SetAcceleration(PxVec3 dir);

private:
	PxVec3 velocity;
	PxVec3 acceleration;
	PxVec3 pose;

	double mass;

	PxTransform* transform = nullptr;

	double damping;

	RenderItem* renderItem = nullptr;
};

