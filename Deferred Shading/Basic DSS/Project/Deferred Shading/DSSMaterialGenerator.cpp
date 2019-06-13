#include "DSSMaterialGenerator.h"


DSSMaterialGenerator::DSSMaterialGenerator(const Ogre::String &BaseName, const unsigned int &MaterialMask, const unsigned int &VertexShaderMask, const unsigned int &FragmentShaderMask) :
	m_BaseName(BaseName),
	m_MaterialMask(MaterialMask),
	m_VertexShaderMask(VertexShaderMask),
	m_FragmentShaderMask(FragmentShaderMask)
{
}


const Ogre::MaterialPtr &DSSMaterialGenerator::GetMaterial(unsigned int Settings)
{
	MaterialMap::iterator it = m_Materials.find(Settings);

	if (it == m_Materials.end())
	{
		Ogre::MaterialPtr material = GetTemplateMaterial(Settings & m_MaterialMask);
		Ogre::GpuProgramPtr vertexShader = GetVertexProgram(Settings & m_VertexShaderMask);
		Ogre::GpuProgramPtr fragmentShader = GetFragmentProgram(Settings & m_FragmentShaderMask);

		Ogre::String name = m_BaseName + Ogre::StringConverter::toString(Settings);

		Ogre::MaterialPtr clone = material->clone(name);
		Ogre::Pass *pass = clone->getTechnique(0)->getPass(0);
		pass->setVertexProgram(vertexShader->getName());
		pass->setFragmentProgram(fragmentShader->getName());

		m_Materials[Settings] = clone;

		return m_Materials[Settings];
	}
	else
		return it->second;
}


const Ogre::MaterialPtr &DSSMaterialGenerator::GetTemplateMaterial(unsigned int Settings)
{
	MaterialMap::iterator it = m_TemplateMaterials.find(Settings);

	if (it == m_TemplateMaterials.end())
	{
		m_TemplateMaterials[Settings] = GenerateTemplateMaterial(Settings);

		return m_TemplateMaterials[Settings];
	}
	else
		return it->second;
}


const Ogre::GpuProgramPtr &DSSMaterialGenerator::GetVertexProgram(unsigned int Settings)
{
	GPUProgramMap::iterator it = m_VertexPrograms.find(Settings);

	if (it == m_VertexPrograms.end())
	{
		m_VertexPrograms[Settings] = GenerateVertexProgram(Settings);

		return m_VertexPrograms[Settings];
	}
	else
		return it->second;
}


const Ogre::GpuProgramPtr &DSSMaterialGenerator::GetFragmentProgram(unsigned int Settings)
{
	GPUProgramMap::iterator it = m_FragmentPrograms.find(Settings);

	if (it == m_FragmentPrograms.end())
	{
		m_FragmentPrograms[Settings] = GenerateFragmentProgram(Settings);

		return m_FragmentPrograms[Settings];
	}
	else
		return it->second;
}