#include "Rigidbody.h"
#include "IeConvert.h"
#include "MeshInfo.h"
#include "PseudoGameObject.h"


Rigidbody::Rigidbody(PxPhysics *Physics, PxCooking* Cooking) : m_Physics(Physics), m_Cooking(Cooking)
{
}


Rigidbody::~Rigidbody(void)
{
	m_Cooking->release();
}


MeshInfo *GetMeshInfo(PseudoGameObject* gameObject, const bool ProcessIndices = false)
{
	MovableObject* object = gameObject->OriginNode->getAttachedObject(0);
	Entity* entity = static_cast<Entity*>(object);
	if(!entity) return 0;

	MeshParams Params;
	Params.Scale = gameObject->GetScale();

	return new MeshInfo(entity->getMesh(), Params, ProcessIndices);
}



PxConvexMesh* Rigidbody::CreateConvexMesh(MeshInfo* meshInfo)
{
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = meshInfo->Vertices.size();
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = meshInfo->GetVerticesVectorPtr();
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	PxDefaultMemoryOutputStream buf;
	if (!m_Cooking->cookConvexMesh(convexDesc, buf))
	{
		return NULL;
	}

	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());

	return m_Physics->createConvexMesh(input);
}


PxTriangleMesh* Rigidbody::CreateTriangleMesh(MeshInfo* meshInfo)
{
	PxTriangleMeshDesc triangleDesc;
	triangleDesc.points.count = meshInfo->Vertices.size();
	triangleDesc.points.stride = sizeof(PxVec3);
	triangleDesc.points.data = meshInfo->GetVerticesFloatPtr();

	triangleDesc.triangles.count = meshInfo->Indices.size() / 3;
	triangleDesc.triangles.stride = 3 * sizeof(unsigned int);
	triangleDesc.triangles.data = meshInfo->GetIndicesUIntPtr();


	PxDefaultMemoryOutputStream buf;
	if (!m_Cooking->cookTriangleMesh(triangleDesc, buf))
	{
		return NULL;
	}

	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	return m_Physics->createTriangleMesh(input);
}


PxRigidDynamic *Rigidbody::CreateDynamicRigidbody(bool &IsPrimitive,
										const PxGeometryType::Enum &geometryType,
										PseudoGameObject *gameObject,
										const PxMaterial &material,
										const float &density,
										bool UseDynamicCCD,
										bool IsTrigger)
{
	PxTransform transform = PxTransform(IeConvert::ToPxTransform(gameObject->GetTransform()));

	PxRigidDynamic *rigidbody = m_Physics->createRigidDynamic(transform);

	PxShape *meshShape = NULL;

	const Vector3 &scale = gameObject->GetScale();
	Vector3 &HalfSize = (gameObject->GetEntity()->getBoundingBox().getHalfSize() * scale) - (scale * 0.009999990f);

	if (HalfSize.x < 0) HalfSize.x = 0;
	if (HalfSize.y < 0) HalfSize.y = 0;
	if (HalfSize.z < 0) HalfSize.z = 0;

	IsPrimitive = true;

	if (geometryType == PxGeometryType::eBOX)
	{
		meshShape = rigidbody->createShape(PxBoxGeometry(IeConvert::ToPxVec3(HalfSize)), material);
	}
	else if (geometryType == PxGeometryType::eSPHERE)
	{
		meshShape = rigidbody->createShape(PxSphereGeometry(HalfSize.y), material);
	}
	else if (geometryType == PxGeometryType::eCAPSULE)
	{
		PxTransform relativeTransform(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
		meshShape = rigidbody->createShape(PxCapsuleGeometry(HalfSize.x, HalfSize.y * 0.5f), material, relativeTransform);
	}
	else
	{
		IsPrimitive = false;

		MeshInfo *meshInfo;

		if (geometryType == PxGeometryType::eCONVEXMESH)
		{
			 meshInfo = GetMeshInfo(gameObject);

			meshShape = rigidbody->createShape(PxConvexMeshGeometry(CreateConvexMesh(meshInfo)), material);
		}
		else if (geometryType == PxGeometryType::eTRIANGLEMESH)
		{
			meshInfo = GetMeshInfo(gameObject, true);

			meshShape = rigidbody->createShape(PxTriangleMeshGeometry(CreateTriangleMesh(meshInfo)), material);

			PxShape *convexShape = rigidbody->createShape(PxConvexMeshGeometry(CreateConvexMesh(meshInfo)), material);

			meshShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			convexShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
			rigidbody->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, false);
		}

		delete meshInfo;
	}
	
	meshShape->userData = gameObject;

	if (UseDynamicCCD)
	{
		meshShape->setFlag(PxShapeFlag::eUSE_SWEPT_BOUNDS, true);
	}

	meshShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, IsTrigger);

	PxRigidBodyExt::updateMassAndInertia(*rigidbody, density);

	return rigidbody;
}


PxRigidStatic *Rigidbody::CreateStaticRigidbody(bool &IsPrimitive,
										const PxGeometryType::Enum &geometryType,
										PseudoGameObject *gameObject,
										const PxMaterial &material,
										bool UseDynamicCCD,
										bool IsTrigger)
{
	PxTransform transform = PxTransform(IeConvert::ToPxVec3(gameObject->GetPosition()));

	PxRigidStatic *rigidbody = m_Physics->createRigidStatic(transform);

	PxShape *meshShape = NULL;

	const Vector3 &scale = gameObject->GetScale();
	Vector3 &HalfSize = (gameObject->GetEntity()->getBoundingBox().getHalfSize() * scale) - (scale * 0.009999990f);

	if (HalfSize.x < 0) HalfSize.x = 0;
	if (HalfSize.y < 0) HalfSize.y = 0;
	if (HalfSize.z < 0) HalfSize.z = 0;

	if (geometryType == PxGeometryType::eBOX)
	{
		meshShape = rigidbody->createShape(PxBoxGeometry(IeConvert::ToPxVec3(HalfSize)), material);
	}
	else if (geometryType == PxGeometryType::eSPHERE)
	{
		meshShape = rigidbody->createShape(PxSphereGeometry(HalfSize.y), material);
	}
	else if (geometryType == PxGeometryType::eCAPSULE)
	{
		PxTransform relativeTransform(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
		meshShape = rigidbody->createShape(PxCapsuleGeometry(HalfSize.x, HalfSize.y / 2), material, relativeTransform);
	}
	else
	{
		MeshInfo *meshInfo;

		if (geometryType == PxGeometryType::eCONVEXMESH)
		{
			meshInfo = GetMeshInfo(gameObject);

			meshShape = rigidbody->createShape(PxConvexMeshGeometry(CreateConvexMesh(meshInfo)), material);
		}
		else if (geometryType == PxGeometryType::eTRIANGLEMESH)
		{
			meshInfo = GetMeshInfo(gameObject, true);

			meshShape = rigidbody->createShape(PxTriangleMeshGeometry(CreateTriangleMesh(meshInfo)), material);

			PxShape *convexShape = rigidbody->createShape(PxConvexMeshGeometry(CreateConvexMesh(meshInfo)), material);
		}

		delete meshInfo;
	}

	meshShape->userData = gameObject;

	if (UseDynamicCCD)
	{
		meshShape->setFlag(PxShapeFlag::eUSE_SWEPT_BOUNDS, true);
	}

	meshShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, IsTrigger);

	return rigidbody;
}


