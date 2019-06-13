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

#include "Common.h"
#include "StdAfxOGRE.h"
#include <OgreCustomCompositionPass.h>

BEGIN_NAMESPACE
class DeferredShadingSystem;

class DeferredLightingPass : public Ogre::CustomCompositionPass
{
public:
	DeferredLightingPass(DeferredShadingSystem *DeferredShadingSystem);

private:
	Ogre::CompositorInstance::RenderSystemOperation *createOperation(Ogre::CompositorInstance *instance, const Ogre::CompositionPass *pass);

private:
	DeferredShadingSystem *m_DeferredShadingSystem;
};
END_NAMESPACE