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

#include "StdAfxOGRE.h"
#include "Common.h"

BEGIN_NAMESPACE
class DSCommon
{
public:
	static void CreateResources(void);

	static void UpdateParameters(const Ogre::Camera *Camera);

	static Ogre::Vector3 &GetFarCorner(void)
	{
		return m_FarCorner;
	}
	
public:
	static const char *MAIN_FP_NAME;
	static const char *MAIN_VP_NAME;

	static const char *LIGHTING_VP_NAME;
	static const char *GEOMETRY_LIGHT_VP_NAME;

	static const char *AMBIENT_LIGHT_MATERIAL_NAME;
	static const char *GEOMETRY_LIGHT_MATERIAL_NAME;
	static const char *QUAD_LIGHT_MATERIAL_NAME;

	static const unsigned char FIRST_RENDER_QUEUE;
	static const unsigned char LAST_RENDER_QUEUE;

	static const char *GBUFFER_NAME;
	static const char *SHOW_LIT_NAME;
	static const char *SHOW_DEPTH_NAME;
	static const char *SHOW_SPECULAR_NAME;
	static const char *SHOW_NORMAL_NAME;
	static const char *SHOW_ALBEDO_NAME;

	static const char *SCHEME_GBUFFER;
	static const char *SCHEME_NO_GBUFFER;

	static const char *DEFERRED_LIGHTING_PASS;

private:
	static Ogre::Vector3 m_FarCorner;
};
END_NAMESPACE