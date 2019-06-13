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
#include "MaterialGenerator.h"


BEGIN_NAMESPACE
MaterialGenerator::MaterialGenerator(const Ogre::String &BaseName) :
	m_BaseName(BaseName)
{
}


//MaterialGenerator::~MaterialGenerator(void)
//{
//}


const Ogre::MaterialPtr &MaterialGenerator::GetMaterial(unsigned int Settings)
{
	MaterialMap::iterator it = m_Materials.find(Settings);

	if (it == m_Materials.end())
	{
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(m_BaseName + "-" + Ogre::StringConverter::toString(Settings), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		m_Materials[Settings] = material;

		return m_Materials[Settings];
	}

	return it->second;
}
END_NAMESPACE
