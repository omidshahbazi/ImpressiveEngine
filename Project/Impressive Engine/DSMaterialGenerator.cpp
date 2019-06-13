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
#include "DSMaterialGenerator.h"


BEGIN_NAMESPACE
DSMaterialGenerator::DSMaterialGenerator(const Ogre::String &BaseName) :
	MaterialGenerator(BaseName)
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

END_NAMESPACE