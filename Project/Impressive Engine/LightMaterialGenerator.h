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

#include "DSMaterialGenerator.h"

BEGIN_NAMESPACE
class LightMaterialGenerator : public DSMaterialGenerator
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
	//~LightMaterialGenerator(void);

	const Ogre::MaterialPtr GetTemplateMaterial(unsigned int Settings);
	const Ogre::GpuProgramPtr GetTemplateVertexProgram(unsigned int Settings);
	const Ogre::GpuProgramPtr GetTemplateFragmentProgram(unsigned int Settings);

private:
	Ogre::String m_MasterSource;
};
END_NAMESPACE