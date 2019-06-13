#pragma once

#include <Ogre.h>

class LightBase : public Ogre::SimpleRenderable
{
public:
	LightBase(const float &Radius = 0.f);

	float getBoundingRadius(void) const;

	virtual void Update(Ogre::Camera *Camera, const Ogre::LightList *LightList = NULL);

private:
	void InjectTechnique(Ogre::SceneManager *SceneManager, const Ogre::LightList *LightList);

protected:
	float m_Radius;

	bool m_IsDirty;
};

