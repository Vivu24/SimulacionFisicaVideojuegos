#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Particle.h"
#include "ParticleSystem.h"
#include <iostream>

std::string display_text = "This is a test";

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

RenderItem *sphere1RI = NULL;
RenderItem *sphere2RI = NULL;

RenderItem *myFloor = NULL;
ParticleSystem* particleSystem = NULL;

//Particle *particle = nullptr;

vector<Particle*> particles;

PxTransform pruebaTR;

Particle * anchor = nullptr;              // Partícula fija (anclaje del muelle)
Particle* dynamicParticle = nullptr;
SpringForceGenerator* spring = nullptr;

RenderItem* anchorRenderItem = NULL;
RenderItem* dynamicRenderItem = NULL;

// Initialize physics engine
void initPhysics(bool interactive)
{    
    PX_UNUSED(interactive);

    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    // For Solid Rigids +++++++++++++++++++++++++++++++++++++
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader;
    sceneDesc.simulationEventCallback = &gContactReportCallback;
    gScene = gPhysics->createScene(sceneDesc);

    //sphere1RI = new RenderItem(CreateShape(PxSphereGeometry(10)), new PxTransform(0, 35, 0), Vector4(1, 0.5, 1, 1));
    //sphere2RI = new RenderItem(CreateShape(PxSphereGeometry(5)), new PxTransform(20, 20, 0), Vector4(0.5, 0.5, 0.25, 1));
    
    //myFloor = new RenderItem(CreateShape(PxBoxGeometry( 100, 1, 100 )), new PxTransform(0, 0, 0), Vector4(0.5, 0.5, 0.25, 1));


    // Sistema de Partículas
    particleSystem = new ParticleSystem();
    particleSystem->CreateUniformGenerator(PxVec3(0, 0, 0), PxVec3(0, 20, 0), 20, 10.0f, 5.0f, spawnDistribution::UNIFORM, 50.0f, 10);
    
    // Práctica 3
    
    //particleSystem->Gravity(-3);

    //particleSystem->Wind(PxVec3(0, 0, 0), PxVec3(10, 10, 10), PxVec3(20, 0, 0), 0.1f);

    //particleSystem->Whirlwind(PxVec3(0.0f, 0.0f, 0.0f), PxVec3(15.0f, 100.0f, 15.0f), 0.5f, 5.0f);

    particleSystem->Explosion(100.0f, 20.0f, 1.0f);  // Intensidad 100, radio 20, tau 1

    //particleSystem->Spring();

    #pragma region Muelle


    // MUELLELELELELE
    anchor = new Particle(
        { 0.0f, 10.0f, 0.0f }, // Pos
        { 0.0f, 0.0f, 0.0f },  // Vel
        { 0.0f, 0.0f, 0.0f },  // Acel
        0.0,                   // Masa
        0.0                    // Vida
    );
    anchor->setRadius(0.5f);

    dynamicParticle = new Particle(
        { 0.0f, 5.0f, 0.0f },   // Init Pos
        { 0.0f, 0.0f, 0.0f },   // Init Vel
        { 0.0f, -9.8f, 0.0f },  // Gravedad Inicial
        1.0,                    // Masa
        10.0                    // vida
    );
    dynamicParticle->setRadius(1.0f);
    
    anchorRenderItem = new RenderItem(
        CreateShape(PxSphereGeometry(anchor->getRadius())),
        new PxTransform(anchor->getTransform()),
        { 1.0f, 0.0f, 0.0f, 1.0f }   // Rojo
    );

    dynamicRenderItem = new RenderItem(
        CreateShape(PxSphereGeometry(dynamicParticle->getRadius())),
        new PxTransform(dynamicParticle->getTransform()),
        { 0.0f, 1.0f, 0.0f, 1.0f }  // Verde
    );

    anchor->setRenderItem(anchorRenderItem);
    dynamicParticle->setRenderItem(dynamicRenderItem);



    spring = new SpringForceGenerator(
        10.0f,  //Const Elást
        5.0f,   // Longitud en reposo
        anchor  // Partícula fija
    );

    particleSystem->AddParticle(anchor);
    particleSystem->AddParticle(dynamicParticle);
    particleSystem->forces.push_back(spring);
    #pragma endregion
}

/*void checkParticlesHigh() {
    for each (Particle * particle in particles)
    {
        if (particle->getPosition().y - 1.5 <= myFloor->transform->p.y) {
            particle->SetAcceleration(PxVec3(0, 0, 0));
            particle->SetVelocity(PxVec3(0, 0, 0));
        }
    }
}*/

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
    particleSystem->Update(t);

    //Actualizar RenderItems    (NO FUNCA)
    //for (Particle* particle : particleSystem->particles) {
    //    if (particle->getRenderItem() != nullptr) {
    //        particle->getRenderItem()->transform->p = particle->getTransform().p;
    //    }
    //}


    PX_UNUSED(interactive);
    gScene->simulate(t);
    gScene->fetchResults(true);
    /*for each (Particle* particle in particles)
    {
        particle->Integrate(t);
        particle->Update(t);
    }*/
    //checkParticlesHigh();
    PX_UNUSED(interactive);

    gScene->simulate(t);
    gScene->fetchResults(true);
    
    particleSystem->Update(t);
}


// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
    PX_UNUSED(interactive);


    // MIO
    for (Particle* p : particles) {
        delete p;
    }
    particles.clear();  // Limpiar vector

    for (ForceGenerator* f : particleSystem->forces) {
        delete f;
    }
    particleSystem->forces.clear();

    delete particleSystem;
    particleSystem = nullptr;

    // Rigid Body ++++++++++++++++++++++++++++++++++++++++++
    gScene->release();
    gDispatcher->release();
    // -----------------------------------------------------
    gPhysics->release();
    PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();
    transport->release();

    gFoundation->release();

    

}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
    PX_UNUSED(camera);

    switch (toupper(key))
    {
    case ' ': {
        // Crear una nueva partícula con el ESPACIO
        Particle* particle = new Particle(GetCamera()->getTransform().p, 10 * GetCamera()->getDir(), PxVec3(0, 0, 0), 1.0, 5.0);
        particle->SetAcceleration(PxVec3(0, -2.5, 0)); // Aceleración debido a la gravedad
        particles.push_back(particle);
        break;
    }
    case 'E': {
        cout << "PULSADO E" << endl;
        PxVec3 position(0, 10, 0);
        particleSystem->TriggerExplosion(position);
        break;
    }

    default:
        break;
    }
}


void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
    PX_UNUSED(actor1);
    PX_UNUSED(actor2);
}

int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
    extern void renderLoop();
    renderLoop()
#else
    static const PxU32 frameCount = 100;
    initPhysics(false);
    for (PxU32 i = 0; i < frameCount; i++)
        stepPhysics(false);
    cleanupPhysics(false);
#endif
    ; return 0;
}
