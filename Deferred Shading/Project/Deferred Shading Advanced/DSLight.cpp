#include "DSLight.h"
#include "DSLightRenderable.h"
#include "DeferredShadingSystem.h"


DSLight::DSLight(DeferredShadingSystem *DeferredShadingSystem)
{
	m_OgreLight = DeferredShadingSystem->GetSceneManager()->createLight();

	m_Light = new DSLightRenderable(DeferredShadingSystem, DSLightRenderable::LT_POINT, 0, m_OgreLight);
}


DSLight::~DSLight(void)
{
	delete m_Light;
}


void DSLight::Render(void)
{
	m_Light->Render();
}


void DSLight::SetType(const LightType &Type)
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


const DSLight::LightType &DSLight::GetType(void) const
{
	switch (m_Light->GetType())
	{
	case DSLightRenderable::LT_POINT: return LT_POINT;
	case DSLightRenderable::LT_DIRECTIONAL: return LT_DIRECTIONAL;
	case DSLightRenderable::LT_SPOTLIGHT: return LT_SPOTLIGHT;
	}

	return LT_DIRECTIONAL;
}


void DSLight::SetSpecularColor(const float &Red, const float &Green, const float &Blue)
{
	m_OgreLight->setSpecularColour(Red, Green, Blue);

	m_Light->SetAsDirty();
}


void DSLight::SetAttenuation(const float &Range, const float &Constant, const float &Linear, const float &Quadratic)
{
	if (GetType() == LT_DIRECTIONAL)
		return;

	m_OgreLight->setAttenuation(Range, Constant, Linear, Quadratic);

	m_Light->SetAsDirty();
}