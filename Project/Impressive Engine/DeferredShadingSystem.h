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
#pragma once

#include "StdAfxOGRE.h"
#include "LightMaterialGenerator.h"
#include "AmbientLight.h"
#include "Light.h"

BEGIN_NAMESPACE
class Scene;

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
	DeferredShadingSystem(Scene *Holder);
	~DeferredShadingSystem(void);

	void Update(void);

	void OnViewportChanges(Ogre::Viewport *Viewport);

	void SetRenderMode(const RenderMode &Mode);
	const RenderMode &GetRenderMode(void) const
	{
		return m_RenderMode;
	}

	void RenderLights(void);

	Light *CreateLight(const Light::LightType &Type = Light::LT_DIRECTIONAL, const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);
	void DestroyLight(Light *Light);

	Scene *GetScene(void) const
	{
		return m_Holder;
	}

	const Ogre::Viewport *GetViewport(void) const
	{
		return m_Viewport;
	}

	LightMaterialGenerator *GetLightMaterialGenerator(void)
	{
		return &m_LightMaterialGenerator;
	}

private:
	void CleanUp(void);
	void Initialize(void);
	
private:
	Scene *m_Holder;

	Ogre::Viewport *m_Viewport;

	RenderMode m_RenderMode;

	Ogre::CompositorInstance *m_GBuffer;

	Ogre::CompositorInstance *m_Instance[RM_COUNT];

	Ogre::CompositionPass *m_LitClearPass;

	LightMaterialGenerator m_LightMaterialGenerator;

	AmbientLight *m_AmbientLight;

	typedef std::map<Ogre::Light*, Light*> LightsMap;

	LightsMap m_DynamicLights;
};
END_NAMESPACE