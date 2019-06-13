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

#include "StdAfxOGRE.h"
#include "Singleton.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class Scene;

class DebugDrawer : public Singleton<DebugDrawer>
{
public:
	enum DrawType
	{
		DT_AI = 0,
		DT_SYSTEM
	};

public:
	//DebugDrawer(void);
	~DebugDrawer(void);

	void Initialize(Scene *Scene);

	void BeginUpdate(void);
	void EndUpdate(void);

	void DrawLine(const DrawType &Type, const Ogre::Vector3 &Start, const Ogre::Vector3 &End, const Ogre::ColourValue &Color);
	void DrawCircle(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Radius, const int &SegmentsCount, const Ogre::ColourValue &Color, const bool &IsFilled = false);
	void DrawCylinder(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Radius, const int &SegmentsCount, const float &Height, const Ogre::ColourValue &Color, const bool &IsFilled = false);
	void DrawQuad(const DrawType &Type, const Ogre::Vector3 &LeftUp, const Ogre::Vector3 &RightUp, const Ogre::Vector3 &RightDown, const Ogre::Vector3 &LeftDown, const Ogre::ColourValue &Color, const bool &IsFilled = false);
	void DrawCuboid(const DrawType &Type, const Ogre::AxisAlignedBox &AAB, const Ogre::ColourValue &Color, const bool &IsFilled = false);
	void DrawSphere(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Radius, const Ogre::ColourValue &Color, const bool &IsFilled = false);
	void DrawTetrahedron(const DrawType &Type, const Ogre::Vector3 &Centre, const float &Scale, const Ogre::ColourValue &Color, const bool &IsFilled = false);

	DECLARE_SCRIPT_REGISTATION()
};
END_NAMESPACE

#endif