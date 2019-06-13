#pragma once

#include <Ogre.h>
#include "OIS.h"
#include "PseudoGameObject.h"
#include "OgreDebugDrawer.h"


class IPhysicsSample
{
protected:
	OIS::Keyboard *m_Keyboard;
	OIS::Mouse *m_Mouse;

public:
	virtual void InitializeRendering(Ogre::SceneManager *SceneManager, Ogre::Camera *Camera, Ogre::DebugDrawer *DebugDrawer) = 0;
	virtual void InitializePhysics(void) = 0;

	virtual void Update(const unsigned long ElapsedTime) = 0;

	virtual void HandleInput(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse) {}

protected:
	static PseudoGameObject *AddGameObject(Ogre::SceneManager *SceneManager, const Ogre::String &Name, const Ogre::String &MeshName, const Ogre::Vector3 &Position = Ogre::Vector3::ZERO, const Ogre::Vector3 &Rotation = Ogre::Vector3::ZERO, const Ogre::Vector3 &OriginOffset = Ogre::Vector3::ZERO)
	{
		// Just for pivot-offset use
		Ogre::SceneNode *parent = SceneManager->getRootSceneNode()->createChildSceneNode(Name + "_Parent", Position);

		//Ogre::SceneNode *sceneNode = parent->createChildSceneNode(Name, OriginOffset);


		Ogre::Entity *entity = SceneManager->createEntity(MeshName);
		//Ogre::Entity *entity2 = SceneManager->createEntity(MeshName);

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(Name + "_Material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		int r = RandomColorValue();
		int g = RandomColorValue();
		int b = RandomColorValue();
		Ogre::ColourValue color(r / 255.f, g / 255.f, b / 255.f);

		material->setAmbient(color);
		material->setColourWriteEnabled(true);

		entity->setMaterial(material);

		//sceneNode->attachObject(entity);
		parent->attachObject(entity);

		parent->pitch(Ogre::Degree(Rotation.x));
		parent->yaw(Ogre::Degree(Rotation.y));
		parent->roll(Ogre::Degree(Rotation.z));

		return new PseudoGameObject(parent, entity);
	}

private:
	static int RandomColorValue(void)
	{
		int value = rand();

		if (value > 255)
			return RandomColorValue();

		return value;
	}
};