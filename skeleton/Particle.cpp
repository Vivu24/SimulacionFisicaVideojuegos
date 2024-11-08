#include "Particle.h"
#include "ParticleSystem.h"

Particle::Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel, double newMass, double time) :
	velocity(velo), acceleration(acel), mass(newMass), lifeTime(time)
{
	transform = PxTransform(pos);
	PxSphereGeometry geo(1);
	PxShape* shape = CreateShape(geo);

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

	renderItem = new RenderItem(shape, &transform, Vector4(1, 0.5, 1, 1));
	RegisterRenderItem(renderItem);
	damping = 0.99;
}

Particle::Particle(Particle const& p)
{
	velocity = p.velocity;
	acceleration = p.acceleration;
	transform = PxTransform(p.transform.p);
	PxShape* shape = CreateShape(PxSphereGeometry(1));
	renderItem = new RenderItem(shape, &transform, Vector4(1, 0.5, 1, 1));
	genOrig = p.genOrig;
	radius = p.radius;
	lifeTime = p.lifeTime;
	damping = 0.99;
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
	//renderItem = nullptr;
}

void Particle::Integrate(double t)
{
	//SEMIEULER (Cambiar primera y segunda linea)
	velocity = velocity * pow(damping, t) + acceleration * t;
	transform.p = transform.p + velocity * t;
	// Actualizamos la aceleracion
	//transform.p += velocity * t;
	//std::cout << transform.p.x << " " << transform.p.y << " " << transform.p.z << "\n ";
	//// std::cout << myIt._Ptr;
	//velocity += acceleration * t;

	//// Actualizamos la velocidad
	//velocity *= pow(damping, t);
	
	// Actualizamos la posición del transform
	// transform.p = transform.p + velocity*t;
}

void Particle::SetAcceleration(PxVec3 dir)
{
	acceleration = dir;
}

void Particle::SetVelocity(PxVec3 vel)
{
	velocity = vel;
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
	cout << "UPDATE PARTICULA - Mass: " << mass << endl;
	//lifeTime -= t;
	Integrate(t);
	if (!OnRadius() || UpdateLifeTime(t)) system.EliminateParticle(this);
}
