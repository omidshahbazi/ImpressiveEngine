#include "NullSchemeListener.h"
#include "DeferredShadingSystem.h"


Ogre::Technique *NullSchemeListener::handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName, Ogre::Material *originalMaterial, unsigned short lodIndex, const Ogre::Renderable *rend)
{
	// Creating a technique so the handler only gets called once per material
	Ogre::Technique *emptyTech = originalMaterial->createTechnique();

	emptyTech->removeAllPasses();
	emptyTech->setSchemeName(DeferredShadingSystem::SCHEME_NO_GBUFFER);

	// For own test only, I want to know, Is schemeName here except "NoGBuffer" ?!
	assert(schemeName == DeferredShadingSystem::SCHEME_NO_GBUFFER);

	return emptyTech;
}
