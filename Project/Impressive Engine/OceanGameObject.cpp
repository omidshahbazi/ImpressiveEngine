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
#include "OceanGameObject.h"
#include "Scene.h"
#include "ViewPortsManager.h"
#include "RenderControl.h"
#include "Time.h"

#ifdef EDIT_MODE
#include "MainWindow.h"
#endif

BEGIN_NAMESPACE
OceanGameObject::OceanGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent) :
	GameObject(Name, Holder, Parent),
	m_Hydrax(NULL),
	m_ModuleType(MT_SIMPLE_GRID),
	m_NoiseType(NT_PERLIN),
	m_NormalMode(Hydrax::MaterialManager::NM_RTT)
{
	OnViewportChanges(
#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport()
#else
		ViewPortsManager::GetInstance().GetViewPort()
#endif
		);

	m_WorldAABB = Ogre::AxisAlignedBox(Ogre::Vector3(-10), Ogre::Vector3(10));
}


OceanGameObject::~OceanGameObject(void)
{
	delete m_Hydrax;
}


bool OceanGameObject::Update(void)
{
#ifdef EDIT_MODE
	if (!MainWindow::GetPointer()->IsInEditMode())
#endif
		if (!GameObject::Update())
			return false;

	m_Hydrax->update(Time::GetInstance().GetTimeSinceLastUpdate() / 1000.f);

	return true;
}


void OceanGameObject::OnViewportChanges(Ogre::Viewport *Viewport)
{
	Ogre::Camera *camera = Viewport->getCamera();

	if (!camera)
		camera = m_Holder->GetSceneManager()->createCamera("HydraxTempCamera");

	bool isThereOldOne = false;

	Hydrax::HydraxComponent components;
	Ogre::PolygonMode mp;
	Hydrax::MaterialManager::ShaderMode sm;
	Ogre::Vector3 pos;
	float frd;
	float gt;
	Ogre::Vector3 sp;
	Ogre::Vector3 wc;
	float nd;
	float ss;
	float sa;
	Ogre::Vector3 sc;
	float fmd;
	float fs;
	float fStart;
	float ft;
	float dl;
	float sPower;
	float cs;
	float cp;
	float ce;
	Ogre::Vector3 gre;
	float gri;
	float ucsd;
	bool v;

	if (m_Hydrax)
	{
		isThereOldOne = true;
		
		components = m_Hydrax->getComponents();
		mp = m_Hydrax->getPolygonMode();
		sm = m_Hydrax->getShaderMode();
		pos = m_Hydrax->getPosition();
		frd = m_Hydrax->getFullReflectionDistance();
		gt = m_Hydrax->getGlobalTransparency();
		sp = m_Hydrax->getSunPosition();
		wc = m_Hydrax->getWaterColor();
		nd = m_Hydrax->getNormalDistortion();
		ss = m_Hydrax->getSunStrength();
		sa = m_Hydrax->getSunArea();
		sc = m_Hydrax->getSunColor();
		fmd = m_Hydrax->getFoamMaxDistance();
		fs = m_Hydrax->getFoamScale();
		fStart = m_Hydrax->getFoamStart();
		ft = m_Hydrax->getFoamTransparency();
		dl = m_Hydrax->getDepthLimit();
		sPower = m_Hydrax->getSmoothPower();
		cs = m_Hydrax->getCausticsScale();
		cp = m_Hydrax->getCausticsPower();
		ce = m_Hydrax->getCausticsEnd();
		gre = m_Hydrax->getGodRaysExposure();
		gri = m_Hydrax->getGodRaysIntensity();
		ucsd = m_Hydrax->getUnderwaterCameraSwitchDelta();
		v = m_Hydrax->isVisible();

		delete m_Hydrax;
		m_Hydrax = NULL;
	}

	m_Hydrax = new Hydrax::Hydrax(m_Holder->GetSceneManager(), camera, Viewport);
	camera = NULL;
	
	CreateModule();

	m_Hydrax->create();

	if (isThereOldOne)
	{
		m_Hydrax->setComponents(components);
		m_Hydrax->setPolygonMode(mp);
		m_Hydrax->setShaderMode(sm);
		m_Hydrax->setPosition(pos);
		m_Hydrax->setFullReflectionDistance(frd);
		m_Hydrax->setGlobalTransparency(gt);
		m_Hydrax->setSunPosition(sp);
		m_Hydrax->setWaterColor(wc);
		m_Hydrax->setNormalDistortion(nd);
		m_Hydrax->setSunStrength(ss);
		m_Hydrax->setSunArea(sa);
		m_Hydrax->setSunColor(sc);
		m_Hydrax->setFoamMaxDistance(fmd);
		m_Hydrax->setFoamScale(fs);
		m_Hydrax->setFoamStart(fStart);
		m_Hydrax->setFoamTransparency(ft);
		m_Hydrax->setDepthLimit(dl);
		m_Hydrax->setSmoothPower(sPower);
		m_Hydrax->setCausticsScale(cs);
		m_Hydrax->setCausticsPower(cp);
		m_Hydrax->setCausticsEnd(ce);
		m_Hydrax->setGodRaysExposure(gre);
		m_Hydrax->setGodRaysIntensity(gri);
		m_Hydrax->setUnderwaterCameraSwitchDelta(ucsd);
		m_Hydrax->setVisible(v);
	}
}

#ifdef EDIT_MODE
bool OceanGameObject::Serialize(Attributes *Attributes)
{
	GameObject::Serialize(Attributes);
	
	Attributes->AddInteger("ModuleType", GetModuleType());
	Attributes->AddInteger("NoiseType", GetNoiseType());
	Attributes->AddInteger("NormalMode", GetNormalMode());

	switch (m_NoiseType)
	{
	case NT_PERLIN:
		{
			Attributes->AddInteger("Octaves", m_PerlinOption.Octaves);
			Attributes->AddFloat("PerlinScale", m_PerlinOption.Scale);
			Attributes->AddFloat("Falloff", m_PerlinOption.Falloff);
			Attributes->AddFloat("Animspeed", m_PerlinOption.Animspeed);
			Attributes->AddFloat("Timemulti", m_PerlinOption.Timemulti);
		} break;

	case NT_FFT:
		{
			Attributes->AddInteger("Resolution", m_FFTOption.Resolution);
			Attributes->AddFloat("PhysicalResolution", m_FFTOption.PhysicalResolution);
			Attributes->AddFloat("FFTScale", m_FFTOption.Scale);
			Attributes->AddFloat("WindDirectionX", m_FFTOption.WindDirection.x);
			Attributes->AddFloat("WindDirectionY", m_FFTOption.WindDirection.y);
			Attributes->AddFloat("AnimationSpeed", m_FFTOption.AnimationSpeed);
			Attributes->AddFloat("KwPower", m_FFTOption.KwPower);
			Attributes->AddFloat("Amplitude", m_FFTOption.Amplitude);
		} break;
	}

	switch (m_ModuleType)
	{
	case MT_SIMPLE_GRID:
		{
			Attributes->AddInteger("Complexity", m_SimpleGridOption.Complexity);
			Attributes->AddInteger("MeshSizeW", m_SimpleGridOption.MeshSize.Width);
			Attributes->AddInteger("MeshSizeH", m_SimpleGridOption.MeshSize.Height);
			Attributes->AddFloat("Strength", m_SimpleGridOption.Strength);
			Attributes->AddBoolean("Smooth", m_SimpleGridOption.Smooth);
			Attributes->AddBoolean("ChoppyWaves", m_SimpleGridOption.ChoppyWaves);
			Attributes->AddFloat("ChoppyStrength", m_SimpleGridOption.ChoppyStrength);
		} break;

	case MT_RADIAL_GRID:
		{
			Attributes->AddInteger("Steps", m_RadialGridOption.Steps);
			Attributes->AddInteger("Circles", m_RadialGridOption.Circles);
			Attributes->AddInteger("Radius", m_RadialGridOption.Radius);
			Attributes->AddBoolean("Smooth", m_RadialGridOption.Smooth);
			Attributes->AddBoolean("ChoppyWaves", m_RadialGridOption.ChoppyWaves);
			Attributes->AddFloat("ChoppyStrength", m_RadialGridOption.ChoppyStrength);
			Attributes->AddFloat("StepSizeCube", m_RadialGridOption.StepSizeCube);
			Attributes->AddFloat("StepSizeFive", m_RadialGridOption.StepSizeFive);
			Attributes->AddFloat("StepSizeLin", m_RadialGridOption.StepSizeLin);
			Attributes->AddFloat("Strength", m_RadialGridOption.Strength);
		} break;

	case MT_PROJECTED_GRID:
		{
			Attributes->AddInteger("Complexity", m_ProjectedGridOption.Complexity);
			Attributes->AddFloat("Strength", m_ProjectedGridOption.Strength);
			Attributes->AddFloat("Elevation", m_ProjectedGridOption.Elevation);
			Attributes->AddBoolean("Smooth", m_ProjectedGridOption.Smooth);
			Attributes->AddBoolean("ForceRecalculateGeometry", m_ProjectedGridOption.ForceRecalculateGeometry);
			Attributes->AddBoolean("ChoppyWaves", m_ProjectedGridOption.ChoppyWaves);
			Attributes->AddFloat("ChoppyStrength", m_ProjectedGridOption.ChoppyStrength);
		} break;
	}


	Attributes->AddInteger("PolygonMode", GetPolygonMode());
	Attributes->AddInteger("ShaderMode", GetShaderMode());
	Attributes->AddFloat("FullReflectionDistance", GetFullReflectionDistance());
	Attributes->AddFloat("GlobalTransparency", GetGlobalTransparency());
	Attributes->AddColor("WaterColor", GetWaterColor());
	Attributes->AddFloat("NormalDistortion", GetNormalDistortion());
	Attributes->AddFloat("DepthLimit", GetDepthLimit());

	Attributes->AddBoolean("Sun", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_SUN));
	Attributes->AddBoolean("Foam", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_FOAM));
	Attributes->AddBoolean("Depth", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_DEPTH));
	Attributes->AddBoolean("Smooth", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_SMOOTH));
	Attributes->AddBoolean("Caustics", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_CAUSTICS));
	Attributes->AddBoolean("UnderWater", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER));
	Attributes->AddBoolean("UnderWaterreflections", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS));
	Attributes->AddBoolean("UnderWaterGodRays", IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS));
			
	Attributes->AddFloat("FoamMaxDistance", GetFoamMaxDistance());
	Attributes->AddFloat("FoamScale", GetFoamScale());
	Attributes->AddFloat("FoamStart", GetFoamStart());
	Attributes->AddFloat("FoamTransparency", GetFoamTransparency());
			
	Attributes->AddFloat("SmoothPower", GetSmoothPower());
	Attributes->AddFloat("CausticsScale", GetCausticsScale());
	Attributes->AddFloat("CausticsPower", GetCausticsPower());
	Attributes->AddFloat("CausticsEnd", GetCausticsEnd());
				
	Attributes->AddColor("GodRaysExposure", GetGodRaysExposure());
	Attributes->AddFloat("GodRaysIntensity", GetGodRaysIntensity());
	Attributes->AddFloat("UnderwaterCameraSwitchDelta", GetUnderwaterCameraSwitchDelta());

	return true;
}
#endif

bool OceanGameObject::Deserialize(Attributes *Attributes)
{	
	m_ModuleType = (ModuleType)Attributes->GetInteger("ModuleType");
	m_NoiseType = (NoiseType)Attributes->GetInteger("NoiseType");
	m_NormalMode = (Hydrax::MaterialManager::NormalMode)Attributes->GetInteger("NormalMode");

	switch (m_NoiseType)
	{
	case NT_PERLIN:
		{
			m_PerlinOption.Octaves = Attributes->GetInteger("Octaves");
			m_PerlinOption.Scale = Attributes->GetFloat("PerlinScale");
			m_PerlinOption.Falloff = Attributes->GetFloat("Falloff");
			m_PerlinOption.Animspeed = Attributes->GetFloat("Animspeed");
			m_PerlinOption.Timemulti = Attributes->GetFloat("Timemulti");
		} break;

	case NT_FFT:
		{
			m_FFTOption.Resolution = Attributes->GetInteger("Resolution");
			m_FFTOption.PhysicalResolution = Attributes->GetFloat("PhysicalResolution");
			m_FFTOption.Scale = Attributes->GetFloat("FFTScale");
			m_FFTOption.WindDirection.x = Attributes->GetFloat("WindDirectionX");
			m_FFTOption.WindDirection.y = Attributes->GetFloat("WindDirectionY");
			m_FFTOption.AnimationSpeed = Attributes->GetFloat("AnimationSpeed");
			m_FFTOption.KwPower = Attributes->GetFloat("KwPower");
			m_FFTOption.Amplitude = Attributes->GetFloat("Amplitude");
		} break;
	}

	switch (m_ModuleType)
	{
	case MT_SIMPLE_GRID:
		{
			m_SimpleGridOption.Complexity = Attributes->GetInteger("Complexity");
			m_SimpleGridOption.MeshSize.Width = Attributes->GetInteger("MeshSizeW");
			m_SimpleGridOption.MeshSize.Height = Attributes->GetInteger("MeshSizeH");
			m_SimpleGridOption.Strength = Attributes->GetFloat("Strength");
			m_SimpleGridOption.Smooth = Attributes->GetBoolean("Smooth");
			m_SimpleGridOption.ChoppyWaves = Attributes->GetBoolean("ChoppyWaves");
			m_SimpleGridOption.ChoppyStrength = Attributes->GetFloat("ChoppyStrength");
		} break;

	case MT_RADIAL_GRID:
		{
			m_RadialGridOption.Steps = Attributes->GetInteger("Steps");
			m_RadialGridOption.Circles = Attributes->GetInteger("Circles");
			m_RadialGridOption.Radius = Attributes->GetInteger("Radius");
			m_RadialGridOption.Smooth = Attributes->GetBoolean("Smooth");
			m_RadialGridOption.ChoppyWaves = Attributes->GetBoolean("ChoppyWaves");
			m_RadialGridOption.ChoppyStrength = Attributes->GetFloat("ChoppyStrength");
			m_RadialGridOption.StepSizeCube = Attributes->GetFloat("StepSizeCube");
			m_RadialGridOption.StepSizeFive = Attributes->GetFloat("StepSizeFive");
			m_RadialGridOption.StepSizeLin = Attributes->GetFloat("StepSizeLin");
			m_RadialGridOption.Strength = Attributes->GetFloat("Strength");
		} break;

	case MT_PROJECTED_GRID:
		{
			m_ProjectedGridOption.Complexity = Attributes->GetInteger("Complexity");
			m_ProjectedGridOption.Strength = Attributes->GetFloat("Strength");
			m_ProjectedGridOption.Elevation = Attributes->GetFloat("Elevation");
			m_ProjectedGridOption.Smooth = Attributes->GetBoolean("Smooth");
			m_ProjectedGridOption.ForceRecalculateGeometry = Attributes->GetBoolean("ForceRecalculateGeometry");
			m_ProjectedGridOption.ChoppyWaves = Attributes->GetBoolean("ChoppyWaves");
			m_ProjectedGridOption.ChoppyStrength = Attributes->GetFloat("ChoppyStrength");
		} break;
	}

	CreateModule();

	SetPolygonMode((Ogre::PolygonMode)Attributes->GetInteger("PolygonMode"));
	SetShaderMode((Hydrax::MaterialManager::ShaderMode)Attributes->GetInteger("ShaderMode"));
	SetFullReflectionDistance(Attributes->GetFloat("FullReflectionDistance"));
	SetGlobalTransparency(Attributes->GetFloat("GlobalTransparency"));
	SetWaterColor(Attributes->GetColor("WaterColor"));
	SetNormalDistortion(Attributes->GetFloat("NormalDistortion"));
	SetDepthLimit(Attributes->GetFloat("DepthLimit"));

	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_SUN, Attributes->GetBoolean("Sun"));
	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_FOAM, Attributes->GetBoolean("Foam"));
	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_DEPTH, Attributes->GetBoolean("Depth"));
	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_SMOOTH, Attributes->GetBoolean("Smooth"));
	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_CAUSTICS, Attributes->GetBoolean("Caustics"));
	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER, Attributes->GetBoolean("UnderWater"));
	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS, Attributes->GetBoolean("UnderWaterreflections"));
	SetComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS, Attributes->GetBoolean("UnderWaterGodRays"));
			
	//SetSunPosition(Attributes->GetVector3("SunPosition"));
	//SetSunStrength(Attributes->GetFloat("SunStrength"));
	//SetSunArea(Attributes->GetFloat("SunArea"));
	//SetSunColor(Attributes->GetColor("SunColor"));
			
	SetFoamMaxDistance(Attributes->GetFloat("FoamMaxDistance"));
	SetFoamScale(Attributes->GetFloat("FoamScale"));
	SetFoamStart(Attributes->GetFloat("FoamStart"));
	SetFoamTransparency(Attributes->GetFloat("FoamTransparency"));
			
	SetSmoothPower(Attributes->GetFloat("SmoothPower"));
	SetCausticsScale(Attributes->GetFloat("CausticsScale"));
	SetCausticsPower(Attributes->GetFloat("CausticsPower"));
	SetCausticsEnd(Attributes->GetFloat("CausticsEnd"));
				
	SetGodRaysExposure(Attributes->GetColor("GodRaysExposure"));
	SetGodRaysIntensity(Attributes->GetFloat("GodRaysIntensity"));
	SetUnderwaterCameraSwitchDelta(Attributes->GetFloat("UnderwaterCameraSwitchDelta"));

	SetSunPosition(m_Holder->GetSunPosition());
	SetSunStrength(m_Holder->GetSunStrength());
	SetSunArea(m_Holder->GetSunArea());
	SetSunColor(m_Holder->GetSunColor());
	
	GameObject::Deserialize(Attributes);

	return true;
}

	
//void OceanGameObject::SetInitializeState(void)
//{
//	GameObject::SetInitializeState();
//}


//void OceanGameObject::ResetToInitializeState(void)
//{
//	GameObject::ResetToInitializeState();
//}


void OceanGameObject::SetComponentEnabled(const Hydrax::HydraxComponent &Component, bool Enabled)
{
	Hydrax::HydraxComponent components[8] = {
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_SUN) ? Hydrax::HYDRAX_COMPONENT_SUN : Hydrax::HYDRAX_COMPONENTS_NONE,
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_FOAM) ? Hydrax::HYDRAX_COMPONENT_FOAM : Hydrax::HYDRAX_COMPONENTS_NONE,
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_DEPTH) ? Hydrax::HYDRAX_COMPONENT_DEPTH : Hydrax::HYDRAX_COMPONENTS_NONE,
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_SMOOTH) ? Hydrax::HYDRAX_COMPONENT_SMOOTH : Hydrax::HYDRAX_COMPONENTS_NONE,
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_CAUSTICS) ? Hydrax::HYDRAX_COMPONENT_CAUSTICS : Hydrax::HYDRAX_COMPONENTS_NONE,
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER) ? Hydrax::HYDRAX_COMPONENT_UNDERWATER : Hydrax::HYDRAX_COMPONENTS_NONE,
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS) ? Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS : Hydrax::HYDRAX_COMPONENTS_NONE,
		IsComponentEnabled(Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS) ? Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS : Hydrax::HYDRAX_COMPONENTS_NONE };

	unsigned int index = 0;

	switch (Component)
	{
	case Hydrax::HYDRAX_COMPONENT_SUN: index = 0; break;
	case Hydrax::HYDRAX_COMPONENT_FOAM: index = 1; break;
	case Hydrax::HYDRAX_COMPONENT_DEPTH: index = 2; break;
	case Hydrax::HYDRAX_COMPONENT_SMOOTH: index = 3; break;
	case Hydrax::HYDRAX_COMPONENT_CAUSTICS: index = 4; break;
	case Hydrax::HYDRAX_COMPONENT_UNDERWATER: index = 5; break;
	case Hydrax::HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS: index = 6; break;
	case Hydrax::HYDRAX_COMPONENT_UNDERWATER_GODRAYS: index = 7; break;
	default: return;
	}

	components[index] = (Enabled ? Component : Hydrax::HYDRAX_COMPONENTS_NONE);

	m_Hydrax->setComponents(static_cast<Hydrax::HydraxComponent>(
		components[0] | components[1] | components[2] | components[3] |
		components[4] | components[5] | components[6] | components[7]));
}


void OceanGameObject::UpdateModuleOptionChanges(void)
{
	switch (m_ModuleType)
	{
	case MT_SIMPLE_GRID:
		{
			((Hydrax::Module::SimpleGrid*)m_Hydrax->getModule())->setOptions(m_SimpleGridOption);
		} break;

	case MT_RADIAL_GRID:
		{
			((Hydrax::Module::RadialGrid*)m_Hydrax->getModule())->setOptions(m_RadialGridOption);
		} break;

	case MT_PROJECTED_GRID:
		{
			((Hydrax::Module::ProjectedGrid*)m_Hydrax->getModule())->setOptions(m_ProjectedGridOption);
		} break;
	}
}

	
void OceanGameObject::UpdateNoiseOptionChanges(void)
{
	switch (m_NoiseType)
	{
	case NT_PERLIN:
		{
			((Hydrax::Noise::Perlin*)m_Hydrax->getModule()->getNoise())->setOptions(m_PerlinOption);
		} break;

	case NT_FFT:
		{
			((Hydrax::Noise::FFT*)m_Hydrax->getModule()->getNoise())->setOptions(m_FFTOption);
		} break;
	}
}


void OceanGameObject::CreateModule(void)
{
	Hydrax::Module::Module *module = NULL;
	Hydrax::Noise::Noise *noise = NULL;

	switch (m_NoiseType)
	{
	case NT_PERLIN:
		{
			noise = new Hydrax::Noise::Perlin(m_PerlinOption);
		} break;

	case NT_FFT:
		{
			noise = new Hydrax::Noise::FFT(m_FFTOption);
		} break;
	}

	switch (m_ModuleType)
	{
	case MT_SIMPLE_GRID:
		{
			module = new Hydrax::Module::SimpleGrid(m_Hydrax, noise, m_NormalMode, m_SimpleGridOption);
		} break;
		
	case MT_RADIAL_GRID:
		{
			module = new Hydrax::Module::RadialGrid(m_Hydrax, noise, m_NormalMode, m_RadialGridOption);
		} break;
		
	case MT_PROJECTED_GRID:
		{
			module = new Hydrax::Module::ProjectedGrid(m_Hydrax, noise, Ogre::Plane(Ogre::Vector3::UNIT_Y, Ogre::Vector3::ZERO), m_NormalMode, m_ProjectedGridOption);
		} break;
	}
	
	m_Hydrax->setModule(module);

    //m_Hydrax->create();

	module = NULL;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(OceanGameObject)
	BEGIN_DEFINE_TYPE_BASE(GameObject, OceanGameObject, "OceanGameObject")
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE