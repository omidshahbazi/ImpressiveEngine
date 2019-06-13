#pragma once

#include "PxPhysicsAPI.h"
#include "Ogre.h"
#include "OgreDebugDrawer.h"
#include "PseudoGameObject.h"

using namespace physx;
using namespace Ogre;


class IeDebugDrawer
{
private:
	Ogre::DebugDrawer *m_DebugDrawer;

	void DrawWireBox(PxShape *shape, PseudoGameObject *gameObject, ColourValue &color);
	void DrawWireBox(PxBounds3 &bounds, ColourValue &color);

public:
	IeDebugDrawer(Ogre::DebugDrawer *DebugDrawer);
	~IeDebugDrawer(void);

	void IeDebugDrawer::DrawLine(const Vector3 &From, const Vector3 &To, const ColourValue &Color);

	void DrawWireShape(PxShape *shape, PseudoGameObject *gameObject, ColourValue color);
};

