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
#include "Light.h"
#include "DSLightRenderable.h"
#include "DeferredShadingSystem.h"
#include "Scene.h"


BEGIN_NAMESPACE
Light::Light(DeferredShadingSystem *DeferredShadingSystem) :
	m_Holder(DeferredShadingSystem)
{
	m_OgreLight = m_Holder->GetScene()->GetSceneManager()->createLight();

	m_Light = new DSLightRenderable(m_Holder, DSLightRenderable::LT_POINT, 0, m_OgreLight);
}


Light::~Light(void)
{
	delete m_Light;

	m_Holder->GetScene()->GetSceneManager()->destroyLight(m_OgreLight);
}


void Light::Render(void)
{
	m_Light->Render();
}


void Light::SetType(const LightType &Type)
{
	switch (Type)
	{
	case LT_POINT:
		{
			m_Light->SetType(DSLightRenderable::LT_POINT);
		} break;

	case LT_DIRECTIONAL:
		{
			m_Light->SetType(DSLightRenderable::LT_DIRECTIONAL);
		} break;

	case LT_SPOTLIGHT:
		{
			m_Light->SetType(DSLightRenderable::LT_SPOTLIGHT);
		} break;
	}
}


const Light::LightType Light::GetType(void) const
{
	switch (m_Light->GetType())
	{
	case DSLightRenderable::LT_POINT: return LT_POINT;
	case DSLightRenderable::LT_DIRECTIONAL: return LT_DIRECTIONAL;
	case DSLightRenderable::LT_SPOTLIGHT: return LT_SPOTLIGHT;
	}

	return LT_DIRECTIONAL;
}


void Light::SetSpecularColor(const float &Red, const float &Green, const float &Blue)
{
	m_OgreLight->setSpecularColour(Red, Green, Blue);

	m_Light->SetAsDirty();
}


void Light::SetAttenuation(const float &Range, const float &Constant, const float &Linear, const float &Quadratic)
{
	if (GetType() == LT_DIRECTIONAL)
		return;

	m_OgreLight->setAttenuation(Range, Constant, Linear, Quadratic);

	m_Light->SetAsDirty();
}
END_NAMESPACE