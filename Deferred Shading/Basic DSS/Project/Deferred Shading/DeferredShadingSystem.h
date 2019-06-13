#pragma once

#include "LightMaterialGenerator.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


class DeferredShadingSystem
{
public:
	enum RenderMode
	{
		RM_DEPTH_SPECULAR = 0,
		RM_NORMALS,
		RM_COLOR,
		RM_LIT,
		RM_COUNT
	};

public:
	DeferredShadingSystem(Ogre::Viewport *Viewport);
	~DeferredShadingSystem(void);

	void SetEnabled(const bool &Enabled);

	void SetEnabledSSAO(const bool &Enabled);

	void SetRenderMode(const RenderMode &Mode);
	
	DirectionalLight *CreateDirectionalLight(const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);
	PointLight *CreatePointLight(const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);
	SpotLight *CreateSpotLight(const Ogre::Vector3 &Position = Ogre::Vector3::ZERO);

	void UpdateLights(void);

private:
	void Initialize(void);

public:
	static const char *SCHEME_GBUFFER;
	static const char *SCHEME_NO_GBUFFER;

private:
	typedef std::map<Ogre::Light*, DynamicLight*> LightMap;

	RenderMode m_RenderMode;

	Ogre::Viewport *m_Viewport;

	Ogre::CompositorInstance *m_GBuffer;
	Ogre::CompositorInstance *m_PPE[RM_COUNT];
	Ogre::CompositorInstance *m_SSAO;
	
	LightMaterialGenerator m_LightMaterialGenerator;

	AmbientLight *m_AmbientLight;

	LightMap m_LightsMap;
};

