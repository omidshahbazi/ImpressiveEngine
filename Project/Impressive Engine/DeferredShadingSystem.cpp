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
#include "DeferredShadingSystem.h"
#include "Scene.h"
#include "GBufferSchemeListener.h"
#include "NoGBufferSchemeListener.h"
#include "DeferredLightingPass.h"
#include "DSTerrainMaterialGenerator.h"
#include "DSCommon.h"
#include "Utility.h"


BEGIN_NAMESPACE
DeferredShadingSystem::DeferredShadingSystem(Scene *Holder) :
	m_Holder(Holder),
	m_Viewport(NULL),
	m_GBuffer(NULL),
	m_RenderMode(RM_LIT)
{
	m_AmbientLight = new AmbientLight(this);

	Ogre::MaterialManager::getSingleton().addListener(new GBufferSchemeListener, DSCommon::SCHEME_GBUFFER);
	Ogre::MaterialManager::getSingleton().addListener(new NoGBufferSchemeListener, DSCommon::SCHEME_NO_GBUFFER);
	Ogre::CompositorManager::getSingleton().registerCustomCompositionPass(DSCommon::DEFERRED_LIGHTING_PASS, new DeferredLightingPass(this));

	Ogre::TerrainGlobalOptions::getSingleton().setDefaultMaterialGenerator(Ogre::TerrainMaterialGeneratorPtr(new DSTerrainMaterialGenerator));
}


DeferredShadingSystem::~DeferredShadingSystem(void)
{
	delete m_AmbientLight;

	CleanUp();
}


void DeferredShadingSystem::Update(void)
{
	if (m_LitClearPass->getClearColour() != m_Viewport->getBackgroundColour())
		m_LitClearPass->setClearColour(m_Viewport->getBackgroundColour());
}


void DeferredShadingSystem::OnViewportChanges(Ogre::Viewport *Viewport)
{
	m_Viewport = Viewport;

	if (m_GBuffer)
		CleanUp();

	Initialize();
}


void DeferredShadingSystem::SetRenderMode(const RenderMode &Mode)
{
	m_RenderMode = Mode;

	if (!m_GBuffer) // in Scene::Deserialize, when we want to call this function, it's before Initialize or OnViewportChanges
		return;

	m_GBuffer->setEnabled(m_RenderMode != RM_SCENE);

	for (unsigned short i = 0; i < RM_COUNT; i++)
		if (i == m_RenderMode)
			m_Instance[i]->setEnabled(true);
		else
			m_Instance[i]->setEnabled(false);

	m_LitClearPass = m_Instance[RM_LIT]->getTechnique()->getOutputTargetPass()->getPass(0);
}


void DeferredShadingSystem::RenderLights(void)
{
	DSCommon::UpdateParameters(m_Viewport->getCamera());

	m_AmbientLight->Render();

	const Ogre::LightList &lights = m_Holder->GetSceneManager()->_getLightsAffectingFrustum();

	LightsMap::iterator it;
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		it = m_DynamicLights.find(lights[i]);
		
		//if (it != m_DynamicLights.end())
			it->second->Render();
	}
}


Light *DeferredShadingSystem::CreateLight(const Light::LightType &Type, const Ogre::Vector3 &Position)
{
	Light *light = new Light(this);

	light->SetType(Type);
	light->SetPosition(Position);

	m_DynamicLights[light->_GetOgreLight()] = light;

	return light;
}


void DeferredShadingSystem::DestroyLight(Light *Light)
{
	m_DynamicLights.erase(Light->_GetOgreLight());

	delete Light;
}


void DeferredShadingSystem::CleanUp(void)
{
	//SetRenderMode(RM_SCENE);

	Ogre::CompositorManager &manager = Ogre::CompositorManager::getSingleton();

	manager.removeCompositor(m_Viewport, DSCommon::GBUFFER_NAME);

	manager.removeCompositor(m_Viewport, DSCommon::SHOW_LIT_NAME);
	manager.removeCompositor(m_Viewport, DSCommon::SHOW_DEPTH_NAME);
	manager.removeCompositor(m_Viewport, DSCommon::SHOW_SPECULAR_NAME);
	manager.removeCompositor(m_Viewport, DSCommon::SHOW_NORMAL_NAME);
	manager.removeCompositor(m_Viewport, DSCommon::SHOW_ALBEDO_NAME);
}


void DeferredShadingSystem::Initialize(void)
{
	Ogre::CompositorManager &manager = Ogre::CompositorManager::getSingleton();

	m_GBuffer = manager.addCompositor(m_Viewport, DSCommon::GBUFFER_NAME);

	m_Instance[RM_LIT] = manager.addCompositor(m_Viewport, DSCommon::SHOW_LIT_NAME);
	m_Instance[RM_DEPTH] = manager.addCompositor(m_Viewport, DSCommon::SHOW_DEPTH_NAME);
	m_Instance[RM_SPECULAR] = manager.addCompositor(m_Viewport, DSCommon::SHOW_SPECULAR_NAME);
	m_Instance[RM_NORMAL] = manager.addCompositor(m_Viewport, DSCommon::SHOW_NORMAL_NAME);
	m_Instance[RM_ALBEDO] = manager.addCompositor(m_Viewport, DSCommon::SHOW_ALBEDO_NAME);

	SetRenderMode(m_RenderMode);
}
END_NAMESPACE