#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_length, Particle* other) :
	ForceGenerator(SPRING, true)
{
	_k = k;
	_resting_length = resting_length;
	_other = other;
}

PxVec3 SpringForceGenerator::applyForce(Particle* particle)
{
	// Particle is the particle to apply the force
	PxVec3 relative_pos_vector = _other->getPosition() - particle->getPosition();
	PxVec3 force;

	// normalize: Normalize the relative_pos_vector and returns its length
	const float lenght = relative_pos_vector.normalize();
	const float delta_x = lenght - _resting_length;

	force = relative_pos_vector * delta_x * _k;

	return force;
}

PxVec3 SpringForceGenerator::applyForce(RigidBody* p)
{
	//Particle is the particle to apply the force
		PxVec3 relative_pos_vector = _other->getPosition() - p->getActor()->getGlobalPose().p;
	PxVec3 force;

	// normalize: Normalize the relative_pos_vector and returns its length
	const float lenght = relative_pos_vector.normalize();
	const float delta_x = lenght - _resting_length;

	force = relative_pos_vector * delta_x * _k;

	return force;
}


