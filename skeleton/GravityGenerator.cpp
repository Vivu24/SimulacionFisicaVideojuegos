#include "GravityGenerator.h"

GravityGenerator::GravityGenerator(double g, double t) : gravity_(g), 
	ForceGenerator(t, true)
{
}

Vector3 GravityGenerator::applyForce(Particle* p)
{
	Vector3 force = p->getMass() * Vector3(0, -gravity_, 0);
	cout << p->getMass() << endl;
	return force;
}
