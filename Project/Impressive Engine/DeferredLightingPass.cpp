/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#include "DeferredLightingPass.h"
#include "DeferredShadingSystem.h"


BEGIN_NAMESPACE
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
END_NAMESPACE