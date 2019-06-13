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
#include "ShadowManager.h"
#include "Scene.h"

BEGIN_NAMESPACE
ShadowManager::ShadowManager(Scene *Scene) :
	m_Scene(Scene),
	m_CurrentShadowCameraSetupType(SCST_DEFAULT),
	m_ShadowTextureFadeEnd(float(0.9)),
	m_ShadowTextureFadeStart(float(0.6)),
	m_ShadowUseInfiniteFarPlane(false)
{
	SetShadowUseInfiniteFarPlane(false);
	SetShadowType(ST_NONE);
	SetShadowTexturePixelFormat(PF_8BIT);
}


//ShadowManager::~ShadowManager(void)
//{
//
//}


void ShadowManager::SetShadowType(const ShadowType &Type)
{
	m_Scene->GetSceneManager()->setShadowTechnique((Ogre::ShadowTechnique)Type);
}


const ShadowManager::ShadowType ShadowManager::GetShadowType(void)
{
	return (ShadowType)m_Scene->GetSceneManager()->getShadowTechnique();
}


void ShadowManager::SetShadowCameraSetupType(const ShadowCameraSetupType &Type)
{
	switch (Type)
	{
	case SCST_DEFAULT:
		m_Scene->GetSceneManager()->setShadowCameraSetup((Ogre::ShadowCameraSetupPtr) (new Ogre::DefaultShadowCameraSetup()));
		break;
	case SCST_FOCUSED:
		m_Scene->GetSceneManager()->setShadowCameraSetup((Ogre::ShadowCameraSetupPtr) (new Ogre::FocusedShadowCameraSetup()));
		break;
	case SCST_LISPSM:
		m_Scene->GetSceneManager()->setShadowCameraSetup((Ogre::ShadowCameraSetupPtr) (new Ogre::LiSPSMShadowCameraSetup()));
		break;
	}

	m_CurrentShadowCameraSetupType = Type;
}


ShadowManager::ShadowCameraSetupType ShadowManager::GetShadowCameraSetupType(void)
{
	return m_CurrentShadowCameraSetupType;
}


void ShadowManager::SetShadowTexturePixelFormat(const PixelFormat &Format)
{
	m_Scene->GetSceneManager()->setShadowTexturePixelFormat((Ogre::PixelFormat)Format);
}


const ShadowManager::PixelFormat ShadowManager::GetShadowTexturePixelFormat(void)
{
	return (PixelFormat) m_Scene->GetSceneManager()->getShadowTextureConfigIterator().begin()->format;
}


void ShadowManager::SetShadowTextureSelfShadow(bool Value)
{
	m_Scene->GetSceneManager()->setShadowTextureSelfShadow(Value);
}


bool ShadowManager::GetShadowTextureSelfShadow(void)
{
	return m_Scene->GetSceneManager()->getShadowTextureSelfShadow();
}


void ShadowManager::SetShadowTextureSettings(const unsigned int &Size, const unsigned int &Count, const PixelFormat &Format)
{
	m_Scene->GetSceneManager()->setShadowTextureSettings(Size, Count, (Ogre::PixelFormat)Format);
}


void ShadowManager::SetShadowTextureSize(const unsigned short &Size)
{
	m_Scene->GetSceneManager()->setShadowTextureSize(Size);
}


const unsigned int ShadowManager::GetShadowTextureSize(void)
{
	return m_Scene->GetSceneManager()->getShadowTextureConfigIterator().begin()->width;
}


void ShadowManager::SetShadowTextureCount(const unsigned int &Count)
{
	m_Scene->GetSceneManager()->setShadowTextureCount(Count);
}


const unsigned int ShadowManager::GetShadowTextureCount(void)
{
	return m_Scene->GetSceneManager()->getShadowTextureCount();

}


void ShadowManager::SetShadowTextureReceiverMaterial(const Ogre::String &Name)
{
	m_Scene->GetSceneManager()->setShadowTextureReceiverMaterial(Name);
}


void ShadowManager::SetShadowTextureCasterMaterial(const Ogre::String &Name)
{
	m_Scene->GetSceneManager()->setShadowTextureCasterMaterial(Name);
}


void ShadowManager::SetShadowDirectionalLightExtrusionDistance(const float &Distance)
{
	m_Scene->GetSceneManager()->setShadowDirectionalLightExtrusionDistance(Distance);
}


const float ShadowManager::GetShadowDirectionalLightExtrusionDistance(void)
{
	return m_Scene->GetSceneManager()->getShadowDirectionalLightExtrusionDistance();
}


const float ShadowManager::GetShadowFarDistance(void)
{
	return m_Scene->GetSceneManager()->getShadowFarDistance();
}


void ShadowManager::SetShadowFarDistance(const float &Distance)
{
	m_Scene->GetSceneManager()->setShadowFarDistance(Distance);
}


void ShadowManager::SetShadowDirLightTextureOffset(const float &Offset)
{
	m_Scene->GetSceneManager()->setShadowDirLightTextureOffset(Offset);
}


const float ShadowManager::GetShadowDirLightTextureOffset(void)
{
	return m_Scene->GetSceneManager()->getShadowDirLightTextureOffset();
}


void ShadowManager::SetShadowCasterRenderBackFaces(bool Value)
{
	m_Scene->GetSceneManager()->setShadowCasterRenderBackFaces(Value);
}


bool ShadowManager::GetShadowCasterRenderBackFaces(void)
{
	return m_Scene->GetSceneManager()->getShadowCasterRenderBackFaces();
}


void ShadowManager::SetShadowColor(const Ogre::ColourValue &Color)
{
	m_Scene->GetSceneManager()->setShadowColour(Color);
}


const Ogre::ColourValue &ShadowManager::GetShadowColor()
{
	return m_Scene->GetSceneManager()->getShadowColour();
}


void ShadowManager::SetShadowIndexBufferSize(const unsigned int &Size)
{
	m_Scene->GetSceneManager()->setShadowIndexBufferSize(Size);
}


const unsigned int ShadowManager::GetShadowIndexBufferSize()
{
	return m_Scene->GetSceneManager()->getShadowIndexBufferSize();
}


void ShadowManager::SetShadowTextureFadeEnd(const float &FadeEnd)
{
	m_Scene->GetSceneManager()->setShadowTextureFadeEnd(FadeEnd);
	m_ShadowTextureFadeEnd = FadeEnd;
}


const float &ShadowManager::GetShadowTextureFadeEnd()
{
	return m_ShadowTextureFadeEnd;
}


void ShadowManager::SetShadowTextureFadeStart(const float &FadeStart)
{
	m_Scene->GetSceneManager()->setShadowTextureFadeStart(FadeStart);
	m_ShadowTextureFadeStart = FadeStart;
}


const float &ShadowManager::GetShadowTextureFadeStart()
{
	return m_ShadowTextureFadeStart;
}


void ShadowManager::SetShadowUseInfiniteFarPlane(bool Enable)
{
	m_Scene->GetSceneManager()->setShadowUseInfiniteFarPlane(Enable);
	m_ShadowUseInfiniteFarPlane = Enable;
}


bool ShadowManager::GetShadowUseInfiniteFarPlane()
{
	return m_ShadowUseInfiniteFarPlane;
}


void ShadowManager::SetShadowUseLightClipPlanes(bool Enable)
{
	m_Scene->GetSceneManager()->setShadowUseLightClipPlanes(Enable);
}


bool ShadowManager::GetShadowUseLightClipPlanes()
{
	return m_Scene->GetSceneManager()->getShadowUseLightClipPlanes();
}
END_NAMESPACE