#include "AmbientLight.h"
#include "DSLightRenderable.h"


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
