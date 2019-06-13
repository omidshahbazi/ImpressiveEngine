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
#include "AmbientLight.h"
#include "DSLightRenderable.h"


BEGIN_NAMESPACE
AmbientLight::AmbientLight(DeferredShadingSystem *DeferredShadingSystem)
{
	m_Light = new DSLightRenderable(DeferredShadingSystem, DSLightRenderable::LT_AMBIENT, 15000.f);
}


AmbientLight::~AmbientLight(void)
{
	delete m_Light;
}


void AmbientLight::Render(void)
{
	m_Light->Render();
}
END_NAMESPACE