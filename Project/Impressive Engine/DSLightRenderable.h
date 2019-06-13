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

#include "Common.h"
#include "StdAfxOGRE.h"


BEGIN_NAMESPACE
class DeferredShadingSystem;


class DSLightRenderable : public Ogre::SimpleRenderable
{
public:
	enum LightType
	{
		LT_AMBIENT = 0,
		LT_POINT,
		LT_DIRECTIONAL,
		LT_SPOTLIGHT
	};

public:
	DSLightRenderable(DeferredShadingSystem *DeferredShadingSystem, const LightType &Type, const float &Radius = 0.f, Ogre::Light *Light = NULL);
	//~DSLightRenderable(void);

	void Render(void);

	void SetType(const LightType &Value);
	const LightType &GetType(void)
	{
		return m_LightType;
	}

	float getBoundingRadius(void) const
	{
		return m_Radius;
	}
	float getSquaredViewDepth(const Ogre::Camera *Camera) const;
	const Ogre::MaterialPtr &getMaterial(void) const
	{
		return m_Material;
	}
	void getWorldTransforms(Ogre::Matrix4 *xform) const;

	void SetAsDirty(void)
	{
		m_IsDirty = true;
	}

private:
	void UpdateLightSettings(void);

	void UpdateParameters(const Ogre::Camera *Camera);

	void UpdateMaterial(const Ogre::Camera *Camera);

	bool IsCameraInside(const Ogre::Camera *Camera);

	void RebuildGeometry(const float &Radius);

	const float GetOuterRadius(void);

protected:
	DeferredShadingSystem *m_DeferredShadingSystem;
	LightType m_LightType;
	float m_Radius;
	Ogre::Light *m_OgreLight;
	Ogre::LightList m_LightList;

	bool m_IgnoreWorld;

	bool m_IsDirty;
	unsigned int m_Settings;

	Ogre::MaterialPtr m_Material;
};


END_NAMESPACE