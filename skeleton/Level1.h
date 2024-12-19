#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include <vector>
#include <PxPhysicsAPI.h>
#include "RigidBody.h"

class Level1 : public Scene {
public:
    Level1(PxPhysics* physics, PxScene* scene);
    ~Level1();

    void initScene() override;
    void update(double t) override;
    void keyPressed(unsigned char key, const PxTransform& camera) override;

    void initCam();
    void createHole1();
    void createHole2();
    void createHole3();
    void createHole4();
    void createHole5();
    void createObstacleGenerator(int obs);

    void createDecoration();
    void createFinishParticles();
    void createTrackSegment(const PxVec3& position, const PxQuat& rotation, const PxVec3& size);
    void createTrackSegment2(const PxVec3& position, const PxQuat& rotation, const PxVec3& size);

    void setDefaultMaterial(PxMaterial* material) { defaultMaterial = material; }

    void checkDeathHeight();
    void respawnAtLastCheckpoint();
    void spawnWater();

    bool lookBall;
    int totalCheckPoints = 0;
private:
    ParticleSystem* myParticleSystem = nullptr;
    ParticleSystem* windParticleSystem = nullptr;
    ParticleSystem* gravityParticleSystem = nullptr;
    std::vector<ParticleSystem*> anchoredSystemVector;

    PxMaterial* defaultMaterial = nullptr;
    PxRigidDynamic* golfBall = nullptr;
    std::vector<RenderItem*> renderItems;

    //PxVec3 cameraOffset = {0.05f, 70.05f, 0.0f };
    PxVec3 cameraOffset = {0.5f, 2.05f, 0.0f };
    //PxVec3 cameraOffset = {-30.05f, 1.05f, 200.0f };
    
    float DeadHeight = -30.0f;
    PxTransform lastCheckPointPos;
    RenderItem* myFloor = nullptr;  // Variable global para almacenar el plano

    // Cooldown para la tecla de disparo
    double accumulatedTime = 0.0;
    const double cooldown = 1.0;

    // Bolas Rojas
    float obstacleTimer = 0.0f; // Temporizador para crear bolas
    const float obstacleInterval = 2.0f; // Intervalo de 2 segundos para crear bolas
    const float hole3Radius = 5.0f; // Radio dentro del cual se crearán las bolas (ajustar según sea necesario)

    // Bordillos
    float bordilloAlto = 1.0f;
    float bordilloAncho = 1.0f;
    PxVec4 bordilloColor = { 0.8, 0.8, 0.8, 1.0 };

    // Transicion de niveles
    bool transitionActive = false;   // Indica si estamos en transición
    double transitionTimer = 0.0;    // Tiempo acumulado durante la transición
    PxVec3 nextSpawnPosition;        // Posición a la que se moverá la bola

    vector<PxVec3> holesPositions{
        { -70.0f, -1.0f, 0.0f },    // HOYO 1
        { -250.0f, -1.0f, 0.0f },   // HOYO 2
        { 40.0f, -1.0f, 70.0f },     // HOYO 3
        { -200.0f, -1.0f, 70.0f },     // HOYO 4
        { -160.0f, -1.0f, 170.0f }     // HOYO 5
    };

    vector<PxVec3> spawnPositions{
        { 0.0f, 0.05f, 0.0f },       // HOYO 1
        { -130.0f, 0.05f, -10.0f },      // HOYO 2
        { -35.0f, 0.05f, 70.0f },      // HOYO 3
        { -140.0f, 0.05f, 70.0f },      // HOYO 4
        { -50.0f, 0.05f, 170.0f },      // HOYO 5
        {  -160.0f, -1.0f, 170.0f }      // HOYO 6
    };

    vector<bool> holesCompleteds{
        false,                      // HOYO 1
        false,                       // HOYO 2
        false,                       // HOYO 3
        false,                       // HOYO 4
        false                       // HOYO 5
    };

    PxVec3 auxSpawn1BR = PxVec3(holesPositions[2].x - 30,
        holesPositions[2].y + 5, 
        holesPositions[2].z - 20);
    PxVec3 auxSpawn2BR = PxVec3(holesPositions[2].x - 40,
        holesPositions[2].y + 5,
        holesPositions[2].z + 20);
    PxVec3 auxSpawn3BR = PxVec3(holesPositions[2].x - 50,
        holesPositions[2].y + 5,
        holesPositions[2].z - 20);

    PxVec3 auxSpawn0BV = PxVec3(holesPositions[3].x + 40,
        holesPositions[2].y + 25,
        holesPositions[2].z + 10);
    PxVec3 auxSpawn1BV = PxVec3(holesPositions[3].x + 20,
        holesPositions[2].y + 25,
        holesPositions[2].z + 10);
    PxVec3 auxSpawn2BV = PxVec3(holesPositions[3].x + 30,
        holesPositions[2].y + 25,
        holesPositions[2].z + 20);
    PxVec3 auxSpawn3BV = PxVec3(holesPositions[3].x + 30,
        holesPositions[2].y + 25,
        holesPositions[2].z - 20);
};
