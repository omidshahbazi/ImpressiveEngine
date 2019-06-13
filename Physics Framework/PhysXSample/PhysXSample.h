#pragma once

#include "IPlugin.h"
#include "IPhysicsSample.h"
#include "PhysicsFramework.h"
#include "Cloth.h"
#include <PxPhysicsAPI.h>
#include <list>
#include <vector>

class Rigidbody;
class IeDebugDrawer;
class CollisionCallback;

using namespace physx;
using namespace Ogre;

class PhysXSample : public IPlugin, public IPhysicsSample
{
private:
	void Install(void) {}
	const char *GetName(void) const
	{
		return "PhysX Sample";
	}
	const char *GetVersion(void) const
	{
		return "1.0.0.0";
	}

	float m_DeltaTime;

	void InitializeRendering(Ogre::SceneManager *SceneManager, Ogre::Camera *Camera, Ogre::DebugDrawer *DebugDrawer);
	void InitializePhysics(void);
	void Update(const unsigned long ElapsedTime);

	void UpdatePose(PxActiveTransform &activeTransform);

	void HandleInput(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse);

	void ClearObjects(void);

	PxVec3 force;
	PxVec3 ApplyForceToActor(PxRigidDynamic* actor, const PxVec3 &force, const float &strength);
	void DrawForce(PxRigidDynamic* actor, const PxVec3 &force, const Ogre::ColourValue &Color);

	PxVec3 mGravity;

	unsigned long m_Counter;

	Cloth *m_Cloth;

	PxRigidDynamic *AddRigidDynamicActor(const Ogre::String &Name,
										 const Ogre::String &meshName,
										 const PxGeometryType::Enum &geometyType,
										 const PxMaterial &material,
										 const float &density,
										 IeTransform &Transform,
										 Vector3 &scale = Vector3(1, 1, 1),
										 bool UseDynamicCCD = false,
										 bool IsTrigger = false);
	void CreateCloth();

private:
	Ogre::SceneManager *m_Scene;
	Ogre::DebugDrawer *m_DebugDrawer;

	PxDefaultAllocator m_DefaultAllocator;
	PxFoundation *m_Foundation;
	PxCooking *m_Cooking;

	PxPhysics *m_PhysicsWorld;
	//PxScene *m_PxScene;
	CollisionCallback *m_CollisionCallback;
	PxMaterial *m_Material;

	Rigidbody *m_Rigidbody;
	IeDebugDrawer *m_IDebugDrawer;

	//std::vector<PseudoGameObject*> m_GameObjects;
	//std::vector<PxRigidDynamic*> m_RigidBodies;

	Ogre::Camera *m_Camera;

	PxParticleFluid *fluid;

	unsigned int bulletCounter;

public:
	PhysXSample(void);
	~PhysXSample(void);
	void release(void);

	static PxFilterFlags SampleFilterShader(
			PxFilterObjectAttributes attributes0, PxFilterData filterData0,
			PxFilterObjectAttributes attributes1, PxFilterData filterData1,
			PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);

	PxRigidBody *m_Actor;

private:

	PxClientID bulletID;
	void CreateObjectSet1(void);
};

