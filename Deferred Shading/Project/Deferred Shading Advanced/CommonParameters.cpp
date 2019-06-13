#include "CommonParameters.h"

Ogre::Vector3 CommonParameters::m_FarCorner = Ogre::Vector3::ZERO;

void CommonParameters::UpdateParameters(const Ogre::Camera *Camera)
{
	m_FarCorner = Camera->getViewMatrix(true) * Camera->getWorldSpaceCorners()[4];
}