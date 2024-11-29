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

void ParticleSystem::Update(double t)
{
    particlesToErase.clear();  // Limpiar la lista de particulas a eliminar

    for (auto generator : generators) {
        if (generator != nullptr)
            generator->update(t, *this);  // Actualiza cada generador
    }

    // Aplicar fuerzas  NOBORRAR
    /*for (auto f : forces) {
        if (f != nullptr && f->isAlive()) {
            for (auto it = particles.begin(); it != particles.end(); ++it) {
                if (*it != nullptr) {
                    PxVec3 force = f->applyForce(*it);
                    (*it)->ApplyForce(force);
                }
            }
        }
    }*/

    for (auto it = particles.begin(); it != particles.end(); ) {
        if (*it != nullptr) {
            (*it)->Update(t, *this);
            it++;
        }
    }

    for (auto p : particlesToErase) {
        auto it = find(particles.begin(), particles.end(), p);
        if (it != particles.end()) {
            particles.erase(it);
            delete p;
        }
    }

    for (auto f : forcesToErase) {
        auto it = find(forces.begin(), forces.end(), f);
        if (it != forces.end()) {
            forces.erase(it);
            delete f;
        }
    }

    // Aplica las fuerzas bien
    for (auto f : forces) {
        if (f != nullptr && f->isAlive()) {
            // Update de cada generador
            f->update(t);

            //Fuerza a cada particula
            for (auto it = particles.begin(); it != particles.end(); ++it) {
                if (*it != nullptr) {
                    PxVec3 force = f->applyForce(*it);
                    (*it)->ApplyForce(force);
                }
            }
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

void ParticleSystem::CreateUniformGenerator(PxVec3 pos, PxVec3 direction, float rate, float range, float sr, spawnDistribution sd, float rat, float lifetime)
{
	// Partícula modelo
	Particle p = Particle(pos, direction, PxVec3(0, -5, 0), lifetime);
	p.setRadius(rat);

	generators.push_back(new UniformGenerator(&p, rate, range, sr, sd));
}

void ParticleSystem::CreateNormalGenerator(PxVec3 pos, PxVec3 direction, float rate, PxVec3 des, float sr, spawnDistribution sd, float rat, float lifetime)
{
	// Partícula modelo
	Particle p = Particle(pos, direction, PxVec3(0, -5, 0), lifetime);
	p.setRadius(rat);

	generators.push_back(new NormalGenerator(&p, rate, des, sr, sd));

}

void ParticleSystem::Gravity(double g)
{
    forces.push_back(new GravityGenerator(g));
}

void ParticleSystem::Wind(PxVec3 center, PxVec3 size, PxVec3 windVelocity, float rozCoef)
{
    forces.push_back(new WindGenerator(center, size, windVelocity, rozCoef));
}

void ParticleSystem::Whirlwind(PxVec3 center, PxVec3 size, float rozCoef, float intensity)
{
    forces.push_back(new WhirlwindGenerator(center, size, rozCoef, intensity));
}

void ParticleSystem::Explosion(float intensity, float radius, float tau)
{
    forces.push_back(new ExplosionGenerator(this, PxVec3(0, 0, 0), PxVec3(radius, radius, radius), intensity, 0.0, tau));
}

// Activar Explosión
void ParticleSystem::TriggerExplosion(PxVec3 position)
{
    for (auto& force : forces)
    {
        // Si la fuerza es explosión
        if (force->getType() == EXPLOSION)
        {
            // Para llamar a resetExplosion
            ExplosionGenerator* explosion = static_cast<ExplosionGenerator*>(force);
            if (explosion)
            {
                // Reinicia la explosión con la nueva posición
                explosion->resetExplosion(position);
            }
        }
    }
}

void ParticleSystem::Spring()
{
    // First one standard spring uniting 2 particles
    Particle* p1 = new Particle({ -10, 10, 0 }, { 0,0,0 }, { 0,0,0 }, 0.85, 60);
    Particle* p2 = new Particle({ 10, 10, 0 }, { 0,0,0 }, { 0,0,0 }, 0.85, 60);
    p2->setMass(2.0);
    SpringForceGenerator* f1 = new SpringForceGenerator(1, 10, p2);
    forces.push_back(f1);
    SpringForceGenerator* f2 = new SpringForceGenerator(1, 10, p1);
    forces.push_back(f2);
    particles.push_back(p1);
    particles.push_back(p2);
}






