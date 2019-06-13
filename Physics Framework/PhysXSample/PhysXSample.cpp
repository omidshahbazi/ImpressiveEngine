#include "PhysXSample.h"
#include "Rigidbody.h"
#include "IeConvert.h"
#include "IeDebugDrawer.h"
#include "CollisionCallback.h"
#include "OIS.h"
#include "common\PxIO.h"
#include <iostream>


std::ofstream out("C:/log.txt");
PxVec3* p;

bool bFired = false;
bool bRayCasted = false;

PxScene *m_PxScene;
bool m_bTransformDone = true;

//class ThreadWorker
//{
//public:
//	ThreadWorker()
//	{
//	}
//
//	~ThreadWorker() {};
//
//	void Do(void)
//	{
//		if (!m_bTransformDone)
//		{
//			unsigned int nbActiveTransforms = 0;
//			PxActiveTransform *activeTransforms = m_PxScene->getActiveTransforms(nbActiveTransforms);
//
//			for (unsigned int i = 0; i < nbActiveTransforms; i++)
//			{
//				PseudoGameObject *renderObject = static_cast<PseudoGameObject*>(activeTransforms[i].userData);
//				renderObject->SetTransform(IeConvert::ToIeTransform(activeTransforms[i].actor2World));
//			}
//
//			m_bTransformDone = true;
//		}
//	}
//
//	void operator() (void)
//	{
//		Do();
//
//	}
//
//};



struct FilterGroup
{
	enum Enum
	{
		eDynamic			=		(1 << 0),
		eStatic				=		(1 << 1),
		eIgnore				=		(1 << 2),
		ePlane				=		(1 << 3)
	};
};

struct FilterDynamic
{
	enum Enum
	{
		eNone			=		(1 << 0),
		eCCD			=		(1 << 1)
	};
};


static int pairFlagID = 1;
void SetupFiltering(PxRigidActor *actor, unsigned int filterGroup, unsigned int filterMask, unsigned int filterDynamic = FilterDynamic::eNone)
{
	PxFilterData filterData;

	filterData.word0 = filterGroup;
	filterData.word1 = filterMask;
	filterData.word2 = PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
	filterData.word3 = filterDynamic;

	const unsigned int nbShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[sizeof(PxShape*) * nbShapes];
	actor->getShapes(shapes, nbShapes);

	for (unsigned int i = 0; i < nbShapes; i++)
	{
		PxShape* shape = shapes[0];
		shape->setSimulationFilterData(filterData);
	}

	delete[] shapes;
}


PhysXSample::PhysXSample(void) : m_Foundation(0),
								 m_PhysicsWorld(0),
								 //m_PxScene(0),
								 m_CollisionCallback(0),
								 m_IDebugDrawer(0),
								 m_Cloth(0)
{
}


PhysXSample::~PhysXSample(void)
{
	release();
}


void PhysXSample::InitializeRendering(Ogre::SceneManager *SceneManager, Ogre::Camera *Camera, Ogre::DebugDrawer *DebugDrawer)
{
	bulletCounter = 0;
	m_Counter = 0;

	m_Scene = SceneManager;
	m_DebugDrawer = DebugDrawer;

	m_IDebugDrawer = new IeDebugDrawer(DebugDrawer);

	Camera->setPosition(0, 20, -20);
	Camera->lookAt(0, 0, 0);
	m_Camera = Camera;
}


PxFilterFlags PhysXSample::SampleFilterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
    {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
            return PxFilterFlag::eDEFAULT;
    }

	if ((filterData0.word0 != 0 || filterData1.word0 != 0) &&
					!(filterData0.word0 & filterData1.word1 || filterData1.word0 & filterData0.word1))
	{
		return PxFilterFlag::eSUPPRESS;
	}

	pairFlags = PxPairFlag::eNOTIFY_CONTACT_POINTS;
	pairFlags |= PxPairFlag::eCONTACT_DEFAULT;

	if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	}

	//if (filterData0.word3 & FilterDynamic::eCCD || filterData1.word3 & FilterDynamic::eCCD)
	//{
	//	pairFlags |= PxPairFlag::eRESOLVE_CONTACTS;
	//	pairFlags |= PxPairFlag::eSWEPT_INTEGRATION_LINEAR;
	//}

	pairFlags |= PxPairFlags(filterData0.word2 | filterData1.word2);
	
	return PxFilterFlags();
}

void PhysXSample::InitializePhysics(void)
{
	force = PxVec3(0, 0, 0);
	mGravity = PxVec3(0, -9.8f, 0);

	PxDefaultErrorCallback errorCallback;

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocator, errorCallback);

	m_PhysicsWorld = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true);

	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, m_PhysicsWorld->getFoundation(), PxCookingParams());

	m_Rigidbody = new Rigidbody(m_PhysicsWorld, m_Cooking);

	PxSceneDesc sceneDesc(m_PhysicsWorld->getTolerancesScale());
	sceneDesc.gravity = mGravity;
	sceneDesc.filterShader = &SampleFilterShader;
	m_CollisionCallback = new CollisionCallback();
	sceneDesc.simulationEventCallback = m_CollisionCallback;
	sceneDesc.contactCorrelationDistance = 0.01f;
	//sceneDesc.bounceThresholdVelocity = 0.1f;
	sceneDesc.flags |= PxSceneFlag::eENABLE_SWEPT_INTEGRATION;
	//sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
	sceneDesc.sweptIntegrationLinearSpeedFactor = 2;
	sceneDesc.sweptIntegrationAngularSpeedFactor = 2;

	const unsigned int mNbThreads = 10;
	PxDefaultCpuDispatcher *cpuDispatcher = PxDefaultCpuDispatcherCreate(mNbThreads);
	if(cpuDispatcher)
	{
		sceneDesc.cpuDispatcher = cpuDispatcher;
	}

	//pxtask::CudaContextManagerDesc cudaContexManagerDesc;
	//pxtask::CudaContextManager* m_CudaContextManager = pxtask::createCudaContextManager(*m_Foundation, cudaContexManagerDesc, 0);

	//if(!sceneDesc.gpuDispatcher && m_CudaContextManager)
	//{
	//	sceneDesc.gpuDispatcher = m_CudaContextManager->getGpuDispatcher();
	//}

	m_PxScene = m_PhysicsWorld->createScene(sceneDesc);

	m_PxScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1);
	m_PxScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1);
	m_PxScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1);
	m_PxScene->setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);

	m_Material = m_PhysicsWorld->createMaterial(0.5f, 0.5f, 0);
	
	PseudoGameObject *Node = NULL;

	Node = AddGameObject(m_Scene, "Plane", "Plane.mesh", Vector3(0, 0, 0));
	Node->SetScale(Vector3(10, 1, 10));
	Node->SetTag("Plane");
	PxRigidStatic *rigid = m_Rigidbody->CreateStaticRigidbody(Node->IsPrimitive, PxGeometryType::eBOX, Node, *m_Material, false);
	rigid->userData = Node;
	SetupFiltering(rigid, FilterGroup::eStatic, ~0);
	m_PxScene->addActor(*rigid);

	bulletID = m_PxScene->createClient();

	CreateObjectSet1();
}


PxRigidDynamic *PhysXSample::AddRigidDynamicActor(const Ogre::String &Name,
										const Ogre::String &meshName,
										const PxGeometryType::Enum &geometyType,
										const PxMaterial &material,
										const float &density,
										IeTransform &Transform,
										Vector3 &scale,
										bool UseDynamicCCD,
										bool IsTrigger)
{
	PseudoGameObject *node = AddGameObject(m_Scene, Name, meshName, Transform.p);
	//node->OriginNode->rotate(Vector3(-1, 0, -1), Radian(60));
	node->SetScale(scale);
	node->SetTag(Name);

	PxRigidDynamic *rigidbody = m_Rigidbody->CreateDynamicRigidbody(node->IsPrimitive, geometyType, node, *m_Material, density, UseDynamicCCD, IsTrigger);
	rigidbody->setLinearDamping(0.0f);
	rigidbody->setAngularDamping(0.05f);

	if (rigidbody)
	{
		rigidbody->userData = node;
		m_PxScene->addActor(*rigidbody);
		return rigidbody;
	}

	return NULL;
}


static float mAccumulator = 0.0f;
static float mStepSize = 1.0f / 60.0f;

void PhysXSample::Update(const unsigned long ElapsedTime)
{
	m_DeltaTime  = (float)ElapsedTime / 1000.f;
	//out << "delta time: " << m_DeltaTime << '\n';

	mAccumulator += m_DeltaTime;

	if (mAccumulator < mStepSize || !m_bTransformDone)
	{
		return;
	}

	mAccumulator -= mStepSize;

	m_PxScene->simulate(mStepSize);
	m_PxScene->fetchResults(true);


	unsigned int nbActiveTransforms = 0;
	PxActiveTransform *activeTransforms = m_PxScene->getActiveTransforms(nbActiveTransforms);

	for (unsigned int i = 0; i < nbActiveTransforms; i++)
	{
		UpdatePose(activeTransforms[i]);
	}

}


void PhysXSample::release(void)
{
	m_PhysicsWorld->release();
	m_Foundation->release();
}


void PhysXSample::UpdatePose(PxActiveTransform &activeTransform)
{

	PseudoGameObject *renderObject = static_cast<PseudoGameObject*>(activeTransform.userData);

	Vector3 &pos = IeConvert::ToVector3(activeTransform.actor2World.p);

	if (renderObject->IsPrimitive)
	{
		Vector3 &diff = renderObject->GetPosition() - renderObject->GetWorldAABB().getCenter();
		pos += diff;
	}

	renderObject->SetTransform(pos, IeConvert::ToQuaternion(activeTransform.actor2World.q));
}

static unsigned int nums = 0;

void PhysXSample::HandleInput(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse)
{
	if (Keyboard->isKeyDown(OIS::KC_I))
	{
		m_Actor->addForce(IeConvert::ToPxVec3(m_Camera->getOrientation() * Vector3::UNIT_Z * 50));
	}
	if (Keyboard->isKeyDown(OIS::KC_SPACE) && nums < 2000 && !bFired)
	{
		bulletCounter++;
		PxRigidDynamic* rigidbody = AddRigidDynamicActor("Bullet" + Ogre::StringConverter::toString(bulletCounter), "Sphere.mesh", PxGeometryType::eSPHERE, *m_Material, 1, IeTransform(m_Camera->getPosition()), Vector3(0.5f, 0.5f, 0.5f), true);
		SetupFiltering(rigidbody, FilterGroup::eDynamic, FilterGroup::eDynamic | FilterGroup::eStatic, FilterDynamic::eCCD);
		((PseudoGameObject*)rigidbody->userData)->SetTag("Bullet");
		Vector3 vel = m_Camera->getOrientation() * -Ogre::Vector3::UNIT_Z;
		vel.normalise();
		vel *= 1000;
		rigidbody->setLinearVelocity(IeConvert::ToPxVec3(vel));
		rigidbody->setAngularVelocity(PxVec3(10, 0, 10));
		bFired = true;
		nums++;
	}
	else if (!Keyboard->isKeyDown(OIS::KC_SPACE))
	{
		bFired = false;
	}
	if (!Keyboard->isKeyDown(OIS::KC_LCONTROL) && Keyboard->isKeyDown(OIS::KC_R) && !bRayCasted)
	{
		PxVec3 origin = IeConvert::ToPxVec3(m_Camera->getPosition());
		PxVec3 dir = IeConvert::ToPxVec3(m_Camera->getDirection());
		float dist = 50;
		PxRaycastHit hit;
		PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;
		bool found = m_PxScene->raycastSingle(origin, dir, dist, outputFlags, hit);
		if (found)
		{
			out << "Raycast found: " << ((PseudoGameObject*)hit.shape->userData)->OriginNode->getName() << '\n';
		}
		else
		{
			out << "Raycast not found: " << "" << '\n';
		}
		bRayCasted = true;
	}
	else if (!Keyboard->isKeyDown(OIS::KC_LCONTROL) && !Keyboard->isKeyDown(OIS::KC_R))
	{
		bRayCasted = false;
	}

	if (Keyboard->isKeyDown(OIS::KC_LCONTROL) && Keyboard->isKeyDown(OIS::KC_R) && !bRayCasted)
	{
		PxVec3 origin = IeConvert::ToPxVec3(m_Camera->getPosition());
		PxVec3 dir = IeConvert::ToPxVec3(m_Camera->getDirection());
		float dist = 50;
		bool blockHit;
		const unsigned int bufferSize = 256;
		PxRaycastHit hitBuffer[bufferSize];
		PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE | PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL;

		unsigned int nbHits = m_PxScene->raycastMultiple(origin, dir, dist, outputFlags, hitBuffer, bufferSize, blockHit);

		if (nbHits)
		{
			for (int i = 0; i < nbHits; i++)
			{
				out << "Raycast found: " << ((PseudoGameObject*)hitBuffer[i].shape->userData)->OriginNode->getName() << '\n';
			}
		}
		else
		{
			out << "Raycast not found: " << "" << '\n';
		}
		bRayCasted = true;
	}
	else if (!Keyboard->isKeyDown(OIS::KC_LCONTROL) && !Keyboard->isKeyDown(OIS::KC_R))
	{
		bRayCasted = false;
	}
}


PxVec3 PhysXSample::ApplyForceToActor(PxRigidDynamic* actor, const PxVec3 &force, const float &strength)
{
	PxVec3 forceVec = force * strength * m_DeltaTime;
	actor->addForce(forceVec);
	return forceVec;
}


void PhysXSample::DrawForce(PxRigidDynamic* actor, const PxVec3 &force, const Ogre::ColourValue &Color)
{
	float forceLength = force.magnitude();
	if (forceLength < 0.1f)
	{
		return;
	}

	PxVec3 forceVec = 3 * force / forceLength;

	PxVec3 pos = actor->getGlobalPose().p;
	PxVec3 to = pos + forceVec;

	//DrawLine(Vector3(pos.x, pos.y, pos.z), Vector3(to.x, to.y, to.z), ColourValue::Blue);
}


void PhysXSample::CreateCloth()
{
	PxClothMeshDesc clothDesc;

}


void PhysXSample::CreateObjectSet1(void)
{
	m_Actor = AddRigidDynamicActor("Box1", "Cube.mesh", PxGeometryType::eBOX, *m_Material, 1, IeTransform(Vector3(0, 0.5, 0)));
	SetupFiltering(m_Actor, FilterGroup::eDynamic, ~0);

	PxRigidDynamic *rigid = AddRigidDynamicActor("Box2", "Cube.mesh", PxGeometryType::eBOX, *m_Material, 10, IeTransform(Vector3(0, 20, -0.3f)), Vector3(1, 1, 1), false, false);
	SetupFiltering(rigid, FilterGroup::eDynamic, FilterGroup::eDynamic | FilterGroup::eStatic);

	rigid = AddRigidDynamicActor("Box3", "Cube.mesh", PxGeometryType::eBOX, *m_Material, 10, IeTransform(Vector3(0, 10, -0.3f)), Vector3(1, 1, 1), false, true);
	rigid->setLinearDamping(10);
	SetupFiltering(rigid, FilterGroup::eDynamic, FilterGroup::eDynamic | FilterGroup::eStatic);
}
