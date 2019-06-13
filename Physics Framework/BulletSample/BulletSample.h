#pragma once

#include <IPlugin.h>
#include <IPhysicsSample.h>
#include <btBulletDynamicsCommon.h>
#include <vector>

class BulletSample : public IPlugin, public IPhysicsSample, public btIDebugDraw
{
public:
	void Install(void) {}
	const char *GetName(void) const
	{
		return "Bullet Sample";
	}
	const char *GetVersion(void) const
	{
		return "1.0.0.0";
	}

	void InitializeRendering(Ogre::SceneManager *SceneManager, Ogre::Camera *Camera, Ogre::DebugDrawer *DebugDrawer);
	void InitializePhysics(void);
	void Update(const unsigned long ElapsedTime);

	void drawLine(const btVector3 &From, const btVector3 &To, const btVector3 &Color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}
	void reportErrorWarning(const char* warningString) {}
	void draw3dText(const btVector3& location, const char* textString) {}
	void setDebugMode(int debugMode) {}
	int getDebugMode() const 
	{ 
		return btIDebugDraw::DBG_DrawWireframe; 
	}

private:
	Ogre::SceneManager *m_Scene;
	Ogre::DebugDrawer *m_DebugDrawer;
	btDiscreteDynamicsWorld *m_PhysicsWorld;

	PseudoGameObject *m_GameObject;
	btRigidBody *m_RigidBody;

	//std::vector<PseudoGameObject*> m_GameObjects;
	//std::vector<btRigidBody*> m_RigidBodies;
};

