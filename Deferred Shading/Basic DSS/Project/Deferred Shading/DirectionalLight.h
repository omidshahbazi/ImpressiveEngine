#pragma once

#include "DynamicLight.h"

class DirectionalLight : public DynamicLight
{
public:
	DirectionalLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator);
	//~DirectionalLight(void);

	const LightType &GetLightType(void) const
	{
		return LT_DIRECTIONAL;
	}

private:
	const float GetOuterRadius()
	{
		return m_Light->getAttenuationRange();
	}

	bool IsCameraInside(const Ogre::Camera *Camera);

	void RebuildGeometry(const float &Radius);
};

