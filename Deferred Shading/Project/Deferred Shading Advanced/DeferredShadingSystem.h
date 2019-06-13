#pragma once

#include <Ogre.h>
#include "LightMaterialGenerator.h"
#include "AmbientLight.h"
#include "DSLight.h"


class DeferredShadingSystem
{
public:
	enum RenderMode
	{
		RM_SCENE = -1,
		RM_LIT,
		RM_DEPTH,
		RM_SPECULAR,
		RM_NORMAL,
		RM_ALBEDO,
		RM_COUNT
	};

public:
	DeferredShadingSystem(void);
	//~DeferredShadingSystem(void);

	void AddViewport(Ogre::Viewport *Viewport);

	void SetRenderMode(const RenderMode &Mode);

	void RenderLights(void);

	DSLight *CreateLight(const DSLight::LightType &Type = DSLight::LT_DIRECTIONAL, const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);

	//const Ogre::Viewport *GetViewport(void) const
	//{
	//	return m_Viewport;
	//}

	//Ogre::SceneManager *GetSceneManager(void) const
	//{
	//	return m_Viewport->getCamera()->getSceneManager();
	//}

	LightMaterialGenerator *GetLightMaterialGenerator(void)
	{
		return &m_LightMaterialGenerator;
	}

private:
	void Initialize(void);
	
public:
	static const char *SCHEME_GBUFFER;
	static const char *SCHEME_NO_GBUFFER;
	
private:
	struct CompositorsHolder
	{
		Ogre::CompositorInstance *GBuffer;
		Ogre::CompositorInstance *Instances[RM_COUNT];
	};

	typedef std::map<Ogre::Viewport*, CompositorsHolder*> ViewportCompositorsMap;

	ViewportCompositorsMap m_Compositors;

	LightMaterialGenerator m_LightMaterialGenerator;

	AmbientLight *m_AmbientLight;

	typedef std::map<Ogre::Light*, DSLight*> LightsMap;

	LightsMap m_DynamicLights;
};
