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
#pragma once

#include "StdAfxOGRE.h"
#include "Common.h"

BEGIN_NAMESPACE
class MaterialGenerator
{
public:
	MaterialGenerator(const Ogre::String &BaseName);
	//~MaterialGenerator(void);

	virtual const Ogre::MaterialPtr &GetMaterial(unsigned int Settings);

protected:
	Ogre::String m_BaseName;

	typedef std::map<unsigned int, Ogre::MaterialPtr> MaterialMap;

	MaterialMap m_Materials;
};
END_NAMESPACE