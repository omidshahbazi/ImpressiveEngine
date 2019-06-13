#include "DeferredLightCompositionPass.h"
#include "DeferredShadingSystem.h"


class DeferredLightRenderOperation : public Ogre::CompositorInstance::RenderSystemOperation
{
public:
	DeferredLightRenderOperation(DeferredShadingSystem *DeferredShadingSystem) :
		m_DeferredShadingSystem(DeferredShadingSystem)
	{
	}

	void execute(Ogre::SceneManager *sm, Ogre::RenderSystem *rs)
	{
		m_DeferredShadingSystem->UpdateLights();
	}

private:
	DeferredShadingSystem *m_DeferredShadingSystem;
};


DeferredLightCompositionPass::DeferredLightCompositionPass(DeferredShadingSystem *DeferredShadingSystem) :
	m_DeferredShadingSystem(DeferredShadingSystem)
{
}


Ogre::CompositorInstance::RenderSystemOperation *DeferredLightCompositionPass::createOperation(Ogre::CompositorInstance *instance, const Ogre::CompositionPass *pass)
{
	return OGRE_NEW DeferredLightRenderOperation(m_DeferredShadingSystem);
}