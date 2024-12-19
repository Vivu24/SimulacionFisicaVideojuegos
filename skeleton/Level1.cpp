#include "Level1.h"

Level1::Level1(PxPhysics* physics, PxScene* scene)
    : Scene(physics, scene)
{
    myParticleSystem = new ParticleSystem();
    myParticleSystem->Gravity(9.8);
    lookBall = true;
    hoyoAlcanzado = false;
}

Level1::~Level1()
{
    for (auto rb : renderItems) {
        delete rb;
    }
}

void Level1::initScene()
{
    if (!defaultMaterial) {
        throw std::runtime_error("Mat not Init");
    }

    initCam();
    createHole1();

    // Crear la bola de golf al iniciar la escena
    PxVec3 ballPosition = { 0, 0.5f, 0 }; // Posición inicial de la bola
    golfBall = gPhysics->createRigidDynamic(PxTransform(ballPosition));
    PxShape* ballShape = CreateShape(PxSphereGeometry(0.5f)); // Radio de la bola
    PxMaterial* ballMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.2f); // Fricción estática, dinámica, restitución
    ballShape->setMaterials(&ballMaterial, 1);

    // Adjuntar la forma de la bola y configurar propiedades físicas
    golfBall->attachShape(*ballShape);
    PxRigidBodyExt::updateMassAndInertia(*golfBall, 1.0f); // Configurar densidad
    golfBall->setLinearDamping(0.5f);  // Amortiguación lineal
    golfBall->setAngularDamping(0.8f); // Amortiguación angular

    // Añadir la bola a la escena
    gScene->addActor(*golfBall);
    renderItems.push_back(new RenderItem(ballShape, golfBall, { 1.0f, 1.0f, 1.0f, 1.0f })); // Bola blanca
}

void Level1::update(double t)
{
    myParticleSystem->Update(t);

    accumulatedTime += t;
    PX_UNUSED(t);

    if (!golfBall || !GetCamera()) return;

    // Obtener la posición de la bola
    PxTransform ballTransform = golfBall->getGlobalPose();
    PxVec3 ballPosition = ballTransform.p;

    // Calcular la nueva posición de la cámara
    PxVec3 newCameraPos = ballPosition + cameraOffset;

    // Actualizar la cámara
    Snippets::Camera* camera = GetCamera();
    camera->mEye = newCameraPos;
    if (lookBall) {
        camera->mDir = (ballPosition - newCameraPos).getNormalized();
        lookBall = false;
    }

    // Comprobar si la bola ha llegado al hoyo
    float distanciaAlHoyo = (ballPosition - hole1Position).magnitude();
    if (distanciaAlHoyo < 1.1f && !hoyoAlcanzado) {
        cout << "¡La bola ha llegado al hoyo!" << endl;
        hoyoAlcanzado = true;
        confettiActive = true; // Activar las partículas de confeti
        confettiTimer = 5.0;   // El confeti dura 5 segundos
        myParticleSystem->CreateUniformGenerator(hole1Position, PxVec3(0, 20, 0), 20, 10.0f, 5.0f, spawnDistribution::UNIFORM, 50.0f, 10);

    }

    // Si el confeti está activo, decrementamos el temporizador
    if (confettiActive) {
        confettiTimer -= t;
        if (confettiTimer <= 0.0) {
            confettiActive = false; // Desactivar las partículas después de 5 segundos
        }
    }
}

void Level1::keyPressed(unsigned char key, const PxTransform& camera)
{
    PX_UNUSED(camera);

    switch (toupper(key))
    {
    case ' ': {
        // Comprobar cooldown (si el tiempo acumulado es mayor que el cooldown, aplicamos la acción)
        if (accumulatedTime < cooldown) {
            cout << "En cooldown, espera..." << endl;
            break;
        }

        // Reiniciar el tiempo acumulado después de la acción
        accumulatedTime = 0.0;

        cout << "ESPACIO" << endl;

        // Verificar si la bola de golf existe
        if (!golfBall) return;

        // Obtener la dirección y posición de la cámara
        Snippets::Camera* cam = GetCamera();
        if (!cam) return;

        PxVec3 camDir = cam->getDir(); // Dirección en la que apunta la cámara
        PxVec3 fuerza = camDir * 15.0f; // Ajustar la fuerza del golpe

        // Aplicar la fuerza a la bola
        golfBall->addForce(fuerza, PxForceMode::eIMPULSE);
        break;
    }
    case 'Z': {
        cout << "Z" << endl;
        if (!lookBall) {
            lookBall = true;
        }
        break;
    }
    default:
        break;
    }
}

void Level1::initCam()
{
    if (!GetCamera()) {
        throw std::runtime_error("Cam not Init");
    }

    // Posición inicial de la bola
    PxVec3 ballPosition = { 0, 0.5f, 0 };

    // Offset de la cámara en relación a la bola
    PxVec3 cameraOffset = { 0, 20.0f, 0.0f }; // La cámara está detrás y por encima de la bola

    // Calcula la posición inicial de la cámara
    PxVec3 initialCameraPos = ballPosition + cameraOffset;

    // Obtén la cámara
    Snippets::Camera* camera = GetCamera();

    // Ajusta la posición y dirección de la cámara
    camera->mEye = initialCameraPos;                // Posición inicial de la cámara
    camera->mDir = (ballPosition - initialCameraPos).getNormalized(); // Apunta hacia la bola
}

void Level1::createHole1()
{
    // CESPED
    PxRigidStatic* cesped = gPhysics->createRigidStatic(PxTransform({ 0, -1, 0 }));
    PxShape* cespedShape = CreateShape(PxBoxGeometry(500, 0.1, 500));
    cesped->attachShape(*cespedShape);
    gScene->addActor(*cesped);
    RenderItem* cespedRender = new RenderItem(cespedShape, cesped, { 0.0, 0.8, 0.0, 1.0 });
    renderItems.push_back(cespedRender);

    // PISTA PRINCIPAL
    PxRigidStatic* pista = gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }));
    PxShape* pistaShape = CreateShape(PxBoxGeometry(50, 0.1, 10));
    pista->attachShape(*pistaShape);
    gScene->addActor(*pista);
    RenderItem* pistaRender = new RenderItem(pistaShape, pista, { 0.05, 0.5, 0.05, 1.0 });
    renderItems.push_back(pistaRender);

    // BORDILLOS
    float bordilloAlto = 0.3f;
    float bordilloAncho = 1.0f;
    PxVec4 bordilloColor = { 0.8, 0.8, 0.8, 1.0 };

    PxRigidStatic* bordilloSup = gPhysics->createRigidStatic(PxTransform({ 0, bordilloAlto / 2, 11 }));
    PxShape* bordilloSupShape = CreateShape(PxBoxGeometry(50, bordilloAlto / 2, bordilloAncho));
    bordilloSup->attachShape(*bordilloSupShape);
    gScene->addActor(*bordilloSup);
    renderItems.push_back(new RenderItem(bordilloSupShape, bordilloSup, bordilloColor));

    PxRigidStatic* bordilloInf = gPhysics->createRigidStatic(PxTransform({ 0, bordilloAlto / 2, -11 }));
    PxShape* bordilloInfShape = CreateShape(PxBoxGeometry(50, bordilloAlto / 2, bordilloAncho));
    bordilloInf->attachShape(*bordilloInfShape);
    gScene->addActor(*bordilloInf);
    renderItems.push_back(new RenderItem(bordilloInfShape, bordilloInf, bordilloColor));

    PxRigidStatic* bordilloIzq = gPhysics->createRigidStatic(PxTransform({ -51, bordilloAlto / 2, 0 }));
    PxShape* bordilloIzqShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, 10));
    bordilloIzq->attachShape(*bordilloIzqShape);
    gScene->addActor(*bordilloIzq);
    renderItems.push_back(new RenderItem(bordilloIzqShape, bordilloIzq, bordilloColor));

    PxRigidStatic* bordilloDer = gPhysics->createRigidStatic(PxTransform({ 51, bordilloAlto / 2, 0 }));
    PxShape* bordilloDerShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, 10));
    bordilloDer->attachShape(*bordilloDerShape);
    gScene->addActor(*bordilloDer);
    renderItems.push_back(new RenderItem(bordilloDerShape, bordilloDer, bordilloColor));

    // HOYO
    PxRigidStatic* hoyo = gPhysics->createRigidStatic(PxTransform(hole1Position));
    PxShape* hoyoShape = CreateShape(PxSphereGeometry(0.5));
    hoyo->attachShape(*hoyoShape);
    gScene->addActor(*hoyo);
    renderItems.push_back(new RenderItem(hoyoShape, hoyo, { 0.0, 0.0, 0.0, 1.0 }));
}


