#include "Particle.h"
#include "ParticleSystem.h"


Particle::Particle()
{
	velocity = PxVec3(0,0,0);
	acceleration = PxVec3(0, 0, 0);
	transform = PxTransform(PxVec3(0, 0, 0));
	PxShape* shape = CreateShape(PxSphereGeometry(1));
	renderItem = new RenderItem(shape, &transform, Vector4(1, 0.5, 1, 1));
	genOrig = PxVec3(0, 0, 0);
	radius = 5000;
	lifeTime = 100;
	damping = 0.99;
	mass = 1;
}

Particle::Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double newMass, double time) :
	velocity(velo), acceleration(acel), mass(newMass), lifeTime(time)
{
	transform = PxTransform(pos);
	PxSphereGeometry geo(1);
	PxShape* shape = CreateShape(geo);

	radius = 5000;
	renderItem = new RenderItem(shape, &transform, Vector4(1, 0.5, 1, 1));
	RegisterRenderItem(renderItem);
	damping = 0.99;
}

Particle::Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double time) :
	lifeTime(time), acceleration(acel), velocity(velo)
{
	transform = PxTransform(pos);
	PxSphereGeometry geo(1);
	PxShape* shape = CreateShape(geo);

	radius = 5000;
	renderItem = new RenderItem(shape, &transform, Vector4(1, 0.5, 1, 1));
	RegisterRenderItem(renderItem);
	damping = 0.99;
}

// Constructor de copia
Particle::Particle(Particle const& p)
{
	velocity = p.velocity;
	acceleration = p.acceleration;
	transform = p.transform;
	//transform = PxTransform(p.transform.p);
	PxShape* shape = CreateShape(PxSphereGeometry(1));
	renderItem = new RenderItem(shape, &transform, Vector4(1, 0.5, 1, 1));
	genOrig = p.genOrig;
	radius = p.radius;
	lifeTime = p.lifeTime;
	damping = 0.99;
	mass = p.mass;
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::Integrate(double t)
{
	// SEMIEULER
	velocity = velocity * pow(damping, t) + acceleration * t;
	transform.p = transform.p + velocity * t;  // Actualizamos la posición
}

bool Particle::OnRadius()
{
	bool isOnRadius = ((transform.p - genOrig).magnitude()) < radius;
	return isOnRadius;
}

bool Particle::UpdateLifeTime(double t)
{
	lifeTime -= t;
	return (lifeTime <= 0);
}

void Particle::Update(double t, ParticleSystem& system)
{
	Integrate(t);
	if (UpdateLifeTime(t)) {		//!OnRadius() || 
		system.EliminateParticle(this);
	}
}
