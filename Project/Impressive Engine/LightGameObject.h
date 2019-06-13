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

#include "GameObject.h"
#include "Light.h"


BEGIN_NAMESPACE
class LightGameObject : public GameObject
{
public:
	LightGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent = NULL);
	~LightGameObject(void);
	
#ifdef EDIT_MODE
	bool Update(void);

	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);
	
	void SetInitializeState(void);
	void ResetToInitializeState(void);

	const bool GetEnabled(void);
	void SetEnabled(const bool &Value);

	void SetScale(const Ogre::Vector3& Scale)
	{
		m_SceneNode->setScale(Ogre::Vector3::UNIT_SCALE);
	}
	
	void SetScale(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_SceneNode->setScale(Ogre::Vector3::UNIT_SCALE);
	}

	GameObjectType GetType(void) const
	{
		return GOT_LIGHT;
	}

	//void SetCastShadows(bool Value)
	//{
	//	m_Light->setCastShadows(Value);
	//}

	//bool GetCastShadows(void)
	//{
	//	return m_Light->getCastShadows();
	//}
	
	void SetVisible(bool Value)
	{
		GameObject::SetVisible(Value);

		SetEnabled(Value);
	}

	bool GetVisible(void)
	{
		return GetEnabled();
	}
	
	void SetLightType(const Light::LightType &Type)
	{
		m_Light->SetType(Type);
	}

	const Light::LightType GetLightType(void)
	{
		return m_Light->GetType();
	}

	void SetDirection(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_Light->SetDirection(X, Y, Z);
	}

	void SetDirection(const Ogre::Vector3 &Direction)
	{
		m_Light->SetDirection(Direction);
	}

	const Ogre::Vector3 &GetDirection(void)
	{
		return m_Light->GetDirection();
	}

	void SetSpotlightRange(const float &InnerAngle, const float &OuterAngle, const float &Falloff = 1.f)
	{
		m_Light->SetSpotlightRange(Ogre::Degree(InnerAngle), Ogre::Degree(OuterAngle), Falloff);
	}
	void SetSpotlightInnerAngle(const float &Value)
	{
		m_Light->SetSpotlightInnerAngle(Ogre::Degree(Value));
	}
	void SetSpotlightOuterAngle(const float &Value)
	{
		m_Light->SetSpotlightOuterAngle(Ogre::Degree(Value));
	}
	void SetSpotlightFalloff(const float &Value)
	{
		m_Light->SetSpotlightFalloff(Value);
	}
	const float GetSpotlightInnerAngle(void) const
	{
		return m_Light->GetSpotlightInnerAngle().valueDegrees();
	}
	const float GetSpotlightOuterAngle(void) const
	{
		return m_Light->GetSpotlightOuterAngle().valueDegrees();
	}
	const float GetSpotlightFalloff(void) const
	{
		return m_Light->GetSpotlightFalloff();
	}

	void SetDiffuseColour(const Ogre::ColourValue &Value)
	{
		m_Light->SetDiffuseColor(Value);
	}

	const Ogre::ColourValue &GetDiffuseColour(void)
	{
		return m_Light->GetDiffuseColor();
	}

	void SetSpecularColour(const Ogre::ColourValue &Value)
	{
		m_Light->SetSpecularColor(Value);
	}

	const Ogre::ColourValue &GetSpecularColour(void)
	{
		return m_Light->GetSpecularColor();
	}

	void SetAttenuationRange(const float &Range)
	{
		m_Light->SetAttenuation(
		Range, 
		m_Light->GetAttenuationConstant(),
		m_Light->GetAttenuationLinear(),
		m_Light->GetAttenuationQuadric());
	}

	const float GetAttenuationRange(void)
	{
		return m_Light->GetAttenuationRange();
	}

	void SetAttenuationConstant(const float &Constant)
	{
		m_Light->SetAttenuation(
		m_Light->GetAttenuationRange(),
		Constant,
		m_Light->GetAttenuationLinear(),
		m_Light->GetAttenuationQuadric());
	}

	const float GetAttenuationConstant(void)
	{
		return m_Light->GetAttenuationConstant();
	}

	void SetAttenuationLinear(const float &Linear)
	{
		m_Light->SetAttenuation(
		m_Light->GetAttenuationRange(),
		m_Light->GetAttenuationConstant(),
		Linear,
		m_Light->GetAttenuationQuadric());
	}

	const float GetAttenuationLinear(void)
	{
		return m_Light->GetAttenuationLinear();
	}

	void SetAttenuationQuadric(const float &Quadratic)
	{
		m_Light->SetAttenuation(
		m_Light->GetAttenuationRange(),
		m_Light->GetAttenuationConstant(),
		m_Light->GetAttenuationLinear(),
		Quadratic);
	}

	const float GetAttenuationQuadric(void)
	{
		return m_Light->GetAttenuationQuadric();
	}

	//void SetPowerScale(Ogre::Real Power)
	//{
	//	m_Light->setPowerScale(Power);
	//}

	//Ogre::Real GetPowerScale(void)
	//{
	//	return m_Light->getPowerScale();
	//}

	DECLARE_SCRIPT_REGISTATION()
	
private:
	//Ogre::Light *m_Light;
	Light *m_Light;

#ifdef EDIT_MODE
	Ogre::Entity *m_BoxEntity;
	Ogre::SceneNode *m_Box;
	//Ogre::BillboardSet *m_BillboardSet;
#endif

	Light::LightType m_Init_Type;
	Ogre::Vector3 m_Init_Direction;
	Ogre::Real m_Init_SpotlightInnerAngle;
	Ogre::Real m_Init_SpotlightOuterAngle;
	Ogre::Real m_Init_SpotlightFalloff;
	Ogre::ColourValue m_Init_DiffuseColour;
	Ogre::ColourValue m_Init_SpecularColour;
	Ogre::Real m_Init_AttenuationRange;
	Ogre::Real m_Init_AttenuationConstant;
	Ogre::Real m_Init_AttenuationLinear;
	Ogre::Real m_Init_AttenuationQuadric;
	Ogre::Real m_Init_PowerScale;
};


END_NAMESPACE