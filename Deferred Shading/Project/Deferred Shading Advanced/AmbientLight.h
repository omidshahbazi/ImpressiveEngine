#pragma once

#include <Ogre.h>

class DeferredShadingSystem;
class DSLightRenderable;

class AmbientLight
{
public:
	AmbientLight(DeferredShadingSystem *DeferredShadingSystem);
	~AmbientLight(void);

	void Render(void);

private:
	DSLightRenderable *m_Light;
};

