#pragma once

#include "DynamicLight.h"

class SpotLight : public DynamicLight
{
public:
	SpotLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator);
	//~SpotLight(void);

	const LightType &GetLightType(void) const
	{
		return LT_SPOTLIGHT;
	}

	void getWorldTransforms(Ogre::Matrix4 *xform) const;

private:
	const float GetOuterRadius(void)
	{
		return m_Light->getAttenuationRange();
	}

	bool IsCameraInside(const Ogre::Camera *Camera);

	void RebuildGeometry(const float &Radius);
};