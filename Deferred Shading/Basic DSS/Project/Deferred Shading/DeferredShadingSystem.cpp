#include "DeferredShadingSystem.h"
#include "GBufferSchemeListener.h"
#include "NullSchemeListener.h"
#include "SSAOLogic.h"
#include "DeferredLightCompositionPass.h"


const char *DeferredShadingSystem::SCHEME_GBUFFER = "GBuffer";
const char *DeferredShadingSystem::SCHEME_NO_GBUFFER = "NoGBuffer";


DeferredShadingSystem::DeferredShadingSystem(Ogre::Viewport *Viewport) :
	m_RenderMode(RM_LIT),
	m_Viewport(Viewport)
{
	Initialize();
	
	SetEnabled(true);

	m_AmbientLight = new AmbientLight;
}


DeferredShadingSystem::~DeferredShadingSystem(void)
{
}


void DeferredShadingSystem::SetEnabled(const bool &Enabled)
{
	m_GBuffer->setEnabled(Enabled);
	
	SetRenderMode(m_RenderMode);
}


void DeferredShadingSystem::SetEnabledSSAO(const bool &Enabled)
{
	if (m_SSAO->getEnabled() == Enabled || !m_GBuffer->getEnabled())
		return;

	m_SSAO->setEnabled(Enabled);
}


void DeferredShadingSystem::SetRenderMode(const RenderMode &Mode)
{
	if (m_RenderMode == Mode && m_PPE[m_RenderMode]->getEnabled() == m_GBuffer->getEnabled())
		return;

	m_RenderMode = Mode;

	for (unsigned short i = 0; i < RM_COUNT; i++)
		if (i == m_RenderMode)
			m_PPE[i]->setEnabled(m_GBuffer->getEnabled());
		else
			m_PPE[i]->setEnabled(false);

	SetEnabledSSAO(m_PPE[m_RenderMode]->getEnabled() ? (m_SSAO->getEnabled() ? Mode == RM_LIT : false) : false);
}


DirectionalLight *DeferredShadingSystem::CreateDirectionalLight(const Ogre::Vector3 &Position)
{
	DirectionalLight *l = new DirectionalLight(m_Viewport->getCamera()->getSceneManager(), &m_LightMaterialGenerator);

	l->GetOgreLight()->setPosition(Position);

	m_LightsMap[l->GetOgreLight()] = l;

	return l;
}


PointLight *DeferredShadingSystem::CreatePointLight(const Ogre::Vector3 &Position)
{
	PointLight *l = new PointLight(m_Viewport->getCamera()->getSceneManager(), &m_LightMaterialGenerator);

	l->GetOgreLight()->setPosition(Position);

	m_LightsMap[l->GetOgreLight()] = l;

	return l;
}


SpotLight *DeferredShadingSystem::CreateSpotLight(const Ogre::Vector3 &Position)
{
	SpotLight *l = new SpotLight(m_Viewport->getCamera()->getSceneManager(), &m_LightMaterialGenerator);

	l->GetOgreLight()->setPosition(Position);

	m_LightsMap[l->GetOgreLight()] = l;

	return l;
}


void DeferredShadingSystem::UpdateLights(void)
{
	Ogre::Camera *camera = m_Viewport->getCamera();

	m_AmbientLight->Update(camera);

	const Ogre::LightList &ll = camera->getSceneManager()->_getLightsAffectingFrustum();

	for (unsigned int i = 0; i < ll.size(); i++)
		m_LightsMap[ll[i]]->Update(camera);
}


void DeferredShadingSystem::Initialize(void)
{
	Ogre::MaterialManager::getSingleton().addListener(new GBufferSchemeListener, SCHEME_GBUFFER);
	Ogre::MaterialManager::getSingleton().addListener(new NullSchemeListener, SCHEME_NO_GBUFFER);

	Ogre::CompositorManager &manager = Ogre::CompositorManager::getSingleton();
	
	manager.registerCompositorLogic("SSAOLogic", new SSAOLogic);
	manager.registerCustomCompositionPass("DeferredLight", new DeferredLightCompositionPass(this));
	
	m_GBuffer = manager.addCompositor(m_Viewport, "DeferredShading/GBuffer");

	m_PPE[RM_DEPTH_SPECULAR] = manager.addCompositor(m_Viewport, "DeferredShading/ShowDepthSpecular");
	m_PPE[RM_NORMALS] = manager.addCompositor(m_Viewport, "DeferredShading/ShowNormals");
	m_PPE[RM_COLOR] = manager.addCompositor(m_Viewport, "DeferredShading/ShowColour");
	m_PPE[RM_LIT] = manager.addCompositor(m_Viewport, "DeferredShading/ShowLit");
	
	m_SSAO = manager.addCompositor(m_Viewport, "DeferredShading/SSAO");
}