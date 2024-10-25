#pragma once
#include "Particle.h"
#include <random>

using namespace std;

class ParticleSystem;	// Evitar Errores Circulares

// Tipo de Distribución
enum SpawnDistribution{
	UNIFORM,
	NORMAL
};

class ParticleGenerator
{
public:
	ParticleGenerator(Particle* p, float rate, float sr, SpawnDistribution sd);

	virtual Particle* generate() = 0;
	
	PxVec3 calculatePosition();

	void update(double t, ParticleSystem& pS);

	float getEmissionRate() const { return creationVelocity; }

protected:	// Para la distribución Normal y Uniforme
	mt19937 myRandom;
	float creationVelocity;
	float lastTimed = 0;
	float spawnDistance;
	SpawnDistribution mySpawnDistribution;
	Particle myParticle;
};

