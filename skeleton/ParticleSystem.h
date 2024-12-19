#pragma once
#include <iostream>
#include "RenderUtils.hpp"
#include "PxPhysicsAPI.h"
#include <cmath>
#include <list>
#include "Particle.h"
#include "NormalGenerator.h"
#include "UniformGenerator.h"
#include "ForceGenerator.h"
#include "GravityGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyForceGenerator.h"
#include "RigidBody.h"


using namespace physx;
using namespace std;

class ParticleSystem
{
public:
	ParticleSystem() {}  // Constructor por defecto

	~ParticleSystem();

	void Update(double t);

	void EliminateParticle(Particle* particle);
	void EliminateRigidBody(RigidBody* rigidBody);
	void AddParticle(Particle* p);
	void AddRigidBody(RigidBody* s);

	void CreateUniformGenerator(PxVec3 pos, PxVec3 direction, float rate, float range, float sr, spawnDistribution sd, float rat, float lifetime = 15.0f);
	void CreateNormalGenerator(PxVec3 pos, PxVec3 direction, float rate, PxVec3 des, float sr, spawnDistribution sd, float rat, float lifetime = 15.0f);

	//-----------------------------//
	// FUERZAS
	void Gravity(double g);
	void Wind(PxVec3 center, PxVec3 size, PxVec3 windVelocity, float rozCoef);
	void Whirlwind(PxVec3 center, PxVec3 size, float rozCoef, float intensity);
	// Explosion
	void Explosion(float intensity, float radius, float tau);
	void TriggerExplosion(PxVec3 position);

	// MUELLES
	void Spring();
	void Anchor();

	void Buoyancy(float h, float V, float d);
	//-----------------------------// 

	list<Particle*> particles;
	list<ParticleGenerator*> generators;
	vector<Particle*> particlesToErase;

	list<ForceGenerator*> forces;
	vector<ForceGenerator*> forcesToErase;


	list<RigidBody*> rigidBodies;
	vector<RigidBody*> rigidBodiesToErase;
};
