#pragma once

#include "PxPhysicsAPI.h"

class MeshInfo;

class PseudoGameObject;

using namespace physx;

class Rigidbody
{
private:
	PxPhysics *m_Physics;
	PxCooking* m_Cooking;

	PxConvexMesh* CreateConvexMesh(MeshInfo* meshInfo);
	PxTriangleMesh* CreateTriangleMesh(MeshInfo* meshInfo);

public:
	Rigidbody(PxPhysics *Physics, PxCooking* Cooking);

	~Rigidbody(void);

	PxRigidStatic *CreateStaticRigidbody(bool &IsPrimitive,
										   const PxGeometryType::Enum &geometryType,
										   PseudoGameObject *gameObject,
										   const PxMaterial &material,
										   bool UseDynamicCCD = false,
										   bool IsTrigger = false);

	PxRigidDynamic *CreateDynamicRigidbody(bool &IsPrimitive,
										   const PxGeometryType::Enum &geometryType,
										   PseudoGameObject *gameObject,
										   const PxMaterial &material,
										   const float &density = 1,
										   bool UseDynamicCCD = false,
										   bool IsTrigger = false);
};

