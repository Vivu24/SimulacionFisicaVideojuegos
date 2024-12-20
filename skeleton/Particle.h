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
	Particle();
	Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double newMass, double time);	// Particula P1.2
	Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double time = 10);	// Particula Generador
	Particle(Particle const& p);	// Constructor de Copia

	~Particle();

	void Integrate(double t);

	bool UpdateLifeTime(double t);


	PxVec3 getAcceleration() { return acceleration; }
	void SetAcceleration(PxVec3 dir) { acceleration = dir; }
	void SetVelocity(PxVec3 vel) { velocity = vel; }

	void setRadius(float r) { radius = r; }
	float getRadius() { return radius; }

	void setPosition(PxVec3 p) { transform.p = p; }
	PxVec3 getPosition() { return transform.p; }

	void setVelocity(PxVec3 v) { velocity = v; }
	PxVec3 getVelocity() { return velocity; }

	void setIterator(list<Particle*>::iterator it) { myIt = it; }
	list<Particle*>::iterator getIterator() const { return myIt; }

	void setTransform(PxTransform tr) { transform = tr; }
	PxTransform getTransform() { return transform; }

	void setMass(double m) { mass = m; }
	double getMass() { return mass; }

	void Update(double t, ParticleSystem& system);

	bool OnRadius();

	void ApplyForce(PxVec3 force) { acceleration = force / mass; }

	void setRenderItem(RenderItem * ri) { renderItem = ri; }

	RenderItem* getRenderItem() { return renderItem; }

private:

	PxVec3 velocity;
	PxVec3 acceleration;
	double mass;
	double lifeTime;
	PxTransform transform;
	double damping;
	RenderItem* renderItem = nullptr;
	float radius;
	PxVec3 genOrig;
	list<Particle*>::iterator myIt;
};
