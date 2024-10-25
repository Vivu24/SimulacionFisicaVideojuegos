#pragma once
#include <iostream>
#include "RenderUtils.hpp"
#include "PxPhysicsAPI.h"
#include <cmath>
#include <list>
#include "Particle.h"
#include "NormalGenerator.h"
#include "UniformGenerator.h"

using namespace physx;
using namespace std;

class ParticleSystem
{
public:
	ParticleSystem() {};

	~ParticleSystem();

	void Update(double t);
	void EliminateParticle(Particle* particle);
	void AddParticle(Particle* p);

	void CreateUniformGenerator(PxVec3 pos, PxVec3 direction, float rate, float range, float sr, SpawnDistribution sd, float rat, float lifetime = 15.0f);
	void CreateNormalGenerator(PxVec3 pos, PxVec3 direction, float rate, PxVec3 des, float sr, SpawnDistribution sd, float rat, float lifetime = 15.0f);
private:
	list<Particle*> particles;
	list<ParticleGenerator*> generators;
	vector<Particle*> particlesToErase;

	PxVec3 origin;
};

