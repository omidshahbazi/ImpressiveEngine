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
#include "Singleton.h"


BEGIN_NAMESPACE
class SceneMaterialGenerator : public MaterialGenerator, public Singleton<SceneMaterialGenerator>
{
public:
	enum SceneUnit
	{
		SU_DEBUG_DRAWER = 0
	};

public:
	SceneMaterialGenerator(void);
	//~SceneMaterialGenerator(void);

	const Ogre::MaterialPtr &GetMaterial(const SceneUnit &Unit);

	void Initialize(void);
};
END_NAMESPACE