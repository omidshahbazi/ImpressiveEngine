#pragma once

#include <Ogre.h>
#include <OgreCustomCompositionPass.h>

class DeferredShadingSystem;

class DeferredLightCompositionPass : public Ogre::CustomCompositionPass
{
public:
	DeferredLightCompositionPass(DeferredShadingSystem *DeferredShadingSystem);

	Ogre::CompositorInstance::RenderSystemOperation *createOperation(Ogre::CompositorInstance *instance, const Ogre::CompositionPass *pass);

private:
	DeferredShadingSystem *m_DeferredShadingSystem;
};

