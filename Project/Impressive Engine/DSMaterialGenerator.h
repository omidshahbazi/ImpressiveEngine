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

#include "MaterialGenerator.h"

BEGIN_NAMESPACE
class DSMaterialGenerator : protected MaterialGenerator
{
public:
	DSMaterialGenerator(const Ogre::String &BaseName);
	//~DSMaterialGenerator(void);

	const Ogre::MaterialPtr &GetMaterial(unsigned int Settings);
	const Ogre::GpuProgramPtr &GetVertexProgram(unsigned int Settings);
	const Ogre::GpuProgramPtr &GetFragmentProgram(unsigned int Settings);

private:
	virtual const Ogre::MaterialPtr GetTemplateMaterial(unsigned int Settings) = 0;
	virtual const Ogre::GpuProgramPtr GetTemplateVertexProgram(unsigned int Settings) = 0;
	virtual const Ogre::GpuProgramPtr GetTemplateFragmentProgram(unsigned int Settings) = 0;

private:
	typedef std::map<unsigned int, Ogre::GpuProgramPtr> ProgramMap;

	ProgramMap m_VertexPrograms;
	ProgramMap m_FragmentPrograms;
};
END_NAMESPACE