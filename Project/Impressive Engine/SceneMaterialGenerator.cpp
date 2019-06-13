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
#include "SceneMaterialGenerator.h"


BEGIN_NAMESPACE
SceneMaterialGenerator *Singleton<SceneMaterialGenerator>::m_Instance = NULL;


SceneMaterialGenerator::SceneMaterialGenerator(void) :
	MaterialGenerator("Editor")
{
}


//SceneMaterialGenerator::~SceneMaterialGenerator(void)
//{
//}


const Ogre::MaterialPtr &SceneMaterialGenerator::GetMaterial(const SceneUnit &Unit)
{
	return MaterialGenerator::GetMaterial(Unit);
}


void SceneMaterialGenerator::Initialize(void)
{
	Ogre::MaterialPtr material = GetMaterial(SU_DEBUG_DRAWER);
	//ambient vertexcolour
	//diffuse vertexcolour
	//specular vertexcolour
	material->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_AMBIENT | Ogre::TVC_DIFFUSE | Ogre::TVC_SPECULAR);
	material->setLightingEnabled(false);
	material->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_DEST_ALPHA);
	material->setDepthCheckEnabled(true);
	material->setDepthWriteEnabled(false);
}
END_NAMESPACE