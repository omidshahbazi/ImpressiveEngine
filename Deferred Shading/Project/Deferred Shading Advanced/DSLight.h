#pragma once

#include <Ogre.h>

class DeferredShadingSystem;
class DSLightRenderable;


class DSLight
{
public:
	enum LightType
	{
		LT_POINT = 0,
		LT_DIRECTIONAL,
		LT_SPOTLIGHT
	};

public:
	DSLight(DeferredShadingSystem *DeferredShadingSystem);
	~DSLight(void);

	void Render(void);

	void SetType(const LightType &type);
	const LightType &GetType(void) const;

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
		m_OgreLight->setSpecularColour(Color);
	}
	const Ogre::ColourValue &SetSpecularColor(void) const
	{
		return m_OgreLight->getSpecularColour();
	}

	void SetAttenuation(const float &Range, const float &Constant, const float &Linear, const float &Quadratic);
	const float &GetAttenuationRange(void) const
	{
		return m_OgreLight->getAttenuationRange();
	}
	const float &GetAttenuationConstant(void) const
	{
		return m_OgreLight->getAttenuationConstant();
	}
	const float &GetAttenuationLinear(void) const
	{
		return m_OgreLight->getAttenuationLinear();
	}
	const float &GetAttenuationQuadric(void) const
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

	//void SetSpotlightRange(const Radian& innerAngle, const Radian& outerAngle, const float &falloff = 1.0);
	//void SetSpotlightInnerAngle(const Radian& val);
	//void SetSpotlightOuterAngle(const Radian& val);
	//void SetSpotlightFalloff(const float &val);
	//const Radian& GetSpotlightInnerAngle(void) const;
	//const Radian& GetSpotlightOuterAngle(void) const;
	//const float &GetSpotlightFalloff(void) const;

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
	const bool &GetEnabled(void)
	{
		m_OgreLight->getVisible();
	}

	Ogre::Light *_GetOgreLight(void) const
	{
		return m_OgreLight;
	}
	
private:
	Ogre::Light *m_OgreLight;

	DSLightRenderable *m_Light;
};

