#include "DeferredLightingPass.h"
#include "DeferredShadingSystem.h"


class DeferredLightingRenderOperation : public Ogre::CompositorInstance::RenderSystemOperation
{
public:
	DeferredLightingRenderOperation(DeferredShadingSystem *DeferredShadingSystem) :
		m_DeferredShadingSystem(DeferredShadingSystem)
	{
	}

	void execute(Ogre::SceneManager *sm, Ogre::RenderSystem *rs)
	{
		m_DeferredShadingSystem->RenderLights();
	}

private:
	DeferredShadingSystem *m_DeferredShadingSystem;
};



DeferredLightingPass::DeferredLightingPass(DeferredShadingSystem *DeferredShadingSystem) :
	m_DeferredShadingSystem(DeferredShadingSystem)
{
}


Ogre::CompositorInstance::RenderSystemOperation *DeferredLightingPass::createOperation(Ogre::CompositorInstance *instance, const Ogre::CompositionPass *pass)
{
	return new DeferredLightingRenderOperation(m_DeferredShadingSystem);
}