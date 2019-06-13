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
#include "AtmosphereManager.h"
#include "Engine.h"
#include "Scene.h"
#include "DSCommon.h"
#include "OceanGameObject.h"
#include "Utility.h"
#include "Log.h"

#ifdef EDIT_MODE
#include "MainWindow.h"
//#include "Time.h"
#endif


BEGIN_NAMESPACE
AtmosphereManager::AtmosphereManager(Scene *Holder) :
	m_Holder(Holder),
	m_Controller(NULL),
	m_SkyX(NULL),
	m_LastAddedWindow(NULL)
{
	m_Controller = new SkyX::BasicController();

	m_SkyX = new SkyX::SkyX(m_Holder->GetSceneManager(), m_Controller);
	m_SkyX->create();

	






	// Layered clouds
	if (m_SkyX->getCloudsManager()->getCloudLayers().empty())
	{
		m_SkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(/* Default options */));
		m_SkyX->getCloudsManager()->add(SkyX::CloudLayer::Options(10, 100, Ogre::Vector2::UNIT_X, 1));
	}





	//m_SkyX->getVCloudsManager()->setWindSpeed(411);
	//m_SkyX->getVCloudsManager()->setAutoupdate(false);

	//SkyX::VClouds::VClouds* vclouds = m_SkyX->getVCloudsManager()->getVClouds();

	//vclouds->setWindDirection(Ogre::Degree(45));
	//vclouds->setAmbientColor(Ogre::Vector3(0, 0, 0.2f));
	//vclouds->setLightResponse(preset.vcLightResponse);
	//vclouds->setAmbientFactors(preset.vcAmbientFactors);
	//vclouds->setWheater(0.5, 0.9f, true);

	//vclouds->getLightningManager()->setEnabled(preset.vcLightnings);
	//vclouds->getLightningManager()->setAverageLightningApparitionTime(preset.vcLightningsAT);
	//vclouds->getLightningManager()->setLightningColor(preset.vcLightningsColor);
	//vclouds->getLightningManager()->setLightningTimeMultiplier(preset.vcLightningsTM);


	// Must called in AtmosphereManager::OnViewportChanges();
	
	//if (m_SkyX->getVCloudsManager()->isCreated())
	//	m_SkyX->getVCloudsManager()->remove();

	//m_SkyX->getVCloudsManager()->create(m_SkyX->getMeshManager()->getSkydomeRadius(Window->getViewport(0)->getCamera()));
}


AtmosphereManager::~AtmosphereManager(void)
{
	m_Holder = NULL;
	
	if (m_LastAddedWindow)
		m_LastAddedWindow->removeListener(m_SkyX);

	Engine::GetInstance().GetGraphic()->removeFrameListener(m_SkyX);

	delete m_SkyX;
	m_SkyX = NULL;

	//delete m_Controller;
	m_Controller = NULL;
}


void AtmosphereManager::Update(void)
{
	//Log::GetInstance().Clear();

	//Log::GetInstance().AddInfo(Ogre::StringConverter::toString(m_SkyX->getMeshManager()->getEntity()->getRenderQueueGroup()));

	//m_SkyX->getMeshManager()->getEntity()->setRenderQueueGroup(DSCommon::FIRST_RENDER_QUEUE - 1);

#ifdef EDIT_MODE
	if (MainWindow::GetPointer()->IsInEditMode())
	{
		m_SkyX->update(0);
		//m_SkyX->getCloudsManager()->update();
		//m_SkyX->getVCloudsManager()->update(Time::GetInstance().GetTimeSinceLastUpdate() / 1000.f);
	}
#endif

	BEGIN_LIST_ITERATOR(m_Holder->GetChildren(), GameObject*)
		if (current->GetType() == GameObject::GOT_OCEAN)
			((OceanGameObject*)current)->SetSunPosition(GetSunDirection());
	END_LIST_ITERATOR(NULL)
}


#ifdef EDIT_MODE
bool AtmosphereManager::Serialize(Attributes *Attributes)
{
	Attributes->AddVector2("TimeOfDay", GetTimeOfDay());
	Attributes->AddVector2("TimeOfSunrise", GetTimeOfSunrise());
	Attributes->AddVector2("TimeOfSunset", GetTimeOfSunset());
	Attributes->AddVector2("EastDirection", GetEastDirection());
	Attributes->AddFloat("MoonPhase", GetMoonPhase());
	Attributes->AddFloat("TimeMultiplier", GetTimeMultiplier());
	Attributes->AddInteger("LightingMode", GetLightingMode());
	Attributes->AddBoolean("StarfieldEnabled", GetStarfieldEnabled());
	Attributes->AddFloat("MaxFarClipDistance", GetMaxFarClipDistance());

	Attributes->AddFloat("InnerRadius", m_AtmosphereManagerOption.InnerRadius);
	Attributes->AddFloat("OuterRadius", m_AtmosphereManagerOption.OuterRadius);
	Attributes->AddFloat("HeightPosition", m_AtmosphereManagerOption.HeightPosition);
	Attributes->AddFloat("RayleighMultiplier", m_AtmosphereManagerOption.RayleighMultiplier);
	Attributes->AddFloat("MieMultiplier", m_AtmosphereManagerOption.MieMultiplier);
	Attributes->AddFloat("SunIntensity", m_AtmosphereManagerOption.SunIntensity);
	Attributes->AddVector3("WaveLength", m_AtmosphereManagerOption.WaveLength);
	Attributes->AddFloat("G", m_AtmosphereManagerOption.G);
	Attributes->AddFloat("Exposure", m_AtmosphereManagerOption.Exposure);
	Attributes->AddInteger("NumberOfSamples", m_AtmosphereManagerOption.NumberOfSamples);

	return true;
}
#endif

bool AtmosphereManager::Deserialize(Attributes *Attributes)
{
	SetTimeOfDay(Attributes->GetVector2("TimeOfDay"));
	SetTimeOfSunrise(Attributes->GetVector2("TimeOfSunrise"));
	SetTimeOfSunset(Attributes->GetVector2("TimeOfSunset"));
	SetMoonPhase(Attributes->GetFloat("MoonPhase"));
	SetTimeMultiplier(Attributes->GetFloat("TimeMultiplier"));
	SetLightingMode((SkyX::SkyX::LightingMode)Attributes->GetInteger("LightingMode"));
	SetStarfieldEnabled(Attributes->GetBoolean("StarfieldEnabled"));
	SetMaxFarClipDistance(Attributes->GetFloat("MaxFarClipDistance"));

	m_AtmosphereManagerOption.InnerRadius = Attributes->GetFloat("InnerRadius");
	m_AtmosphereManagerOption.OuterRadius = Attributes->GetFloat("OuterRadius");
	m_AtmosphereManagerOption.HeightPosition = Attributes->GetFloat("HeightPosition");
	m_AtmosphereManagerOption.RayleighMultiplier = Attributes->GetFloat("RayleighMultiplier");
	m_AtmosphereManagerOption.MieMultiplier = Attributes->GetFloat("MieMultiplier");
	m_AtmosphereManagerOption.SunIntensity = Attributes->GetFloat("SunIntensity");
	m_AtmosphereManagerOption.WaveLength = Attributes->GetVector3("WaveLength");
	m_AtmosphereManagerOption.G = Attributes->GetFloat("G");
	m_AtmosphereManagerOption.Exposure = Attributes->GetFloat("Exposure");
	m_AtmosphereManagerOption.NumberOfSamples = Attributes->GetInteger("NumberOfSamples");

	UpdateAtmosphereManagerOptionChanges();

	return true;
}


void AtmosphereManager::OnViewportChanges(Ogre::RenderWindow *Window)
{
	if (m_LastAddedWindow)
		m_LastAddedWindow->removeListener(m_SkyX);
	
	m_LastAddedWindow = Window;

	m_LastAddedWindow->addListener(m_SkyX);

#ifdef EDIT_MODE
	if (MainWindow::GetPointer()->IsInEditMode())
		Engine::GetInstance().GetGraphic()->removeFrameListener(m_SkyX);
	else
#endif
		Engine::GetInstance().GetGraphic()->addFrameListener(m_SkyX);
	
	//m_SkyX->getVCloudsManager()->create(m_SkyX->getMeshManager()->getSkydomeRadius(Window->getViewport(0)->getCamera()));
}


void AtmosphereManager::SetTimeOfDay(const int &Hour, const float &Minute)
{
	m_Controller->setTime(Ogre::Vector3(
		Hour + (Minute / 60),
		m_Controller->getTime().y,
		m_Controller->getTime().z));
}


const Ogre::Vector2 AtmosphereManager::GetTimeOfDay(void)
{
	int hour = (int)m_Controller->getTime().x;
	return Ogre::Vector2(hour, (m_Controller->getTime().x - hour) * 60);
}


void AtmosphereManager::SetTimeOfSunrise(const int &Hour, const float &Minute)
{
	m_Controller->setTime(Ogre::Vector3(
		m_Controller->getTime().x,
		Hour + (Minute / 60),
		m_Controller->getTime().z));
}


void AtmosphereManager::SetTimeOfSunset(const int &Hour, const float &Minute)
{
	m_Controller->setTime(Ogre::Vector3(
		m_Controller->getTime().x,
		m_Controller->getTime().y,
		Hour + (Minute / 60)));
}


const Ogre::Vector2 AtmosphereManager::GetTimeOfSunset(void)
{
	int hour = (int)m_Controller->getTime().z;
	return Ogre::Vector2(hour, (m_Controller->getTime().z - hour) * 60);
}


const Ogre::Vector3 &AtmosphereManager::GetSunPosition(void) const
{
	//if (m_LastAddedWindow)
		//return m_LastAddedWindow->getViewport(0)->getCamera()->getDerivedPosition() - m_Controller->getSunDirection() * m_LastAddedWindow->getViewport(0)->getCamera()->getFarClipDistance() * 0.5f;

	return Ogre::Vector3::ZERO;
}


const Ogre::Vector2 AtmosphereManager::GetTimeOfSunrise(void)
{
	int hour = (int)m_Controller->getTime().y;
	return Ogre::Vector2(hour, (m_Controller->getTime().y - hour) * 60);
}


void AtmosphereManager::UpdateAtmosphereManagerOptionChanges(void)
{
	m_SkyX->getAtmosphereManager()->setOptions(m_AtmosphereManagerOption);
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AtmosphereManager)
	BEGIN_DEFINE_TYPE(AtmosphereManager, "AtmosphereManager")

	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE