#include "DSMaterialGenerator.h"


DSMaterialGenerator::DSMaterialGenerator(const Ogre::String &BaseName) :
	m_BaseName(BaseName)
{
}


const Ogre::MaterialPtr &DSMaterialGenerator::GetMaterial(unsigned int Settings)
{
	MaterialMap::iterator it = m_Materials.find(Settings);

	if (it == m_Materials.end())
	{
		const Ogre::MaterialPtr material = GetTemplateMaterial(Settings);
		Ogre::MaterialPtr clone = material->clone(material->getName() + "-" + Ogre::StringConverter::toString(Settings));
		Ogre::Pass *pass = clone->getTechnique(0)->getPass(0);
		pass->setVertexProgram(GetVertexProgram(Settings)->getName());
		pass->setFragmentProgram(GetFragmentProgram(Settings)->getName());

		m_Materials[Settings] = clone;

		return m_Materials[Settings];
	}

	return it->second;
}


const Ogre::GpuProgramPtr &DSMaterialGenerator::GetVertexProgram(unsigned int Settings)
{
	ProgramMap::iterator it = m_VertexPrograms.find(Settings);

	if (it == m_VertexPrograms.end())
	{
		m_VertexPrograms[Settings] = GetTemplateVertexProgram(Settings);

		return m_VertexPrograms[Settings];
	}

	return it->second;
}


const Ogre::GpuProgramPtr &DSMaterialGenerator::GetFragmentProgram(unsigned int Settings)
{
	ProgramMap::iterator it = m_FragmentPrograms.find(Settings);

	if (it == m_FragmentPrograms.end())
	{
		m_FragmentPrograms[Settings] = GetTemplateFragmentProgram(Settings);

		return m_FragmentPrograms[Settings];
	}

	return it->second;
}
