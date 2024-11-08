#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

//#include "Particle.h"
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

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

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

	particleSystem = new ParticleSystem();
	particleSystem->CreateUniformGenerator(PxVec3(0, 0, 0), PxVec3(0, 20, 0), 20, 10.0f, 5.0f, SpawnDistribution::UNIFORM, 50.0f, 10);
	//particleSystem->CreateUniformGenerator(PxVec3(0, 0, 0), PxVec3(0, 20, 0), 10, 1.0f, 10.0f, SpawnDistribution::UNIFORM, 50.0f, 10);
	//particleSystem->CreateUniformGenerator(PxVec3(-20, 35, -20), PxVec3(10, 0, 0), 40, 20.0f, 25.0f, SpawnDistribution::UNIFORM, 50.0f, 10);

	//particleSystem->CreateNormalGenerator(PxVec3(0, 0, 0), PxVec3(30, 20, -50), 0.1f, PxVec3(3, 3, 3), 2.0f, SpawnDistribution::UNIFORM, 1000.0f, 2);

	//particleSystem->Gravity(1000);

}

void checkParticlesHigh() {
	/*for each (Particle * particle in particles)
	{
		if (particle->getPosition().y - 1.5 <= myFloor->transform->p.y) {
			particle->SetAcceleration(PxVec3(0, 0, 0));
			particle->SetVelocity(PxVec3(0, 0, 0));
		}
	}*/
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	particleSystem->Update(t);
	PX_UNUSED(interactive);
	gScene->simulate(t);
	gScene->fetchResults(true);
	/*for each (Particle* particle in particles)
	{
		particle->Integrate(t);
		particle->Update(t);
	}*/
	checkParticlesHigh();
}


// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

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

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
	case 'G':
	{	
		cout << "Particle" << endl;

		Particle* particle = new Particle(GetCamera()->getTransform().p, 10 * GetCamera()->getDir(), PxVec3(0, 0, 0), 1, 5);
		particle->SetAcceleration(PxVec3(0, -2.5, 0));
		particles.push_back(particle);
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


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}