#pragma once

#include <Ogre.h>

class DSMaterialGenerator
{
public:
	DSMaterialGenerator(const Ogre::String &BaseName);

	const Ogre::MaterialPtr &GetMaterial(unsigned int Settings);
	const Ogre::GpuProgramPtr &GetVertexProgram(unsigned int Settings);
	const Ogre::GpuProgramPtr &GetFragmentProgram(unsigned int Settings);

private:
	virtual const Ogre::MaterialPtr GetTemplateMaterial(unsigned int Settings) = 0;
	virtual const Ogre::GpuProgramPtr GetTemplateVertexProgram(unsigned int Settings) = 0;
	virtual const Ogre::GpuProgramPtr GetTemplateFragmentProgram(unsigned int Settings) = 0;

protected:
	Ogre::String m_BaseName;

private:
	typedef std::map<unsigned int, Ogre::MaterialPtr> MaterialMap;
	typedef std::map<unsigned int, Ogre::GpuProgramPtr> ProgramMap;

	MaterialMap m_Materials;
	ProgramMap m_VertexPrograms;
	ProgramMap m_FragmentPrograms;
};

