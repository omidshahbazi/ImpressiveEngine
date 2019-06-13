#pragma once

#include <Ogre.h>
#include "GBufferMaterialGenerator.h"

class NoGBufferSchemeListener : public Ogre::MaterialManager::Listener
{
private:
	Ogre::Technique *handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName, Ogre::Material *originalMaterial, unsigned short lodIndex, const Ogre::Renderable *rend);
};