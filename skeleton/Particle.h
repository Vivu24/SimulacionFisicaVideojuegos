#pragma once

#include <iostream>
#include "MyVector3.h"
#include "RenderUtils.hpp"
#include "PxPhysicsAPI.h"
#include <cmath>
#include <list>

using namespace physx;
using namespace std;

class ParticleSystem;

class Particle
{
public:
	Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double newMass, double time);	// Particula P1.2
	Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double time = 10);	// Particula Generador
	Particle(Particle const& p);
	~Particle();

	void Integrate(double t);

	void SetAcceleration(PxVec3 dir);
	void SetVelocity(PxVec3 vel);
	bool UpdateLifeTime(double t);

	void setRadius(float r) { radius = r; }
	float getRadius() { return radius; }

	void setPosition(PxVec3 p) { transform.p = p; }
	PxVec3 getPosition() { return transform.p; }

	//PxVec3 getPose() { return pose; }

	void setVelocity(PxVec3 v) { velocity = v; }
	PxVec3 getVelocity() { return velocity; }

	void setIterator(list<Particle*>::iterator it) { myIt = it; }
	list<Particle*>::iterator getIterator() const { return myIt; }

	void Update(double t, ParticleSystem& system);

	bool OnRadius();

private:
	PxVec3 velocity;
	PxVec3 acceleration;
	//PxVec3 pose;

	double mass;
	double lifeTime;

	PxTransform transform;

	double damping;

	RenderItem* renderItem = nullptr;

	float radius;
	PxVec3 genOrig;

	list<Particle*>::iterator myIt;
};

