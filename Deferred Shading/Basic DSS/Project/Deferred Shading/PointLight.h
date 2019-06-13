#pragma once

#include "DynamicLight.h"

class PointLight : public DynamicLight
{
public:
	PointLight(Ogre::SceneManager *SceneManager, DSSMaterialGenerator *DSSMaterialGenerator);
	//~PointLight(void);

	const LightType &GetLightType(void) const
	{
		return LT_POINT;
	}

private:
	const float GetOuterRadius(void);

	bool IsCameraInside(const Ogre::Camera *Camera);

	void RebuildGeometry(const float &Radius);
};

