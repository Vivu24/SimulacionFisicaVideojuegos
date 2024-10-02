#include "Particle.h"

Particle::Particle(PxVec3 pos, PxVec3 velo, PxVec3 acel) : pose(pos), velocity(velo), acceleration(acel)
{
	transform = new PxTransform(pose);
	PxSphereGeometry geo(1);
	PxShape* shape = CreateShape(geo);

	renderItem = new RenderItem(shape, transform, Vector4(1, 0.5, 1, 1));
	RegisterRenderItem(renderItem);
	damping = 0.99;
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
	delete renderItem;
	delete transform;
}

void Particle::Integrate(double t)
{
	// Actualizamos la velocidad
	velocity = velocity * pow(damping, t) + acceleration * t;
	
	// ACtualizamos la aceleracion
	pose = pose + velocity * t;
	// Actualizamos la posici�n del transform
	transform->p = transform->p + velocity*t;
}