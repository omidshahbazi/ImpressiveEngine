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
#include "GBufferMaterialGenerator.h"

BEGIN_NAMESPACE
class GBufferSchemeListener : public Ogre::MaterialManager::Listener
{
private:
	Ogre::Technique *handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String &schemeName, Ogre::Material *originalMaterial, unsigned short lodIndex, const Ogre::Renderable *rend);

private:
	GBufferMaterialGenerator m_GBufferMaterialGenerator;
};
END_NAMESPACE