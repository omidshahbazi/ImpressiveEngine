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
#include "ScriptRegister.h"
#include "ISerializable.h"

BEGIN_NAMESPACE
class Scene;


class AtmosphereManager : public ISerializable
{
public:
	AtmosphereManager(Scene *Holder);
	~AtmosphereManager(void);

	void Update(void);
	
#ifdef EDIT_MODE
	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);

	void OnViewportChanges(Ogre::RenderWindow *Window);

	void SetVisible(const bool &Value)
	{
		m_SkyX->setVisible(Value);
	}

	void SetTimeOfDay(const int &Hour, const float &Minute);

	void SetTimeOfDay(const Ogre::Vector2 &Time)
	{
		SetTimeOfDay(Time.x, Time.y);
	}

	const Ogre::Vector2 GetTimeOfDay(void);

	void SetTimeOfSunrise(const int &Hour, const float &Minute);

	void SetTimeOfSunrise(const Ogre::Vector2 &Time)
	{
		SetTimeOfSunrise(Time.x, Time.y);
	}

	const Ogre::Vector2 GetTimeOfSunrise(void);

	void SetTimeOfSunset(const int &Hour, const float &Minute);

	void SetTimeOfSunset(const Ogre::Vector2 &Time)
	{
		SetTimeOfSunset(Time.x, Time.y);
	}

	const Ogre::Vector2 GetTimeOfSunset(void);

	void SetEastDirection(const Ogre::Vector2 &Direction)
	{
		m_Controller->setEastDirection(Direction);
	}

	const Ogre::Vector2 &GetEastDirection(void) const
	{
		return m_Controller->getEastDirection();
	}

	const Ogre::Vector3 &GetSunDirection(void) const
	{
		return m_Controller->getSunDirection();
	}

	const Ogre::Vector3 &GetSunPosition(void) const;

	const Ogre::Vector3 &GetMoonDirection(void) const
	{
		return m_Controller->getMoonDirection();
	}

	void SetMoonPhase(const float &Phase)
	{
		m_Controller->setMoonPhase(Phase);
	}

	const float GetMoonPhase(void)
	{
		return m_Controller->getMoonPhase();
	}

	void SetTimeMultiplier(const float &TimeMultiplier)
	{
		m_SkyX->setTimeMultiplier(TimeMultiplier);
	}

	const float &GetTimeMultiplier(void) const
	{
		return m_SkyX->getTimeMultiplier();
	}

	void SetLightingMode(const SkyX::SkyX::LightingMode &Lighting)
	{
		m_SkyX->setLightingMode(Lighting);
	}

	const SkyX::SkyX::LightingMode &GetLightingMode(void) const
	{
		return m_SkyX->getLightingMode();
	}

	void SetStarfieldEnabled(const bool &Value)
	{
		m_SkyX->setStarfieldEnabled(Value);
	}

	const bool &GetStarfieldEnabled(void) const
	{
		return m_SkyX->isStarfieldEnabled();
	}

	void SetMaxFarClipDistance(const float &Distance)
	{
		m_SkyX->setInfiniteCameraFarClipDistance(Distance);
	}

	const float &GetMaxFarClipDistance(void) const
	{
		return m_SkyX->getInfiniteCameraFarClipDistance();
	}

	SkyX::AtmosphereManager::Options &GetAtmosphereManagerOption(void)
	{
		return m_AtmosphereManagerOption;
	}

	const Ogre::String &GetEntityName(void) const
	{
		return m_SkyX->getMeshManager()->getEntity()->getName();
	}

	void UpdateAtmosphereManagerOptionChanges(void);

	DECLARE_SCRIPT_REGISTATION()

private:
	Scene *m_Holder;
	SkyX::BasicController *m_Controller;
	SkyX::SkyX *m_SkyX;
	SkyX::AtmosphereManager::Options m_AtmosphereManagerOption;
	Ogre::RenderWindow *m_LastAddedWindow;
};

END_NAMESPACE
