#pragma once

#include <Ogre.h>
#include <OgreCustomCompositionPass.h>

class DeferredShadingSystem;

class DeferredLightingPass : public Ogre::CustomCompositionPass
{
public:
	DeferredLightingPass(DeferredShadingSystem *DeferredShadingSystem);

private:
	Ogre::CompositorInstance::RenderSystemOperation *createOperation(Ogre::CompositorInstance *instance, const Ogre::CompositionPass *pass);

private:
	DeferredShadingSystem *m_DeferredShadingSystem;
};

