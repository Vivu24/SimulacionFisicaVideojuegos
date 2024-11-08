#include "ParticleSystem.h"


ParticleSystem::~ParticleSystem()
{
	for (auto particle : particles) {
		delete particle;
		particle = nullptr;
	}
	particles.clear();

	for (auto generator : generators) {
		delete generator;
		generator = nullptr;
	}
	generators.clear();

	for (auto particleE : particlesToErase) {
		delete particleE;
		particleE = nullptr;
	}
	particlesToErase.clear();

	for (auto forceE : forcesToErase) {
		delete forceE;
		forceE = nullptr;
	}
	forcesToErase.clear();
}

void ParticleSystem::Update(double t) {

	particlesToErase.clear();

	for (auto generator : generators) {
		if (generator != nullptr)
			generator->update(t, *this);
	}

	for (auto it = particles.begin(); it != particles.end(); ) {
		if (*it != nullptr) {
			(*it)->Update(t, *this);
			it++;
		}
	}

	for (auto p : particlesToErase) {
		cout << "Borra" << endl;
		auto it = find(particles.begin(), particles.end(), p);
		if (it != particles.end()) {
			particles.erase(it);
			delete p;
		}
	}


	//Fuerzas
	for (auto f : forces) {
		if (f) {
			f->update(t);
		}
	}
	//Eliminacion particulas
	//for (auto p : forcesToErase) {
	//	auto it = find(particles.begin(), particles.end(), p);
	//	if (it != particles.end()) {
	//		particles.erase(it);
	//		delete p;
	//	}
	//}
	////Eliminacion fuerzas
	for (auto f : forcesToErase) {
		auto it = find(forces.begin(), forces.end(), f);
		if (it != forces.end()) {
			forces.erase(it);
			delete f;
		}
	}
	
}

void ParticleSystem::EliminateParticle(Particle* p) {
	if (p != nullptr && p->getIterator() != particles.end())
		particlesToErase.push_back(p);
}

void ParticleSystem::AddParticle(Particle* p) {
	particles.push_back(p);
 	p->setIterator(--particles.end());
}

void ParticleSystem::CreateUniformGenerator(PxVec3 pos, PxVec3 direction, float rate, float range, float sr, SpawnDistribution sd, float rat, float lifetime)
{
	Particle p = Particle(pos, direction, PxVec3(0, -5, 0), lifetime);
	p.setRadius(rat);

	generators.push_back(new UniformGenerator(&p, rate, range, sr, sd));
}

void ParticleSystem::CreateNormalGenerator(PxVec3 pos, PxVec3 direction, float rate, PxVec3 des, float sr, SpawnDistribution sd, float rat, float lifetime)
{
	Particle p = Particle(pos, direction, PxVec3(0, -5, 0), lifetime);
	p.setRadius(rat);
	generators.push_back(new NormalGenerator(&p, rate, des, sr, sd));
}

void ParticleSystem::Gravity(double g)
{
	forces.push_back(new GravityGenerator(g, -1.0));
}
