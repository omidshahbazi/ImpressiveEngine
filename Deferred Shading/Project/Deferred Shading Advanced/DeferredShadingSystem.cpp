#include "DeferredShadingSystem.h"
#include "GBufferSchemeListener.h"
#include "NoGBufferSchemeListener.h"
#include "DeferredLightingPass.h"
#include "CommonParameters.h"


const char *DeferredShadingSystem::SCHEME_GBUFFER = "GBufferScheme";
const char *DeferredShadingSystem::SCHEME_NO_GBUFFER = "NoGBufferScheme";

const char *CUSTOM_PASS_DEFERRED_LIGHTING("DeferredLighting");


DeferredShadingSystem::DeferredShadingSystem(void)
{
	Initialize();

	m_AmbientLight = new AmbientLight(this);
}


void DeferredShadingSystem::AddViewport(Ogre::Viewport *Viewport)
{
	Ogre::CompositorManager &manager = Ogre::CompositorManager::getSingleton();

	CompositorsHolder *ch = new CompositorsHolder;

	ch->GBuffer = manager.addCompositor(Viewport, "GBufferCompositor");;

	ch->Instances[RM_LIT] = manager.addCompositor(Viewport, "ShowLitCompositor");
	ch->Instances[RM_DEPTH] = manager.addCompositor(Viewport, "ShowDepthCompositor");
	ch->Instances[RM_SPECULAR] = manager.addCompositor(Viewport, "ShowSpecularCompositor");
	ch->Instances[RM_NORMAL] = manager.addCompositor(Viewport, "ShowNormalCompositor");
	ch->Instances[RM_ALBEDO] = manager.addCompositor(Viewport, "ShowAlbedoCompositor");
	
	m_Compositors[Viewport] = ch;

	SetRenderMode(RM_LIT);
}


//DeferredShadingSystem::~DeferredShadingSystem(void)
//{
//}


void DeferredShadingSystem::SetRenderMode(const RenderMode &Mode)
{
	ViewportCompositorsMap::iterator it;

	CompositorsHolder *ch = NULL;
	for (it = m_Compositors.begin(); it != m_Compositors.end(); it++)
	{
		ch = it->second;

		ch->GBuffer->setEnabled(Mode != RM_SCENE);

		for (unsigned short i = 0; i < RM_COUNT; i++)
			if (i == Mode)
				ch->Instances[i]->setEnabled(true);
			else
				ch->Instances[i]->setEnabled(false);
	}
}


void DeferredShadingSystem::RenderLights(void)
{
	ViewportCompositorsMap::iterator it;

	CompositorsHolder *ch = NULL;
	for (it = m_Compositors.begin(); it != m_Compositors.end(); it++)
	{
		CommonParameters::UpdateParameters(it->first->getCamera());

		m_AmbientLight->Render();

		const Ogre::LightList &lights = it->first->getCamera()->getSceneManager()->_getLightsAffectingFrustum();

		LightsMap::iterator it;
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			it = m_DynamicLights.find(lights[i]);
		
			//if (it != m_DynamicLights.end())
				it->second->Render();
		}
	}
}


DSLight *DeferredShadingSystem::CreateLight(const DSLight::LightType &Type, const Ogre::Vector3 &Position)
{
	DSLight *light = new DSLight(this);

	light->SetType(Type);
	light->SetPosition(Position);

	m_DynamicLights[light->_GetOgreLight()] = light;

	return light;
}


void DeferredShadingSystem::Initialize(void)
{
	Ogre::MaterialManager::getSingleton().addListener(new GBufferSchemeListener, SCHEME_GBUFFER);
	Ogre::MaterialManager::getSingleton().addListener(new NoGBufferSchemeListener, SCHEME_NO_GBUFFER);

	Ogre::CompositorManager::getSingleton().registerCustomCompositionPass(CUSTOM_PASS_DEFERRED_LIGHTING, new DeferredLightingPass(this));
}