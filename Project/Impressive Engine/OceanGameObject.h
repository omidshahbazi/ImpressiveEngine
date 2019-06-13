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


BEGIN_NAMESPACE
class OceanGameObject : public GameObject
{
public:
	enum ModuleType
	{
		MT_SIMPLE_GRID = 0,
		MT_RADIAL_GRID,
		MT_PROJECTED_GRID
	};
	
	enum NoiseType
	{
		NT_PERLIN = 0,
		NT_FFT
	};

public:
	OceanGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent = NULL);
	~OceanGameObject(void);

	bool Update(void);

	void OnViewportChanges(Ogre::Viewport *Viewport);
	
#ifdef EDIT_MODE
	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);
	
	//void SetInitializeState(void);
	//void ResetToInitializeState(void);

	//void SetComponents(const HydraxComponent &Components);

	const Ogre::Vector3 &GetPosition(void)
	{
		return m_Hydrax->getPosition();
	}

	void SetPosition(const Ogre::Vector3 &Position)
	{
		m_Hydrax->setPosition(Position);
	}
	
	void SetPosition(float X, float Y, float Z)
	{
		SetPosition(Ogre::Vector3(X, Y, Z));
	}

	void SetScale(const Ogre::Vector3 &Scale)
	{
		// Do Nothing
	}
	
	void SetScale(float X, float Y, float Z)
	{
		// Do Nothing
	}

	void Pitch(float Value)
	{
		// Do Nothing
	}

	void Yaw(float Value)
	{
		// Do Nothing
	}

	void Roll(float Value)
	{
		// Do Nothing
	}
	
	void SetRotation(float X, float Y, float Z)
	{
		// Do Nothing
	}
	
	void SetRotation(const Ogre::Vector3 &Rotation)
	{
		// Do Nothing
	}
	
	void SetOrientation(const Ogre::Quaternion &Quaternion)
	{
		// Do Nothing
	}

	void SetOrientation(float W, float X, float Y, float Z)
	{
		// Do Nothing
	}

	const Ogre::AxisAlignedBox &GetWorldAABB(void) const
	{
		return m_WorldAABB;
	}

	bool IsComponentEnabled(const Hydrax::HydraxComponent &Component)
	{
		return m_Hydrax->isComponent(Component);
	}

	void SetComponentEnabled(const Hydrax::HydraxComponent &Component, bool Enabled);

	bool GetVisible(void)
	{
		return m_Hydrax->isVisible();
	}

	void SetVisible(bool Value)
	{
		GameObject::SetVisible(Value);

		m_Hydrax->setVisible(Value);
	}

	const Ogre::PolygonMode &GetPolygonMode(void)
	{
		return m_Hydrax->getPolygonMode();
	}

	void SetPolygonMode(const Ogre::PolygonMode &Mode)
	{
		m_Hydrax->setPolygonMode(Mode);
	}

	const Hydrax::MaterialManager::ShaderMode &GetShaderMode(void)
	{
		return m_Hydrax->getShaderMode();
	}

	void SetShaderMode(const Hydrax::MaterialManager::ShaderMode &Mode)
	{
		m_Hydrax->setShaderMode(Mode);
	}

	const float &GetFullReflectionDistance(void)
	{
		return m_Hydrax->getFullReflectionDistance();
	}

	void SetFullReflectionDistance(const float &FullReflectionDistance)
	{
		m_Hydrax->setFullReflectionDistance(FullReflectionDistance);
	}

	const float &GetGlobalTransparency(void)
	{
		return m_Hydrax->getGlobalTransparency();
	}

	void SetGlobalTransparency(const float &GlobalTransparency)
	{
		m_Hydrax->setGlobalTransparency(GlobalTransparency);
	}

	const Ogre::ColourValue GetWaterColor(void)
	{
		return Ogre::ColourValue(m_Hydrax->getWaterColor().x, m_Hydrax->getWaterColor().y, m_Hydrax->getWaterColor().z); 
	}

	void SetWaterColor(const Ogre::ColourValue &SunColor)
	{
		SetWaterColor(SunColor.r, SunColor.g, SunColor.b);
	}

	void SetWaterColor(const float &R, const float &G, const float &B)
	{
		m_Hydrax->setWaterColor(Ogre::Vector3(R, G, B));
	}

	const float &GetNormalDistortion(void)
	{
		return m_Hydrax->getNormalDistortion();
	}

	void SetNormalDistortion(const float &NormalDistortion)
	{
		m_Hydrax->setNormalDistortion(NormalDistortion);
	}

	//const Ogre::Vector3 &GetSunPosition(void)
	//{
	//	return m_Hydrax->getSunPosition();
	//}

	void SetSunPosition(const Ogre::Vector3 &SunPosition)
	{
		m_Hydrax->setSunPosition(SunPosition);
	}

	//const float &GetSunStrength(void)
	//{
	//	return m_Hydrax->getSunStrength();
	//}

	void SetSunStrength(const float &SunStrength)
	{
		m_Hydrax->setSunStrength(SunStrength);
	}

	//const float &GetSunArea(void)
	//{
	//	return m_Hydrax->getSunArea();
	//}

	void SetSunArea(const float &SunArea)
	{
		m_Hydrax->setSunArea(SunArea);
	}

	//const Ogre::ColourValue GetSunColor(void)
	//{
	//	return Ogre::ColourValue(m_Hydrax->getSunColor().x, m_Hydrax->getSunColor().y, m_Hydrax->getSunColor().z); 
	//}

	void SetSunColor(const Ogre::ColourValue &SunColor)
	{
		SetSunColor(SunColor.r, SunColor.g, SunColor.b);
	}

	void SetSunColor(const float &R, const float &G, const float &B)
	{
		m_Hydrax->setSunColor(Ogre::Vector3(R, G, B));
	}

	const float &GetFoamMaxDistance(void)
	{
		return m_Hydrax->getFoamMaxDistance();
	}

	void SetFoamMaxDistance(const float &FoamMaxDistance)
	{
		m_Hydrax->setFoamMaxDistance(FoamMaxDistance);
	}

	const float &GetFoamScale(void)
	{
		return m_Hydrax->getFoamScale();
	}

	void SetFoamScale(const float &FoamScale)
	{
		m_Hydrax->setFoamScale(FoamScale);
	}

	const float &GetFoamStart(void)
	{
		return m_Hydrax->getFoamStart();
	}

	void SetFoamStart(const float &FoamStart)
	{
		m_Hydrax->setFoamStart(FoamStart);
	}

	const float &GetFoamTransparency(void)
	{
		return m_Hydrax->getFoamTransparency();
	}

	void SetFoamTransparency(const float &FoamTransparency)
	{
		m_Hydrax->setFoamTransparency(FoamTransparency);
	}

	const float &GetDepthLimit(void)
	{
		return m_Hydrax->getDepthLimit();
	}

	void SetDepthLimit(const float &DepthLimit)
	{
		m_Hydrax->setDepthLimit(DepthLimit);
	}

	const float &GetSmoothPower(void)
	{
		return m_Hydrax->getSmoothPower();
	}

	void SetSmoothPower(const float &SmoothPower)
	{
		m_Hydrax->setSmoothPower(SmoothPower);
	}

	const float &GetCausticsScale(void)
	{
		return m_Hydrax->getCausticsScale();
	}

	void SetCausticsScale(const float &CausticsScale)
	{
		m_Hydrax->setCausticsScale(CausticsScale);
	}

	const float &GetCausticsPower(void)
	{
		return m_Hydrax->getCausticsPower();
	}

	void SetCausticsPower(const float &CausticsPower)
	{
		m_Hydrax->setCausticsPower(CausticsPower);
	}

	const float &GetCausticsEnd(void)
	{
		return m_Hydrax->getCausticsEnd();
	}

	void SetCausticsEnd(const float &CausticsEnd)
	{
		m_Hydrax->setCausticsEnd(CausticsEnd);
	}

	const Ogre::ColourValue GetGodRaysExposure(void)
	{
		return Ogre::ColourValue(m_Hydrax->getGodRaysExposure().x, m_Hydrax->getGodRaysExposure().y, m_Hydrax->getGodRaysExposure().z); 
	}

	void SetGodRaysExposure(const Ogre::ColourValue &SunColor)
	{
		SetGodRaysExposure(SunColor.r, SunColor.g, SunColor.b);
	}

	void SetGodRaysExposure(const float &R, const float &G, const float &B)
	{
		m_Hydrax->setGodRaysExposure(Ogre::Vector3(R, G, B));
	}

	const float &GetGodRaysIntensity(void)
	{
		return m_Hydrax->getGodRaysIntensity();
	}

	void SetGodRaysIntensity(const float &GodRaysIntensity)
	{
		m_Hydrax->setGodRaysIntensity(GodRaysIntensity);
	}

	const float &GetUnderwaterCameraSwitchDelta(void)
	{
		return m_Hydrax->getUnderwaterCameraSwitchDelta();
	}

	void SetUnderwaterCameraSwitchDelta(const float &UnderwaterCameraSwitchDelta)
	{
		m_Hydrax->setUnderwaterCameraSwitchDelta(UnderwaterCameraSwitchDelta);
	}

	Hydrax::Module::SimpleGrid::Options &GetSimpleGridOption(void)
	{
		return m_SimpleGridOption;
	}

	Hydrax::Module::RadialGrid::Options &GetRadialGridOption(void)
	{
		return m_RadialGridOption;
	}

	Hydrax::Module::ProjectedGrid::Options &GetProjectedGridOption(void)
	{
		return m_ProjectedGridOption;
	}

	void UpdateModuleOptionChanges(void);

	Hydrax::Noise::Perlin::Options &GetPerlinOption(void)
	{
		return m_PerlinOption;
	}

	Hydrax::Noise::FFT::Options &GetFFTOption(void)
	{
		return m_FFTOption;
	}
	
	void UpdateNoiseOptionChanges(void);

	const ModuleType &GetModuleType(void)
	{
		return m_ModuleType;
	}

	void SetModuleType(const ModuleType &ModuleType)
	{
		m_ModuleType = ModuleType;

		CreateModule();
	}

	const NoiseType &GetNoiseType(void)
	{
		return m_NoiseType;
	}

	void SetNoiseType(const NoiseType &NoiseType)
	{
		m_NoiseType = NoiseType;

		CreateModule();
	}

	const Hydrax::MaterialManager::NormalMode &GetNormalMode(void)
	{
		return m_NormalMode;
	}

	void SetNormalMode(const Hydrax::MaterialManager::NormalMode &NormalMode)
	{
		m_NormalMode = NormalMode;

		CreateModule();
	}

	GameObjectType GetType(void) const
	{
		return GOT_OCEAN;
	}

private:
	void CreateModule(void);

public:
	DECLARE_SCRIPT_REGISTATION()
	
private:
	Hydrax::Hydrax *m_Hydrax;

	Hydrax::Module::SimpleGrid::Options m_SimpleGridOption;
	Hydrax::Module::RadialGrid::Options m_RadialGridOption;
	Hydrax::Module::ProjectedGrid::Options m_ProjectedGridOption;

	Hydrax::Noise::Perlin::Options m_PerlinOption;
	Hydrax::Noise::FFT::Options m_FFTOption;

	ModuleType m_ModuleType;
	NoiseType m_NoiseType;
	Hydrax::MaterialManager::NormalMode m_NormalMode;

	Ogre::AxisAlignedBox m_WorldAABB;
};
END_NAMESPACE