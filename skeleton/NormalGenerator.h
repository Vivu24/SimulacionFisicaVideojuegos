#pragma once
#include "ParticleGenerator.h"
#include "ParticleSystem.h"
#include <random>

using namespace std;

class NormalGenerator : public ParticleGenerator
{
private:
	PxVec3 mediumVel;
	
	PxVec3 desviationVel;

public:
	NormalGenerator(Particle* p, float rate, PxVec3 devV, float spawnR, SpawnDistribution sp);

	Particle* generate() override;

};

