#pragma once
#include "Particle.h"
#include <random>

using namespace std;

class ParticleSystem;	// Evitar Errores Circulares

enum SpawnDistribution{
	UNIFORM,
	NORMAL
};

class ParticleGenerator
{
public:
	ParticleGenerator(Particle* p, float rate, float spawnR, SpawnDistribution sp);

	/**
	*	Metodo virtual para generacion de particulas
	*/
	virtual Particle* generate() = 0;
	/**
	*	Calcula la`posicion de la particula a emitir dependiendo de la distribucion
	*/
	Vector3 calculatePosition();

	void update(double t, ParticleSystem& pS);

	float getEmissionRate() const { return emissionRate; }

protected:	// Para la distribución Normal y Uniforme
	mt19937 random_engine;
	float emissionRate; // particulas por segundo
	float accumulatedTime = 0.0f;
	// Rango en el que se pueden spawnear las particulas
	float spawn_range;
	// Distribucion de la posicion para spawneo de particulas del generador
	SpawnDistribution spawn_distribution;
	/**
	*	Define la posicion del generador, la direccion de generacion (vel)
	*/
	Particle model_particle;
};

