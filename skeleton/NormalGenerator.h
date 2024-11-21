#pragma once
#include "ParticleGenerator.h"
#include "ParticleSystem.h"
#include <random>

using namespace std;

class NormalGenerator : public ParticleGenerator
{
public:
	NormalGenerator(Particle* p, float rate, PxVec3 des, float sr, spawnDistribution sp);

	Particle* generate() override;

private:
	PxVec3 mediumVel;
	PxVec3 desviationVel;
};
