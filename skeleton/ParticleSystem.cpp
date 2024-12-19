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
    //particlesToErase.clear();
    //forcesToErase.clear();
    //gToErase.clear();

    rigidBodiesToErase.clear();

    // Actualizar todos los generadores
    for (auto g : generators) {
        if (g != nullptr) {
            g->update(t, *this);
        }
    }

    

    // Eliminar partículas y generadores inactivos
    for (auto p : particlesToErase) {
        auto it = std::find(particles.begin(), particles.end(), p);
        if (it != particles.end()) {
            particles.erase(it);
            delete p;
        }
    }

    for (auto f : forcesToErase) {
        auto it = std::find(forces.begin(), forces.end(), f);
        if (it != forces.end()) {
            forces.erase(it);
            delete f;
        }
    }

   /* for (auto g : gToErase) {
        auto it = std::find(gList.begin(), gList.end(), g);
        if (it != gList.end()) {
            gList.erase(it);
            delete g;
        }
    }*/

    for (auto r : rigidBodiesToErase) {
        auto it = std::find(rigidBodies.begin(), rigidBodies.end(), r);
        if (it != rigidBodies.end()) {
            rigidBodies.erase(it);
            delete r;
        }
    }

    // Actualizar todas las partículas
    for (auto it = particles.begin(); it != particles.end(); ) {
        if (*it != nullptr) {
            // Reiniciar aceleración a cero
            (*it)->SetAcceleration(PxVec3(0, 0, 0));

            // Aplicar fuerzas (como gravedad) a la aceleracion
            for (auto f : forces) {
                if (f && f->isAlive()) {
                    const PxVec3 force = f->applyForce(*it);
                    const PxVec3 accel = force / (*it)->getMass();  // F = ma
                    (*it)->SetAcceleration((*it)->getAcceleration() + accel);
                }
            }

            // Integrar la particula con la nueva aceleracion
            (*it)->Update(t, *this);
            it++;
        }
    }

    for (RigidBody* r : rigidBodies) {
        if (r != nullptr) {
            r->getActor()->clearForce(PxForceMode::eFORCE);

            for (auto f : forces) {
                if (f && f->isAlive()) {
                    PxVec3 force = f->applyForce(r);
                    r->getActor()->addForce(force);
                }
            }
        }
    }

    //Para la explosión    y en applyforce particle poner +
    //for (auto f : forces) {
    //    if (f != nullptr && f->isAlive()) {
    //        // Update de cada generador
    //        f->update(t);
    //        //Fuerza a cada particula
    //        for (auto it = particles.begin(); it != particles.end(); ++it) {
    //            if (*it != nullptr) {
    //                PxVec3 force = f->applyForce(*it);
    //                (*it)->ApplyForce(force);
    //            }
    //        }
    //    }
    //}
    //for (auto it = particles.begin(); it != particles.end(); ) {
    //    if (*it != nullptr) {
    //        (*it)->Update(t, *this);
    //        it++;
    //    }
    //}
}


void ParticleSystem::EliminateParticle(Particle* p) {
	if (p != nullptr && p->getIterator() != particles.end())
		particlesToErase.push_back(p);
}

void ParticleSystem::EliminateRigidBody(RigidBody* rigidBody)
{
    if (rigidBody != nullptr && rigidBody->getIterator() != rigidBodies.end())
        rigidBodiesToErase.push_back(rigidBody);
}

void ParticleSystem::AddParticle(Particle* p) {
	particles.push_back(p);
	p->setIterator(--particles.end());
}

void ParticleSystem::AddRigidBody(RigidBody* s)
{
    rigidBodies.push_back(s);
    s->setIterator(--rigidBodies.end());
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
    cout << "SPRING NORMAL" << endl;
    // First one standard spring uniting 2 particles
    Particle* p1 = new Particle({ -10, 10, 0 }, { 0,0,0 }, { 0,0,0 }, 1, 60);
    Particle* p2 = new Particle({ 10, 10, 0 }, { 0,0,0 }, { 0,0,0 }, 5, 60);
    SpringForceGenerator* f1 = new SpringForceGenerator(1, 10, p2);
    SpringForceGenerator* f2 = new SpringForceGenerator(1, 10, p1);
    forces.push_back(f2);
    forces.push_back(f1);
    AddParticle(p1);
    AddParticle(p2);
}

void ParticleSystem::Anchor()
{
    cout << "ANCHOR" << endl;
    // First one standard spring uniting 2 particles
    AnchoredSpringFG* anchor = new AnchoredSpringFG(0.5, 10, {-10, 10, 0});
    Particle* particle = new Particle({ 10, 10, 0 }, { 0,0,0 }, { 0,0,0 }, 0.5, 60);

    //Particle* particle2 = new Particle({ 10, 10, 0 }, { 0,0,0 }, { 0,0,0 }, 1, 60);

    
    forces.push_back(anchor);
    //SpringForceGenerator* f2 = new SpringForceGenerator(0.1, 10, anchor);
    //forces.push_back(f2);
    //particles.push_back(anchor);
    particles.push_back(particle);
    //particles.push_back(particle2);


}

void ParticleSystem::NewAnchor(PxPhysics* gPhysics, PxScene* gScene,
    const PxVec3& anchorPosition, const PxVec3& rigidBodyPosition,
    const PxVec3& dimensions, float mass,
    float springConstant, float restLength, Vector4 color)
{
    // Crear el resorte anclado
    AnchoredSpringFG* anchor = new AnchoredSpringFG(springConstant, restLength, anchorPosition);

    // Crear el RigidBody móvil
    RigidBody* rigidBody = new RigidBody(gPhysics, gScene, rigidBodyPosition, PxVec3(0, 0, 0), PxVec3(0, 0, 0),
        dimensions, mass, color, 0.0f);
    rigidBody->getActor()->setLinearDamping(0.0f);
    rigidBody->getActor()->setAngularDamping(0.0f);

    // Añadir el resorte al vector de fuerzas
    forces.push_back(anchor);

    // Añadir el RigidBody al vector de cuerpos rígidos
    rigidBodies.push_back(rigidBody);
}



void ParticleSystem::Buoyancy(float h, float V, float d)
{
    Particle* particle = new Particle({ 0, 30, 0 }, { 0,0,0 }, { 0,0,0 }, 1500, 60);
    Particle* water = new Particle({ 0, 50, 0 }, { 0,0,0 }, { 0,0,0 }, 0, 60);

    particles.push_back(particle);

    BuoyancyForceGenerator* buoyancy = new BuoyancyForceGenerator(h, V, d);
    buoyancy->setLiquidParticle(water);
    forces.push_back(buoyancy);

    GravityGenerator* g = new GravityGenerator(10);
    forces.push_back(g);
}






