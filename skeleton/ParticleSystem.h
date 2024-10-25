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

	void CreateUniformGenerator(Vector3 pos, Vector3 direction, float rate, float range, float spawnR, SpawnDistribution sp, float rat, float lifetime = 10.0f);
	void CreateNormalGenerator(Vector3 pos, Vector3 direction, float rate, Vector3 dev, float spawnR, SpawnDistribution sp, float rat, float lifetime = 10.0f);
private:
	list<Particle*> particles;
	list<ParticleGenerator*> generators;
	vector<Particle*> particlesToErase;

	PxVec3 origin;
};

