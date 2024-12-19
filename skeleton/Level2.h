#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include <vector>
#include <PxPhysicsAPI.h>

class Level2 : public Scene {
public:
    Level2(PxPhysics* physics, PxScene* scene);
    ~Level2();

    void initScene() override;
    void update(double t) override;
    void keyPressed(unsigned char key, const PxTransform& camera) override;

    void initCam();
    void createHole2();

    void setDefaultMaterial(PxMaterial* material) { defaultMaterial = material; }

    bool lookBall;
    bool hoyoAlcanzado;

private:
    ParticleSystem* myParticleSystem = nullptr;
    PxMaterial* defaultMaterial = nullptr;
    PxRigidDynamic* golfBall = nullptr; // Bola de golf única
    std::vector<RenderItem*> renderItems;

    PxVec3 cameraOffset = { 10.0f, 5.0f, 0.0f }; // Offset de la cámara respecto a la bola

    // Cooldown para la tecla de disparo
    double accumulatedTime = 0.0;   // Tiempo acumulado para el cooldown
    const double cooldown = 2;    // Cooldown en segundos

    // Posiciones de los hoyos y de la bola
    PxVec3 hole2Position = { -40.0f, 0.05f, 0.0f };  // Posición del primer hoyo
    PxVec3 spawnPosition = { 0.0f, 0.5f, 0.0f };     // Posición de "spawn" inicial de la bola
};
