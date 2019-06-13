#pragma once

#include "GBufferMaterialGenerator.h"

class GBufferSchemeListener : public Ogre::MaterialManager::Listener
{
public:
	Ogre::Technique *handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName, Ogre::Material *originalMaterial, unsigned short lodIndex, const Ogre::Renderable *rend);

private:
	GBufferMaterialGenerator m_MaterialGenerator;
};