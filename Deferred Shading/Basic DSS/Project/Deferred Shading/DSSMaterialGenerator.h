#pragma once

#include <Ogre.h>

class DSSMaterialGenerator
{
public:
	DSSMaterialGenerator(const Ogre::String &BaseName, const unsigned int &MaterialMask, const unsigned int &VertexShaderMask, const unsigned int &FragmentShaderMask);

	const Ogre::MaterialPtr &GetMaterial(unsigned int Settings);

private:
	const Ogre::MaterialPtr &GetTemplateMaterial(unsigned int Settings);
	const Ogre::GpuProgramPtr &GetVertexProgram(unsigned int Settings);
	const Ogre::GpuProgramPtr &GetFragmentProgram(unsigned int Settings);

	virtual const Ogre::MaterialPtr GenerateTemplateMaterial(unsigned int Settings) = 0;
	virtual const Ogre::GpuProgramPtr GenerateVertexProgram(unsigned int Settings) = 0;
	virtual const Ogre::GpuProgramPtr GenerateFragmentProgram(unsigned int Settings) = 0;

protected:
	Ogre::String m_BaseName;
	
	/// Mask of settings bits that influence template material choice
	unsigned int m_MaterialMask;
	/// Mask of settings bits that influence vertex shader choice
	unsigned int m_VertexShaderMask;
	/// Mask of settings bits that influence fragment shader choice
	unsigned int m_FragmentShaderMask;
	
private:
	typedef std::map<unsigned int, Ogre::MaterialPtr> MaterialMap;
	typedef std::map<unsigned int, Ogre::GpuProgramPtr> GPUProgramMap;

	MaterialMap m_Materials;
	
	MaterialMap m_TemplateMaterials;
	GPUProgramMap m_VertexPrograms;
	GPUProgramMap m_FragmentPrograms;
};