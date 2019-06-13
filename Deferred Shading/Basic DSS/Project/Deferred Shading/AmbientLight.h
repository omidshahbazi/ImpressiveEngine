#pragma once

#include "LightBase.h"

class AmbientLight : public LightBase
{
public:
	AmbientLight(void);
	//~AmbientLight(void);

	void Update(Ogre::Camera *Camera/*, const Ogre::LightList *LightList = NULL*/);

	float getSquaredViewDepth(const Ogre::Camera *Camera) const;
	const Ogre::MaterialPtr &getMaterial(void) const;
	void getWorldTransforms(Ogre::Matrix4 *xform) const;

private:
	void UpdateParameters(const Ogre::Camera *Camera);

private:
	Ogre::MaterialPtr m_Material;
};

