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
			++it;
		}
	}

	for (auto p : particlesToErase) {
		cout << "ENTRA A BORRAR\n";
		auto it = std::find(particles.begin(), particles.end(), p);
		if (it != particles.end()) {
			particles.erase(it);
			delete p;
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

void ParticleSystem::CreateUniformGenerator(Vector3 pos, Vector3 direction, float rate, float range, float spawnR, SpawnDistribution sp, float rat, float lifetime)
{
	Particle p = Particle(pos, direction, Vector3(0, -10, 0), lifetime);
	p.setRadius(rat);

	generators.push_back(new UniformGenerator(&p, rate, range, spawnR, sp));
}

void ParticleSystem::CreateNormalGenerator(Vector3 pos, Vector3 direction, float rate, Vector3 dev, float spawnR, SpawnDistribution sp, float rat, float lifetime)
{
	Particle p = Particle(pos, direction, Vector3(0, -10, 0), lifetime);
	p.setRadius(rat);
	generators.push_back(new NormalGenerator(&p, rate, dev, spawnR, sp));
}
