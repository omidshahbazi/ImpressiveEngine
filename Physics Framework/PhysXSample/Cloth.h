#pragma once

#include "PxPhysicsAPI.h"
#include "Ogre.h"
#include <vector>

using namespace physx;
using namespace Ogre;

class MeshInfo;
class PseudoGameObject;
struct IeTransform;

class Cloth : public ManualObject
{
private:
	PseudoGameObject *m_GameObject;
	ManualObject *m_ClothRenderer;


	PxClothFabric *m_ClothRabric;
	PxCloth *m_Cloth;
	PxClothCollisionSphere m_BoxCollider;
	PxClothCollisionSphere m_Collider[2];
	PxClothMeshDesc clothDesc;

	PxPhysics *m_Physics;
	PxCooking* m_Cooking;

	PxVec3 *m_NormalBuffer;
	unsigned int m_nbNormals;

	MeshInfo *GetMeshInfo(PseudoGameObject* gameObject, const bool ProcessIndices = false);

	PxClothCollisionData *CollisionData(void);
	PxClothParticle *ParticlePositions(void);

public:
	Cloth(const String& name, PxPhysics *Physics, PxCooking *Cooking, PseudoGameObject* GameObject, PxVec3 &Gravity);
	~Cloth(void);

	PxVec3 *m_VertexBuffer;
	unsigned int m_nbVectices;

	unsigned int *m_IndexBuffer;
	unsigned int m_nbIndices;

	PxClothFabric *CreateClothFabric(MeshInfo* meshInfo, PxVec3 &Gravity);

	IeTransform GetGlobalPose(void);

	PxCloth *GetClothActor(void);

	void Update(void);
	void Render(void);
};

