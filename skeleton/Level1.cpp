#include "Level1.h"

Level1::Level1(PxPhysics* physics, PxScene* scene)
    : Scene(physics, scene)
{
    myParticleSystem = new ParticleSystem();
    myParticleSystem->Gravity(9.8);
    lookBall = true;
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

    createHole2();

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

    if (transitionActive) {
        // Si estamos en pausa, acumula el tiempo
        transitionTimer += t;

        if (transitionTimer >= 3.0) { // Pausa de 2 segundos
            // Finaliza la pausa y mueve la bola al siguiente spawn
            golfBall->setGlobalPose(PxTransform(nextSpawnPosition));
            transitionActive = false; // Salimos de la transición
            transitionTimer = 0.0;    // Reiniciamos el temporizador
        }
        return; // Detenemos el resto de las actualizaciones durante la pausa
    }

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

    for (int i = 0; i < holesPositions.size(); i++) {
        // Comprobar si la bola ha llegado al hoyo
        float distanciaAlHoyo = (ballPosition - holesPositions[i]).magnitude();
        if (distanciaAlHoyo < 1.1f && !holesCompleteds[i]) {
            cout << "¡La bola ha llegado al hoyo!" << endl;
            holesCompleteds[i] = true;

            // Activa el confeti
            myParticleSystem->CreateUniformGenerator(holesPositions[i], PxVec3(0, 20, 0), 20, 10.0f, 5.0f, spawnDistribution::UNIFORM, 50.0f, 10);

            // Inicia la transición antes de mover la bola
            transitionActive = true;
            nextSpawnPosition = spawnPositions[i + 1]; // Posición del siguiente spawn
            return; // Detenemos el resto de la lógica en este frame
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
    PxRigidStatic* hoyo = gPhysics->createRigidStatic(PxTransform(holesPositions[0]));
    PxShape* hoyoShape = CreateShape(PxSphereGeometry(0.5));
    hoyo->attachShape(*hoyoShape);
    gScene->addActor(*hoyo);
    renderItems.push_back(new RenderItem(hoyoShape, hoyo, { 0.0, 0.0, 0.0, 1.0 }));
}

void Level1::createHole2()
{
    // CESPED
    PxRigidStatic* cesped = gPhysics->createRigidStatic(PxTransform({ -100, -1, 0 }));
    PxShape* cespedShape = CreateShape(PxBoxGeometry(500, 0.1, 500));
    cesped->attachShape(*cespedShape);
    gScene->addActor(*cesped);
    RenderItem* cespedRender = new RenderItem(cespedShape, cesped, { 0.0, 0.8, 0.0, 1.0 });
    renderItems.push_back(cespedRender);

    // PISTA PRINCIPAL (DOBLE DE LARGO)
    PxRigidStatic* pista = gPhysics->createRigidStatic(PxTransform({ -100, 0, 0 }));
    PxShape* pistaShape = CreateShape(PxBoxGeometry(100, 0.1, 10)); // Largo extendido
    pista->attachShape(*pistaShape);
    gScene->addActor(*pista);
    RenderItem* pistaRender = new RenderItem(pistaShape, pista, { 0.05, 0.5, 0.05, 1.0 });
    renderItems.push_back(pistaRender);

    // SEGMENTO EXTRA (AL FONDO)
    PxRigidStatic* pistaExtra = gPhysics->createRigidStatic(PxTransform({ -100, 0, -60 })); // Posicionado al fondo
    PxShape* pistaExtraShape = CreateShape(PxBoxGeometry(50, 0.1, 10)); // Segmento extra alineado
    pistaExtra->attachShape(*pistaExtraShape);
    gScene->addActor(*pistaExtra);
    RenderItem* pistaExtraRender = new RenderItem(pistaExtraShape, pistaExtra, { 0.05, 0.5, 0.05, 1.0 });
    renderItems.push_back(pistaExtraRender);

    // BORDILLOS PARA EL TRAMO PRINCIPAL
    PxRigidStatic* bordilloSup = gPhysics->createRigidStatic(PxTransform({ -100, bordilloAlto / 2, 11 }));
    PxShape* bordilloSupShape = CreateShape(PxBoxGeometry(100, bordilloAlto / 2, bordilloAncho));
    bordilloSup->attachShape(*bordilloSupShape);
    gScene->addActor(*bordilloSup);
    renderItems.push_back(new RenderItem(bordilloSupShape, bordilloSup, bordilloColor));

    PxRigidStatic* bordilloInf = gPhysics->createRigidStatic(PxTransform({ -100, bordilloAlto / 2, -11 }));
    PxShape* bordilloInfShape = CreateShape(PxBoxGeometry(100, bordilloAlto / 2, bordilloAncho));
    bordilloInf->attachShape(*bordilloInfShape);
    gScene->addActor(*bordilloInf);
    renderItems.push_back(new RenderItem(bordilloInfShape, bordilloInf, bordilloColor));

    // BORDILLOS PARA EL SEGMENTO EXTRA
    PxRigidStatic* bordilloExtraSup = gPhysics->createRigidStatic(PxTransform({ -100, bordilloAlto / 2, -49 }));
    PxShape* bordilloExtraSupShape = CreateShape(PxBoxGeometry(50, bordilloAlto / 2, bordilloAncho));
    bordilloExtraSup->attachShape(*bordilloExtraSupShape);
    gScene->addActor(*bordilloExtraSup);
    renderItems.push_back(new RenderItem(bordilloExtraSupShape, bordilloExtraSup, bordilloColor));

    PxRigidStatic* bordilloExtraInf = gPhysics->createRigidStatic(PxTransform({ -100, bordilloAlto / 2, -71 }));
    PxShape* bordilloExtraInfShape = CreateShape(PxBoxGeometry(50, bordilloAlto / 2, bordilloAncho));
    bordilloExtraInf->attachShape(*bordilloExtraInfShape);
    gScene->addActor(*bordilloExtraInf);
    renderItems.push_back(new RenderItem(bordilloExtraInfShape, bordilloExtraInf, bordilloColor));

    PxRigidStatic* bordilloExtraDer = gPhysics->createRigidStatic(PxTransform({ -49, bordilloAlto / 2, -60 }));
    PxShape* bordilloExtraDerShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, 10));
    bordilloExtraDer->attachShape(*bordilloExtraDerShape);
    gScene->addActor(*bordilloExtraDer);
    renderItems.push_back(new RenderItem(bordilloExtraDerShape, bordilloExtraDer, bordilloColor));

    PxRigidStatic* bordilloExtraIzq = gPhysics->createRigidStatic(PxTransform({ -151, bordilloAlto / 2, -60 }));
    PxShape* bordilloExtraIzqShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, 10));
    bordilloExtraIzq->attachShape(*bordilloExtraIzqShape);
    gScene->addActor(*bordilloExtraIzq);
    renderItems.push_back(new RenderItem(bordilloExtraIzqShape, bordilloExtraIzq, bordilloColor));

    // HOYO 2 (NEGRO)
    PxRigidStatic* hoyo2 = gPhysics->createRigidStatic(PxTransform(holesPositions[1]));
    PxShape* hoyo2Shape = CreateShape(PxSphereGeometry(0.5));
    hoyo2->attachShape(*hoyo2Shape);
    gScene->addActor(*hoyo2);
    renderItems.push_back(new RenderItem(hoyo2Shape, hoyo2, { 0.0, 0.0, 0.0, 1.0 })); // Negro
}




