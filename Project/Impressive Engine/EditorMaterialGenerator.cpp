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
#ifdef EDIT_MODE

#include "EditorMaterialGenerator.h"


BEGIN_NAMESPACE
EditorMaterialGenerator *Singleton<EditorMaterialGenerator>::m_Instance = NULL;


EditorMaterialGenerator::EditorMaterialGenerator(void) :
	MaterialGenerator("Editor")
{
}


//EditorMaterialGenerator::~EditorMaterialGenerator(void)
//{
//}


const Ogre::MaterialPtr &EditorMaterialGenerator::GetMaterial(const EditorUnit &Unit)
{
	return MaterialGenerator::GetMaterial(Unit);
}


void EditorMaterialGenerator::Initialize(void)
{
	Ogre::MaterialPtr material = GetMaterial(EU_CAMERA);
	material->setLightingEnabled(false);
	material->setSeparateSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_DEST_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ZERO);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("Camera.png");

	material = GetMaterial(EU_LIGHT);
	material->setLightingEnabled(false);
	material->setSeparateSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_DEST_ALPHA, Ogre::SBF_ONE, Ogre::SBF_ZERO);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("Sun.png");
}
END_NAMESPACE

#endif