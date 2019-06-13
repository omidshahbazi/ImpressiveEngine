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
#include <Ogre.h>

BEGIN_NAMESPACE
class DeferredShadingSystem;
class DSLightRenderable;


class Light
{
public:
	enum LightType
	{
		LT_POINT = 0,
		LT_DIRECTIONAL,
		LT_SPOTLIGHT
	};

public:
	Light(DeferredShadingSystem *DeferredShadingSystem);
	~Light(void);

	void Render(void);

	void SetType(const LightType &type);
	const LightType GetType(void) const;

	void SetDiffuseColor(const float &Red, const float &Green, const float &Blue)
	{
		m_OgreLight->setDiffuseColour(Red, Green, Blue);
	}
	void SetDiffuseColor(const Ogre::ColourValue &Color)
	{
		m_OgreLight->setDiffuseColour(Color);
	}
	const Ogre::ColourValue &GetDiffuseColor(void) const
	{
		return m_OgreLight->getDiffuseColour();
	}

	void SetSpecularColor(const float &Red, const float &Green, const float &Blue);
	void SetSpecularColor(const Ogre::ColourValue &Color)
	{
		SetSpecularColor(Color.r, Color.g, Color.b);
	}
	const Ogre::ColourValue &GetSpecularColor(void) const
	{
		return m_OgreLight->getSpecularColour();
	}

	void SetAttenuation(const float &Range, const float &Constant, const float &Linear, const float &Quadratic);
	const float GetAttenuationRange(void) const
	{
		return m_OgreLight->getAttenuationRange();
	}
	const float GetAttenuationConstant(void) const
	{
		return m_OgreLight->getAttenuationConstant();
	}
	const float GetAttenuationLinear(void) const
	{
		return m_OgreLight->getAttenuationLinear();
	}
	const float GetAttenuationQuadric(void) const
	{
		return m_OgreLight->getAttenuationQuadric();
	}

	void SetPosition(const float &X, const float &Y, const float &Z)
	{
		m_OgreLight->setPosition(X, Y, Z);
	}
	void SetPosition(const Ogre::Vector3 &Position)
	{
		m_OgreLight->setPosition(Position);
	}
	const Ogre::Vector3 &GetPosition(void) const
	{
		return m_OgreLight->getPosition();
	}

	void SetDirection(const float &X, const float &Y, const float &Z)
	{
		m_OgreLight->setDirection(X, Y, Z);
	}
	void SetDirection(const Ogre::Vector3 &Position)
	{
		m_OgreLight->setDirection(Position);
	}
	const Ogre::Vector3 &GetDirection(void) const
	{
		return m_OgreLight->getDirection();
	}

	void SetSpotlightRange(const Ogre::Degree &InnerAngle, const Ogre::Degree &OuterAngle, const float &Falloff = 1.f)
	{
		m_OgreLight->setSpotlightRange(InnerAngle, OuterAngle, Falloff);
	}
	void SetSpotlightInnerAngle(const Ogre::Degree &Value)
	{
		m_OgreLight->setSpotlightInnerAngle(Value);
	}
	void SetSpotlightOuterAngle(const Ogre::Degree &Value)
	{
		m_OgreLight->setSpotlightOuterAngle(Value);
	}
	void SetSpotlightFalloff(const float &Value)
	{
		m_OgreLight->setSpotlightFalloff(Value);
	}
	const Ogre::Degree GetSpotlightInnerAngle(void) const
	{
		return m_OgreLight->getSpotlightInnerAngle();
	}
	const Ogre::Degree GetSpotlightOuterAngle(void) const
	{
		return m_OgreLight->getSpotlightOuterAngle();
	}
	const float GetSpotlightFalloff(void) const
	{
		return m_OgreLight->getSpotlightFalloff();
	}

	// Right not, it doesn't works properly, because there's no any implementation for it
	// So we can use it bu passing a float to fragment-shader by ACT_LIGHT_POWER_SCALE
	//void SetPowerScale(const float &PowerScale)
	//{
	//	m_OgreLight->setPowerScale(PowerScale);
	//}
	//const float &GetPowerScale(void) const
	//{
	//	return m_OgreLight->getPowerScale();
	//}

	void SetEnabled(const bool &Enabled)
	{
		m_OgreLight->setVisible(Enabled);
	}
	const bool GetEnabled(void)
	{
		return m_OgreLight->getVisible();
	}

	Ogre::Light *_GetOgreLight(void) const
	{
		return m_OgreLight;
	}
	
private:
	DeferredShadingSystem *m_Holder;

	Ogre::Light *m_OgreLight;

	DSLightRenderable *m_Light;
};


END_NAMESPACE