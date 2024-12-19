#include "RigidBody.h"

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene)
{
    actor = gPhysics->createRigidDynamic(PxTransform(0, 0, 0));

    actor->setLinearVelocity(PxVec3(0, 0, 0));
    actor->setAngularVelocity(PxVec3(0, 0, 0));
    
    PxShape* sh = CreateShape(PxBoxGeometry(1, 1, 1));
    actor->attachShape(*sh);

    PxRigidBodyExt::updateMassAndInertia(*actor, 0.1f);

    gScene->addActor(*actor);

    RenderItem* r = new RenderItem(sh, actor, PxVec4(1, 0, 0, 1));
}

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene, const PxVec3& position, const PxVec3& linearVelocity,
    const PxVec3& angularVelocity, const PxVec3& dimensions, double masa, Vector4 color, double t)
{
    lifeTime = t;
    // Crear el actor dinámico
    actor = gPhysics->createRigidDynamic(PxTransform(position));

    // Bloquear todas las rotaciones
    actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
    actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
    actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);

    // Configurar velocidades
    actor->setLinearVelocity(linearVelocity);
    actor->setAngularVelocity(angularVelocity);

    PxMaterial* material = gPhysics->createMaterial(0.5f, 0.5f, 0.0f);

    // Crear la forma del objeto
    shape = CreateShape(PxBoxGeometry(dimensions.x, dimensions.y, dimensions.z), material);
    actor->attachShape(*shape);

    //agregar inercia manual
    PxRigidBodyExt::updateMassAndInertia(*actor, 0.15);
    PxVec3 aux = calculateTensor(masa);
    actor->setMassSpaceInertiaTensor(aux);
    actor->setMass(masa);
    // Agregar el actor a la escena
    gScene->addActor(*actor);
    // Crear el item de renderizado
    renderItem = new RenderItem(shape, actor, color);
}

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene, const PxVec3& position, float radius, double masa, Vector4 color, double t)
{
    lifeTime = t;

    // Crear el actor dinámico en la posición proporcionada
    actor = gPhysics->createRigidDynamic(PxTransform(position));

    // Bloquear todas las rotaciones
    actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
    actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
    actor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);

    actor->setLinearVelocity(PxVec3(0,0,0));
    actor->setAngularVelocity(PxVec3(0, 0, 0));

    // Material para la esfera
    PxMaterial* material = gPhysics->createMaterial(0.5f, 0.5f, 0.0f);

    // Crear la forma de la esfera
    PxSphereGeometry sphereGeom(radius);  // Convertimos a float ya que PxSphereGeometry usa floats
    shape = CreateShape(sphereGeom, material);

    // Asociar la forma al actor
    actor->attachShape(*shape);

    // Calcular la inercia y la masa
    PxRigidBodyExt::updateMassAndInertia(*actor, 0.15f);
    PxVec3 aux = calculateTensor(masa);
    actor->setMassSpaceInertiaTensor(aux);
    actor->setMass(masa);

    // Agregar el actor a la escena
    gScene->addActor(*actor);

    // Crear el item de renderizado
    renderItem = new RenderItem(shape, actor, color);
}


RigidBody::~RigidBody()
{
    DeregisterRenderItem(renderItem);
    renderItem = nullptr;
}

void RigidBody::moveRight(float velocidad)
{
    PxVec3 posicion = actor->getGlobalPose().p;
    if (posicion.z > -17) { // Verificar límite derecho
        actor->setLinearVelocity(PxVec3(0, 0, -velocidad));
    }
    else {
        stop(); // Detener si llega al límite
    }
}

void RigidBody::moveLeft(float velocidad)
{
    PxVec3 posicion = actor->getGlobalPose().p;
    if (posicion.z < 17) { // Verificar límite izquierdo
        actor->setLinearVelocity(PxVec3(0, 0, velocidad));
    }
    else {
        stop(); // Detener si llega al límite
    }
}

void RigidBody::stop()
{
    actor->setLinearVelocity(PxVec3(0, 0, 0));
}

PxVec3 RigidBody::calculateTensor(float m)
{
    PxGeometryType::Enum tipo = shape->getGeometryType();
    if (tipo == PxGeometryType::eBOX)
    {
        PxBoxGeometry cubo;
        shape->getBoxGeometry(cubo);
        float x = 2 * cubo.halfExtents.x;
        float y = 2 * cubo.halfExtents.y;
        float z = 2 * cubo.halfExtents.z;

        return PxVec3((1.0f / 12.0f) * (y * y + z * z) * m,
            (1.0f / 12.0f)  * (x * x + z * z) * m,
            (1.0f / 12.0f)  * (x * x + y * y) * m);
    }
    else if (tipo == PxGeometryType::eSPHERE)
    {
        PxSphereGeometry esfera;
        shape->getSphereGeometry(esfera);

        float aux = (2.0f / 5.0f) * m * esfera.radius * esfera.radius;
        return PxVec3(aux, aux, aux);
    }
    else {
        return PxVec3(1.0, 1.0, 1.0);
    }
}
