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

    // Posiciones de los hoyos y de la bola
    PxVec3 hole1Position = { -40.0f, 0.05f, 0.0f };
    PxVec3 spawnPosition = { 0.0f, 0.5f, 0.0f };
    bool hoyoAlcanzado = false; // Flag para cuando la bola alcanza el hoyo

};
