#pragma once

#include "DSSMaterialGenerator.h"

class LightMaterialGenerator : public DSSMaterialGenerator
{
public:
	enum MaterialID
	{
		MI_POINT = 0x01, // Is a point light
		MI_SPOTLIGHT = 0x02, // Is a spot light
		MI_DIRECTIONAL = 0x04, // Is a directional light
		
		MI_ATTENUATED = 0x08, // Rendered attenuated
		MI_SPECULAR = 0x10, // Specular component is calculated
		MI_SHADOW_CASTER = 0x20	// Will cast shadows
	};

public:
	LightMaterialGenerator(void);

	const Ogre::MaterialPtr GenerateTemplateMaterial(unsigned int Settings);
	const Ogre::GpuProgramPtr GenerateVertexProgram(unsigned int Settings);
	const Ogre::GpuProgramPtr GenerateFragmentProgram(unsigned int Settings);

private:
	Ogre::String m_MasterSource;
};

