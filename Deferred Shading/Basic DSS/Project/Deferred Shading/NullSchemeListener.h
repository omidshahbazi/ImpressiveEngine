#pragma once

#include <Ogre.h>

class NullSchemeListener : public Ogre::MaterialManager::Listener
{
public:
	Ogre::Technique *handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName, Ogre::Material *originalMaterial, unsigned short lodIndex, const Ogre::Renderable *rend);
};

