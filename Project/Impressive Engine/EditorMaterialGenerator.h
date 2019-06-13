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

#pragma once

#include "MaterialGenerator.h"
#include "Singleton.h"


BEGIN_NAMESPACE
class EditorMaterialGenerator : public MaterialGenerator, public Singleton<EditorMaterialGenerator>
{
public:
	enum EditorUnit
	{
		EU_CAMERA = 0,
		EU_LIGHT
	};

public:
	EditorMaterialGenerator(void);
	//~EditorMaterialGenerator(void);

	const Ogre::MaterialPtr &GetMaterial(const EditorUnit &Unit);

	void Initialize(void);
};
END_NAMESPACE

#endif