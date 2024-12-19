#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include <vector>
#include <PxPhysicsAPI.h>

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

    void setDefaultMaterial(PxMaterial* material) { defaultMaterial = material; }

    bool lookBall;

private:
    ParticleSystem* myParticleSystem = nullptr;
    PxMaterial* defaultMaterial = nullptr;
    PxRigidDynamic* golfBall = nullptr;
    std::vector<RenderItem*> renderItems;

    PxVec3 cameraOffset = { 10.0f, 5.0f, 0.0f };

    // Cooldown para la tecla de disparo
    double accumulatedTime = 0.0;
    const double cooldown = 2.0;

    // Temporizador de confeti
    double confettiTimer = 0.0; // Temporizador para las partículas de confeti
    bool confettiActive = false; // Si las partículas están activas

    float bordilloAlto = 1.3f;
    float bordilloAncho = 1.0f;
    PxVec4 bordilloColor = { 0.8, 0.8, 0.8, 1.0 };

    bool transitionActive = false;   // Indica si estamos en transición
    double transitionTimer = 0.0;    // Tiempo acumulado durante la transición
    PxVec3 nextSpawnPosition;        // Posición a la que se moverá la bola


    vector<PxVec3> holesPositions{
        { -40.0f, 0.05f, 0.0f },    // HOYO 1
        { -150.0f, 0.05f, 50.0f }    // HOYO 2
    };

    vector<PxVec3> spawnPositions{
        { 0.0f, 0.05f, 0.0f },       // HOYO 1
        { -60.0f, 0.05f, 0.0f }      // HOYO 2
    };

    vector<bool> holesCompleteds{
        false,                      // HOYO 1
        false                       // HOYO 2
    };
};
