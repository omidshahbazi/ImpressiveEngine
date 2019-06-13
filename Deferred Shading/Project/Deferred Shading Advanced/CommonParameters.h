#pragma once

#include <Ogre.h>

class CommonParameters
{
public:
	static void UpdateParameters(const Ogre::Camera *Camera);

	static Ogre::Vector3 &GetFarCorner(void)
	{
		return m_FarCorner;
	}

private:
	static Ogre::Vector3 m_FarCorner;
};