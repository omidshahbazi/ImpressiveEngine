#include "BulletSample.h"


btRigidBody *CreateRigidBody(const float &Mass, const btVector3 &Position, btCollisionShape *Shape)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(Position);

	btVector3 localInertia;
	Shape->calculateLocalInertia(Mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo info(Mass, new btDefaultMotionState(transform), Shape, localInertia);
	return new btRigidBody(info);
}


btRigidBody *CreateBoxShape(const float &Mass, const btVector3 &Size, const btVector3 &Position = btVector3(0, 0, 0))
{
	return CreateRigidBody(Mass, Position, new btBoxShape(Size / 2));
}


btRigidBody *CreateSphereShape(const float &Mass, const float &Radius, const btVector3 &Position = btVector3(0, 0, 0))
{
	return CreateRigidBody(Mass, Position, new btSphereShape(Radius));
}


btRigidBody *CreateMeshShape(const float &Mass, const PseudoGameObject *GameObject)
{
	btCollisionShape *shape;


	
	return CreateRigidBody(Mass, 
		btVector3(GameObject->OriginNode->getPosition().x, GameObject->OriginNode->getPosition().y, GameObject->OriginNode->getPosition().z),
		shape);
}


void BulletSample::InitializeRendering(Ogre::SceneManager *SceneManager, Ogre::Camera *Camera, Ogre::DebugDrawer *DebugDrawer)
{
	m_Scene = SceneManager;
	m_DebugDrawer = DebugDrawer;

	Camera->setPosition(0, 10, -20);
	Camera->lookAt(0, 0, 0);

	AddGameObject(m_Scene, "Plane", "Plane.mesh")->OriginNode->setScale(50, 1, 50);
}


void BulletSample::InitializePhysics(void)
{
	btDefaultCollisionConfiguration *conf = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(conf);

	btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	m_PhysicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, conf);
	m_PhysicsWorld->setGravity(btVector3(0, -9.8f, 0));
	m_PhysicsWorld->setDebugDrawer(this);


	// Ground
	m_PhysicsWorld->addRigidBody(CreateBoxShape(0, btVector3(500, 0, 500)));
	
	// Boxes
	m_GameObject = AddGameObject(m_Scene, "Box", "Cube.mesh", Ogre::Vector3(0, 0, 0), Ogre::Vector3::ZERO, Ogre::Vector3(1, 0, 1));
	m_RigidBody = CreateMeshShape(10, m_GameObject);





	//PseudoGameObject *gameObject = NULL;
	//btRigidBody *rigidBody = NULL;
	//for (unsigned int x = 0; x < 4; x++)
	//	for (unsigned int y = 0; y < 4; y++)
	//		for (unsigned int z = 0; z < 4; z++)
	//		{
	//			const float yValue = (y * 1.2f) + 10;

	//			gameObject = AddGameObject(m_Scene, "Sphere" + Ogre::StringConverter::toString(x) + Ogre::StringConverter::toString(y) + Ogre::StringConverter::toString(z), "Sphere.mesh", Ogre::Vector3(x, yValue, z));
	//			//sceneNode->setScale(Ogre::Vector3(2));
	//			m_GameObjects.push_back(gameObject);

	//			rigidBody = CreateSphereShape(10, .5f, btVector3(x, yValue, z));
	//			m_RigidBodies.push_back(rigidBody);
	//			m_PhysicsWorld->addRigidBody(rigidBody);
	//		}

	//	gameObject = AddGameObject(m_Scene, "Box", "Cube.mesh", Ogre::Vector3(0, 0, 0), Ogre::Vector3::ZERO, Ogre::Vector3(1, 0, 1)));
	//	gameObject->OriginNode->setScale(Ogre::Vector3(2));

	//	rigidBody = CreateBoxShape(0, btVector3(2, 2, 2), btVector3(2, 1, 2));
	//	m_PhysicsWorld->addRigidBody(rigidBody);
}


void BulletSample::Update(const unsigned long ElapsedTime)
{
	m_PhysicsWorld->stepSimulation(ElapsedTime / 1000.f);
	//m_PhysicsWorld->debugDrawWorld();

	m_GameObject->OriginNode->setPosition(
		m_RigidBody->getWorldTransform().getOrigin().x(),
		m_RigidBody->getWorldTransform().getOrigin().y(),
		m_RigidBody->getWorldTransform().getOrigin().z());

	m_GameObject->OriginNode->setOrientation(
		m_RigidBody->getWorldTransform().getRotation().w(),
		m_RigidBody->getWorldTransform().getRotation().x(),
		m_RigidBody->getWorldTransform().getRotation().y(),
		m_RigidBody->getWorldTransform().getRotation().z());

	//Ogre::SceneNode *node = NULL;
	//btRigidBody *body = NULL;
	//for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	//{
	//	node = m_GameObjects[i]->OriginNode;
	//	body = m_RigidBodies[i];

	//	node->setPosition(
	//		body->getWorldTransform().getOrigin().x(),
	//		body->getWorldTransform().getOrigin().y(),
	//		body->getWorldTransform().getOrigin().z());

	//	node->setOrientation(
	//		body->getWorldTransform().getRotation().w(),
	//		body->getWorldTransform().getRotation().x(),
	//		body->getWorldTransform().getRotation().y(),
	//		body->getWorldTransform().getRotation().z());
	//}
}


void BulletSample::drawLine(const btVector3 &From, const btVector3 &To, const btVector3 &Color)
{
	m_DebugDrawer->drawLine(
		Ogre::Vector3(From.x(), From.y(), From.z()),
		Ogre::Vector3(To.x(), To.y(), To.z()),
		Ogre::ColourValue(Color.x(), Color.y(), Color.z()));
}