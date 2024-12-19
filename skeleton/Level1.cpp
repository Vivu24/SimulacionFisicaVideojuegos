#include "Level1.h"

Level1::Level1(PxPhysics* physics, PxScene* scene)
    : Scene(physics, scene)
{
    myParticleSystem = new ParticleSystem();
    windParticleSystem = new ParticleSystem();
    gravityParticleSystem = new ParticleSystem();
    myParticleSystem->Gravity(9.8);
    lookBall = true;
    spawnWater();
    createDecoration();
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
    createHole3();
    createHole4();
    createHole5();

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

    //createFinishParticles();

}

void Level1::update(double t) {
    /*if (holesCompleteds[4]) {
        createFinishParticles();
        holesCompleteds[4] = false;
    }*/
    if (totalCheckPoints >= 5) {
        createFinishParticles();
        totalCheckPoints = 0;
    }
    checkDeathHeight();

    myParticleSystem->Update(t);
    windParticleSystem->Update(t);
    gravityParticleSystem->Update(t);
    for each (ParticleSystem* as in anchoredSystemVector)
    {
        as->Update(t);
    }

    // Acumula el tiempo transcurrido
    obstacleTimer += t;
    accumulatedTime += t;

    // Verifica si el temporizador ha alcanzado el intervalo
    if (obstacleTimer >= obstacleInterval) {
        // Crear bola roja en una posición fija (en el hoyo 3)
        cout << "BOLAROJA" << endl;

        // Restablecer el temporizador
        obstacleTimer = 0.0f;
        // Crear la bola roja como obstáculo
        RigidBody* obstacle1 = new RigidBody(gPhysics, gScene, auxSpawn1BR, 5.0f, 5.5f, PxVec4(1.0f, 0.0f, 0.0f, 1.0f), 5); // Bola roja
        RigidBody* obstacle2 = new RigidBody(gPhysics, gScene, auxSpawn2BR, 5.0f, 5.5f, PxVec4(1.0f, 0.0f, 0.0f, 1.0f), 5); // Bola roja
        RigidBody* obstacle3 = new RigidBody(gPhysics, gScene, auxSpawn3BR, 5.0f, 5.5f, PxVec4(1.0f, 0.0f, 0.0f, 1.0f), 5); // Bola roja

        // Añadir el obstáculo al sistema de partículas
        windParticleSystem->AddRigidBody(obstacle1);
        windParticleSystem->AddRigidBody(obstacle2);
        windParticleSystem->AddRigidBody(obstacle3);

        // Crear bola roja en una posición fija (en el hoyo 3)
        cout << "BOLAVERDE" << endl;

        // Crear la bola roja como obstáculo
        RigidBody* obstacleV0 = new RigidBody(gPhysics, gScene, auxSpawn1BV, 5.0f, 5.5f, PxVec4(0.0f, 1.0f, 0.0f, 1.0f), 5); // Bola roja
        RigidBody* obstacleV1 = new RigidBody(gPhysics, gScene, auxSpawn1BV, 5.0f, 5.5f, PxVec4(0.0f, 1.0f, 0.0f, 1.0f), 5); // Bola roja
        RigidBody* obstacleV2 = new RigidBody(gPhysics, gScene, auxSpawn2BV, 5.0f, 5.5f, PxVec4(0.0f, 1.0f, 0.0f, 1.0f), 5); // Bola roja
        RigidBody* obstacleV3 = new RigidBody(gPhysics, gScene, auxSpawn3BV, 5.0f, 5.5f, PxVec4(0.0f, 1.0f, 0.0f, 1.0f), 5); // Bola roja

        // Añadir el obstáculo al sistema de partículas
        gravityParticleSystem->AddRigidBody(obstacleV0);
        gravityParticleSystem->AddRigidBody(obstacleV1);
        gravityParticleSystem->AddRigidBody(obstacleV2);
        gravityParticleSystem->AddRigidBody(obstacleV3);
    }

    // Resto de la lógica de actualización (transiciones, cámara, etc.)
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
            myParticleSystem->CreateUniformGenerator(holesPositions[i], PxVec3(0, 20, 0), 20, 10.0f, 5.0f, spawnDistribution::UNIFORM, 20.0f, 2);
            //myParticleSystem->CreateNormalGenerator(holesPositions[i], PxVec3(0, 20, 0), 20, PxVec3(0,0,0), 5.0f, spawnDistribution::NORMAL, 20.0f, 2);
            totalCheckPoints += 1;
            // Inicia la transición antes de mover la bola
            transitionActive = true;
            nextSpawnPosition = spawnPositions[i + 1]; // Posición del siguiente spawn
            lastCheckPointPos.p = nextSpawnPosition;
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
        PxVec3 fuerza = camDir * 10.0f; // Ajustar la fuerza del golpe

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

void Level1::createTrackSegment(const PxVec3& position, const PxQuat& rotation, const PxVec3& size) {
    // Crear el césped
    //PxRigidStatic* cesped = gPhysics->createRigidStatic(PxTransform(position, rotation));
    //PxShape* cespedShape = CreateShape(PxBoxGeometry(size.x, 0.1f, size.z));
    //cesped->attachShape(*cespedShape);
    //gScene->addActor(*cesped);
    //renderItems.push_back(new RenderItem(cespedShape, cesped, { 0.0f, 0.8f, 0.0f, 1.0f })); // Verde césped

    // Crear la pista
    PxRigidStatic* pista = gPhysics->createRigidStatic(PxTransform(position, rotation));
    PxShape* pistaShape = CreateShape(PxBoxGeometry(size.x, 0.1f, size.z - 2 * bordilloAncho)); // Menos los bordillos
    pista->attachShape(*pistaShape);
    gScene->addActor(*pista);
    renderItems.push_back(new RenderItem(pistaShape, pista, { 0.05f, 0.5f, 0.05f, 1.0f })); // Verde claro

    // Crear bordillos superior e inferior
    PxRigidStatic* bordilloSup = gPhysics->createRigidStatic(PxTransform(position + PxVec3(0, bordilloAlto / 2, size.z - bordilloAncho), rotation));
    PxShape* bordilloSupShape = CreateShape(PxBoxGeometry(size.x, bordilloAlto / 2, bordilloAncho));
    bordilloSup->attachShape(*bordilloSupShape);
    gScene->addActor(*bordilloSup);
    renderItems.push_back(new RenderItem(bordilloSupShape, bordilloSup, bordilloColor));

    PxRigidStatic* bordilloInf = gPhysics->createRigidStatic(PxTransform(position + PxVec3(0, bordilloAlto / 2, -(size.z - bordilloAncho)), rotation));
    PxShape* bordilloInfShape = CreateShape(PxBoxGeometry(size.x, bordilloAlto / 2, bordilloAncho));
    bordilloInf->attachShape(*bordilloInfShape);
    gScene->addActor(*bordilloInf);
    renderItems.push_back(new RenderItem(bordilloInfShape, bordilloInf, bordilloColor));

    // Crear bordillos izquierdo y derecho
    PxRigidStatic* bordilloIzq = gPhysics->createRigidStatic(PxTransform(position + PxVec3(-(size.x - bordilloAncho), bordilloAlto / 2, 0), rotation));
    PxShape* bordilloIzqShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, size.z));
    bordilloIzq->attachShape(*bordilloIzqShape);
    gScene->addActor(*bordilloIzq);
    renderItems.push_back(new RenderItem(bordilloIzqShape, bordilloIzq, bordilloColor));

    PxRigidStatic* bordilloDer = gPhysics->createRigidStatic(PxTransform(position + PxVec3(size.x - bordilloAncho, bordilloAlto / 2, 0), rotation));
    PxShape* bordilloDerShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, size.z));
    bordilloDer->attachShape(*bordilloDerShape);
    gScene->addActor(*bordilloDer);
    renderItems.push_back(new RenderItem(bordilloDerShape, bordilloDer, bordilloColor));
}

void Level1::createDecoration() {
    // Definir las posiciones para las 4 esquinas del fondo
    std::vector<PxVec3> cornerPositions = {
        { -400, -5.0f, -400 },  // Esquina inferior izquierda
        { 100, -5.0f, -400 },   // Esquina inferior derecha
        { -400, -5.0f, 400.0f },   // Esquina superior izquierda
        { 100.0f, -5.0f, 400.0f }     // Esquina superior derecha
    };

    // Crear montañas en las 4 esquinas
    for (int i = 0; i < 4; ++i) {
        PxVec3 mountainPosition = cornerPositions[i];
        PxVec3 mountainScale = PxVec3(30.0f, 50.0f, 30.0f); // Tamaño de la montaña (base más ancha, alta)

        // Crear una cápsula para representar la montaña
        PxRigidStatic* mountain = gPhysics->createRigidStatic(PxTransform(mountainPosition));
        PxShape* mountainShape = CreateShape(PxCapsuleGeometry(mountainScale.x, mountainScale.y)); // Usando el escalado en la geometría
        mountain->attachShape(*mountainShape);
        gScene->addActor(*mountain);

        // Renderizar la forma de la montaña
        RenderItem* mountainRender = new RenderItem(mountainShape, mountain, { 0.4f, 0.3f, 0.2f, 1.0f }); // Marrón claro
        renderItems.push_back(mountainRender);
    }

    // Parametrización de la cantidad de nubes
    int numClouds = 10;  // Número de nubes a generar
    float cloudHeight = 50.0f;  // Altura en Y donde se generarán las nubes

    // Crear nubes dentro de la zona definida por las esquinas
    for (int i = 0; i < numClouds; ++i) {
        // Generar una posición aleatoria dentro de la zona
        float randomX = (rand() % 500) - 400;  // Rango entre -400 y 100
        float randomZ = (rand() % 800) - 400;  // Rango entre -400 y 400

        PxVec3 cloudPosition = PxVec3(randomX, cloudHeight + (rand() % 10 - 5), randomZ); // Variación en la altura y Z
        PxVec3 cloudScale = PxVec3(10.0f, 5.0f, 5.0f); // Tamaño de la nube

        // Crear una esfera para representar la nube
        PxRigidStatic* cloud = gPhysics->createRigidStatic(PxTransform(cloudPosition));
        PxShape* cloudShape = CreateShape(PxSphereGeometry(cloudScale.x)); // Usando el escalado en la geometría (radio)
        cloud->attachShape(*cloudShape);
        gScene->addActor(*cloud);

        // Renderizar la forma de la nube
        RenderItem* cloudRender = new RenderItem(cloudShape, cloud, { 1.0f, 1.0f, 1.0f, 0.8f }); // Blanco semitransparente
        renderItems.push_back(cloudRender);
    }

    // Parametrización de las montañas adicionales dentro de la zona
    int numAdditionalMountains = 3;  // Número de montañas adicionales a generar

    // Crear montañas adicionales dentro de la zona
    for (int i = 0; i < numAdditionalMountains; ++i) {
        // Generar una posición aleatoria dentro del rango de las esquinas
        float randomX = (rand() % 500) - 400;  // Rango entre -400 y 100
        float randomZ = (rand() % 800) - 400;  // Rango entre -400 y 400

        PxVec3 mountainPosition = PxVec3(randomX, -5.0f, randomZ);
        PxVec3 mountainScale = PxVec3(30.0f, 50.0f, 30.0f); // Tamaño de la montaña

        // Crear una cápsula para representar la montaña
        PxRigidStatic* mountain = gPhysics->createRigidStatic(PxTransform(mountainPosition));
        PxShape* mountainShape = CreateShape(PxCapsuleGeometry(mountainScale.x, mountainScale.y)); // Usando el escalado en la geometría
        mountain->attachShape(*mountainShape);
        gScene->addActor(*mountain);

        // Renderizar la forma de la montaña
        RenderItem* mountainRender = new RenderItem(mountainShape, mountain, { 0.4f, 0.3f, 0.2f, 1.0f }); // Marrón claro
        renderItems.push_back(mountainRender);
    }
}

void Level1::createFinishParticles()
{
    // Posición del jugador (bola)
    PxVec3 playerPosition = golfBall->getGlobalPose().p;

    // Dirección de dispersión de partículas
    PxVec3 forwardDirection = PxVec3(0, 0, 1);  // Dirección hacia adelante (Y positivo)
    PxVec3 backwardDirection = PxVec3(0, 0, -1); // Dirección hacia atrás (Y negativo)
    PxVec3 leftDirection = PxVec3(-1, 0, 0);  // Dirección hacia la izquierda (X negativo)
    PxVec3 rightDirection = PxVec3(1, 0, 0);  // Dirección hacia la derecha (X positivo)

    // Creación de cuatro sistemas de partículas alrededor del jugador
    // Partículas hacia adelante
    myParticleSystem->CreateUniformGenerator(playerPosition - PxVec3(0, 5, 0) + forwardDirection * 20.0f, PxVec3(0, 30, 0), 30, 1.0f, 1.0f, spawnDistribution::UNIFORM, 20.0f, 2);

    // Partículas hacia atrás
    myParticleSystem->CreateUniformGenerator(playerPosition - PxVec3(0, 5, 0) + backwardDirection * 20.0f, PxVec3(0, 30, 0), 30, 1.0f, 1.0f, spawnDistribution::UNIFORM, 20.0f, 2);

    // Partículas hacia la izquierda
    myParticleSystem->CreateUniformGenerator(playerPosition - PxVec3(0, 5, 0) + leftDirection * 20.0f, PxVec3(0, 30, 0), 30, 1.0f, 1.0f, spawnDistribution::UNIFORM, 20.0f, 2);

    // Partículas hacia la derecha
    myParticleSystem->CreateUniformGenerator(playerPosition - PxVec3(0, 5, 0) + rightDirection * 20.0f, PxVec3(0, 30, 0), 30, 1.0f, 1.0f, spawnDistribution::UNIFORM, 20.0f, 2);
}



void Level1::checkDeathHeight()
{
    if (golfBall->getGlobalPose().p.y <= DeadHeight) respawnAtLastCheckpoint();
}

void Level1::respawnAtLastCheckpoint()
{
    golfBall->setGlobalPose(PxTransform(nextSpawnPosition));
}

void Level1::spawnWater()
{
    // Definir las dimensiones del plano (por ejemplo, un plano grande)
    PxVec3 planeSize(1000.0f, 1.0f, 1000.0f);  // Un plano de 100x100 en X y Z, con una altura de 1

    // Crear el RenderItem para el plano
    PxTransform planeTransform(PxVec3(0.0f, DeadHeight, 0.0f));  // Posición del plano (ajustado para la altura de muerte)

    // Crear la geometría para el plano (en este caso, una caja)
    PxShape* planeShape = CreateShape(PxBoxGeometry(planeSize.x, planeSize.y, planeSize.z));

    // Crear el RenderItem con el material azul
    RenderItem* bluePlane = new RenderItem(planeShape, new PxTransform(planeTransform), Vector4(0.3f, 0.7f, 1.0f, 1.0f));  // Un azul claro // Azul: (0, 0, 1, 1)

    // Añadir el RenderItem a la escena
    myFloor = bluePlane;  // Usamos 'myFloor' como el contenedor de este RenderItem
}

void Level1::createTrackSegment2(const PxVec3& position, const PxQuat& rotation, const PxVec3& size) {
    // Crear el césped
    //PxRigidStatic* cesped = gPhysics->createRigidStatic(PxTransform(position, rotation));
    //PxShape* cespedShape = CreateShape(PxBoxGeometry(size.x, 0.1f, size.z));
    //cesped->attachShape(*cespedShape);
    //gScene->addActor(*cesped);
    //renderItems.push_back(new RenderItem(cespedShape, cesped, { 0.0f, 0.8f, 0.0f, 1.0f })); // Verde césped

    // Crear la pista
    PxRigidStatic* pista = gPhysics->createRigidStatic(PxTransform(position, rotation));
    PxShape* pistaShape = CreateShape(PxBoxGeometry(size.x, 0.1f, size.z - 2 * bordilloAncho)); // Menos los bordillos
    pista->attachShape(*pistaShape);
    gScene->addActor(*pista);
    renderItems.push_back(new RenderItem(pistaShape, pista, { 0.05f, 0.5f, 0.05f, 1.0f })); // Verde claro

    // Crear bordillos superior e inferior
    /*PxRigidStatic* bordilloSup = gPhysics->createRigidStatic(PxTransform(position + PxVec3(0, bordilloAlto / 2, size.z - bordilloAncho), rotation));
    PxShape* bordilloSupShape = CreateShape(PxBoxGeometry(size.x, bordilloAlto / 2, bordilloAncho));
    bordilloSup->attachShape(*bordilloSupShape);
    gScene->addActor(*bordilloSup);
    renderItems.push_back(new RenderItem(bordilloSupShape, bordilloSup, bordilloColor));*/

    /*PxRigidStatic* bordilloInf = gPhysics->createRigidStatic(PxTransform(position + PxVec3(0, bordilloAlto / 2, -(size.z - bordilloAncho)), rotation));
    PxShape* bordilloInfShape = CreateShape(PxBoxGeometry(size.x, bordilloAlto / 2, bordilloAncho));
    bordilloInf->attachShape(*bordilloInfShape);
    gScene->addActor(*bordilloInf);
    renderItems.push_back(new RenderItem(bordilloInfShape, bordilloInf, bordilloColor));*/

    // Crear bordillos izquierdo y derecho
    PxRigidStatic* bordilloIzq = gPhysics->createRigidStatic(PxTransform(position + PxVec3(-(size.x - bordilloAncho), bordilloAlto / 2, 0), rotation));
    PxShape* bordilloIzqShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, size.z));
    bordilloIzq->attachShape(*bordilloIzqShape);
    gScene->addActor(*bordilloIzq);
    renderItems.push_back(new RenderItem(bordilloIzqShape, bordilloIzq, bordilloColor));

    PxRigidStatic* bordilloDer = gPhysics->createRigidStatic(PxTransform(position + PxVec3(size.x - bordilloAncho, bordilloAlto / 2, 0), rotation));
    PxShape* bordilloDerShape = CreateShape(PxBoxGeometry(bordilloAncho, bordilloAlto / 2, size.z));
    bordilloDer->attachShape(*bordilloDerShape);
    gScene->addActor(*bordilloDer);
    renderItems.push_back(new RenderItem(bordilloDerShape, bordilloDer, bordilloColor));
}

void Level1::createObstacleGenerator(int obs) {
    // Número de obstáculos que quieres crear
    int numObstacles = obs;

    // Zona del hoyo 2 (donde se generarán los obstáculos)
    // Usamos las dimensiones de la pista de hoyo 2
    PxVec3 minPosition(-250.0f, 5.0f, -20.0f);  // Esquina inferior izquierda
    PxVec3 maxPosition(-140.0f, 5.0f, 20.0f);     // Esquina superior derecha

    // Rango de tamaño para los obstáculos (estos tamaños pueden ajustarse según se desee)
    PxVec3 minSize(1.0f, 1.0f, 1.0f);
    PxVec3 maxSize(3.0f, 3.0f, 3.0f);

    for (int i = 0; i < numObstacles; i++) {
        // Generar una posición aleatoria dentro del rango de la zona de hoyo 2
        PxVec3 randomPosition(
            minPosition.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPosition.x - minPosition.x))),
            minPosition.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPosition.y - minPosition.y))),
            minPosition.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxPosition.z - minPosition.z)))
        );

        // Generar un tamaño aleatorio dentro del rango definido
        PxVec3 randomSize(
            minSize.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxSize.x - minSize.x))),
            minSize.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxSize.y - minSize.y))),
            minSize.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxSize.z - minSize.z)))
        );

        // Generar la rotación aleatoria
        PxQuat randomRotation(PxIdentity); // Si deseas que los obstáculos estén rotados aleatoriamente, puedes generar un valor aleatorio aquí

        // Crear el RigidBody (obstáculo) con la posición y tamaño aleatorio
        RigidBody* rb = new RigidBody(gPhysics, gScene, randomPosition, PxVec3(0, 0, 0), PxVec3(0, 0, 0),
            randomSize, 0.5f, PxVec4(0, 0, 1, 1), 0);

        // Agregar el RigidBody a la lista de cuerpos rígidos
        myParticleSystem->AddRigidBody(rb);
    }
}


void Level1::createHole1() {
    createTrackSegment({ -40, -1, 0 }, PxQuat(PxIdentity), { 50, 0.1f, 10 });

    // Agregar el hoyo
    PxRigidStatic* hoyo = gPhysics->createRigidStatic(PxTransform(holesPositions[0]));
    PxShape* hoyoShape = CreateShape(PxSphereGeometry(0.5f));
    hoyo->attachShape(*hoyoShape);
    gScene->addActor(*hoyo);
    renderItems.push_back(new RenderItem(hoyoShape, hoyo, { 0.0f, 0.0f, 0.0f, 1.0f })); // Negro
}

void Level1::createHole2() {
    createTrackSegment({ -200, -1, 0 }, PxQuat(PxIdentity), { 75.0f, 0.1f, 20.0f });

    RigidBody* rb = new RigidBody(gPhysics, gScene, PxVec3(-200, -1, 0), PxVec3(0, 0, 0), PxVec3(0, 0, 0),
        PxVec3(3, 2, 1), 0.5, PxVec4(0, 0, 1, 1), 0);
    myParticleSystem->AddRigidBody(rb);
    createObstacleGenerator(25);

    // Agregar el hoyo
    PxRigidStatic* hoyo2 = gPhysics->createRigidStatic(PxTransform(holesPositions[1]));
    PxShape* hoyo2Shape = CreateShape(PxSphereGeometry(0.5f));
    hoyo2->attachShape(*hoyo2Shape);
    gScene->addActor(*hoyo2);
    renderItems.push_back(new RenderItem(hoyo2Shape, hoyo2, { 0.0f, 0.0f, 0.0f, 1.0f })); // Negro
}

void Level1::createHole3() {
    createTrackSegment2({ 0, -1, 70 }, PxQuat(PxIdentity), { 50, 0.1f, 25 });

    // Agregar el hoyo
    PxRigidStatic* hoyo = gPhysics->createRigidStatic(PxTransform(holesPositions[2]));
    PxShape* hoyoShape = CreateShape(PxSphereGeometry(0.5f));
    hoyo->attachShape(*hoyoShape);
    gScene->addActor(*hoyo);
    renderItems.push_back(new RenderItem(hoyoShape, hoyo, { 0.0f, 0.0f, 0.0f, 1.0f })); // Negro

    
    /*windParticleSystem->CreateUniformGenerator(auxSpawn1BR, PxVec3(0, 5, 20), 2, 20.0f, 5.0f, spawnDistribution::UNIFORM, 30.0f, 2);
    windParticleSystem->CreateUniformGenerator(auxSpawn2BR, PxVec3(0, 5, -20), 2, 20.0f, 5.0f, spawnDistribution::UNIFORM, 30.0f, 2);
    windParticleSystem->CreateUniformGenerator(auxSpawn3BR, PxVec3(0, 5, 20), 2, 20.0f, 5.0f, spawnDistribution::UNIFORM, 30.0f, 2);*/

    windParticleSystem->Wind(auxSpawn1BR, PxVec3(5, 5, 5), PxVec3(0, 0, 50), 20);
    windParticleSystem->Wind(auxSpawn2BR, PxVec3(5, 5, 5), PxVec3(0, 0, -50), 20);
    windParticleSystem->Wind(auxSpawn3BR, PxVec3(5, 5, 5), PxVec3(0, 0, 50), 20);

}

void Level1::createHole4() {
    createTrackSegment2({ -165, -1, 70 }, PxQuat(PxIdentity), { 75, 0.1f, 25 });

    // Agregar el hoyo
    PxRigidStatic* hoyo = gPhysics->createRigidStatic(PxTransform(holesPositions[3]));
    PxShape* hoyoShape = CreateShape(PxSphereGeometry(0.5f));
    hoyo->attachShape(*hoyoShape);
    gScene->addActor(*hoyo);
    renderItems.push_back(new RenderItem(hoyoShape, hoyo, { 0.0f, 0.0f, 0.0f, 1.0f })); // Negro


    //gravityParticleSystem->CreateUniformGenerator(auxSpawn1BV, PxVec3(0, -5, 0), 10, 20.0f, 5.0f, spawnDistribution::UNIFORM, 30.0f, 2);
    //gravityParticleSystem->CreateUniformGenerator(auxSpawn2BV, PxVec3(0, 5, 0), 2, 20.0f, 5.0f, spawnDistribution::UNIFORM, 30.0f, 2);
    //gravityParticleSystem->CreateUniformGenerator(auxSpawn3BV, PxVec3(0, 5, 0), 2, 20.0f, 5.0f, spawnDistribution::UNIFORM, 30.0f, 2);

    gravityParticleSystem->Whirlwind(auxSpawn1BV, PxVec3(30.0f, 50.0f, 30.0f), 0.5f, 25);
    //gravityParticleSystem->Gravity(15.0f);

}

void Level1::createHole5() {
    // Crear el segmento de pista
    createTrackSegment2({ -100, -1, 170 }, PxQuat(PxIdentity), { 75, 0.1f, 20 });

    // Agregar el hoyo
    PxRigidStatic* hoyo = gPhysics->createRigidStatic(PxTransform(holesPositions[4]));
    PxShape* hoyoShape = CreateShape(PxSphereGeometry(0.5f));
    hoyo->attachShape(*hoyoShape);
    gScene->addActor(*hoyo);
    renderItems.push_back(new RenderItem(hoyoShape, hoyo, { 0.0f, 0.0f, 0.0f, 1.0f })); // Negro

    // Parámetros comunes
    PxVec3 initialAnchorPosition = PxVec3(holesPositions[4].x + 100, holesPositions[4].y + 30, holesPositions[4].z + 10);
    PxVec3 rigidBodyDimensions = PxVec3(5, 5, 5);
    float rigidBodyMass = 0.51f;
    float springConstant = 0.5f;
    float restLength = 10.0f;
    Vector4 rigidBodyColor = Vector4(0, 0, 1, 1); // Azul

    // Crear los tres muelles de derecha a izquierda en tres filas desplazadas en X
    for (int j = 0; j < 3; ++j) { // Bucle para las filas en el eje X
        PxVec3 rowAnchorPosition = initialAnchorPosition - PxVec3(40 * j, 0, 0); // Separación entre filas en X

        for (int i = 0; i < 3; ++i) { // Bucle para los muelles en el eje Z
            PxVec3 currentAnchorPosition = rowAnchorPosition - PxVec3(0, 0, 10 * i); // Separación entre muelles en Z
            ParticleSystem* auxPS = new ParticleSystem();
            auxPS->Gravity(-9.8);   //Contrarrestogravedad
            anchoredSystemVector.push_back(auxPS);
            auxPS->NewAnchor(
                gPhysics,                      // Motor de física
                gScene,                        // Escena
                currentAnchorPosition,         // Posición del anclaje
                currentAnchorPosition,         // Posición del RigidBody
                rigidBodyDimensions,           // Dimensiones del RigidBody
                rigidBodyMass,                 // Masa del RigidBody
                springConstant,                // Constante del resorte
                restLength,                    // Longitud en reposo del resorte
                rigidBodyColor                 // Color del RigidBody
            );
        }
    }
}



