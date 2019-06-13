
#include "LightBase.h"


LightBase::LightBase(const float &Radius) :
	m_Radius(Radius),
	m_IsDirty(true)
{

}


float LightBase::getBoundingRadius(void) const
{
	return m_Radius;
}


void LightBase::Update(Ogre::Camera *Camera, const Ogre::LightList *LightList)
{
	InjectTechnique(Camera->getSceneManager(), LightList);
}


void LightBase::InjectTechnique(Ogre::SceneManager *SceneManager, const Ogre::LightList *LightList)
{
	Ogre::Technique *tech = getMaterial()->getBestTechnique();

	Ogre::Pass *pass = NULL;
	for (unsigned short i = 0; i < tech->getNumPasses(); i++)
	{
		pass = tech->getPass(i);

        if (LightList)
			SceneManager->_injectRenderWithPass(pass, this, false, false, LightList);
		else
			SceneManager->_injectRenderWithPass(pass, this, false);
	}
}