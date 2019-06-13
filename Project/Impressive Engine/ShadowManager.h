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
class Scene;

class ShadowManager
{
public:	
	enum ShadowType
	{
		ST_NONE = Ogre::SHADOWTYPE_NONE,
		ST_TEXTURE = Ogre::SHADOWTYPE_TEXTURE_ADDITIVE ,
		ST_STENCIL = Ogre::SHADOWTYPE_STENCIL_ADDITIVE
	};

	enum ShadowCameraSetupType
	{
		SCST_DEFAULT = 0,
		SCST_FOCUSED,
		SCST_LISPSM
		//SCST_PLANEOPTIMAL
	};

	enum PixelFormat
	{
		/// 32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue
		/// like PF_A8R8G8B8, but alpha will get discarded
		PF_8BIT = Ogre::PF_X8R8G8B8
	};

	ShadowManager(Scene *Scene);
	//~ShadowManager(void);

	void SetShadowType(const ShadowType &Type);
	const ShadowType GetShadowType(void);

	void SetShadowCameraSetupType(const ShadowCameraSetupType &Type);
	ShadowCameraSetupType GetShadowCameraSetupType(void);

	void SetShadowTexturePixelFormat(const PixelFormat &Format);
	const PixelFormat GetShadowTexturePixelFormat(void);

	void SetShadowTextureSelfShadow(bool Value);
	bool GetShadowTextureSelfShadow(void);

	void SetShadowTextureSettings(const unsigned int &Size, const unsigned int &Count, const PixelFormat &Format = PF_8BIT);

	void SetShadowTextureCount(const unsigned int &Count);
	const unsigned int GetShadowTextureCount(void);

	void SetShadowTextureSize(const unsigned short &Size);
	const unsigned int GetShadowTextureSize(void);

	void SetShadowTextureReceiverMaterial(const Ogre::String &Name);
	void SetShadowTextureCasterMaterial(const Ogre::String &Name);

	void SetShadowDirectionalLightExtrusionDistance(const float &Distance);
	const float GetShadowDirectionalLightExtrusionDistance(void);

	void SetShadowFarDistance(const float &Distance);
	const float GetShadowFarDistance(void);

	void SetShadowDirLightTextureOffset(const float &Offset);
	const float GetShadowDirLightTextureOffset(void);

	void SetShadowCasterRenderBackFaces(bool Value);
	bool GetShadowCasterRenderBackFaces(void);

	void SetShadowColor(const Ogre::ColourValue &Color);
	const Ogre::ColourValue &GetShadowColor();

	void SetShadowIndexBufferSize(const unsigned int &Size);
	const unsigned int GetShadowIndexBufferSize();

	void SetShadowTextureFadeEnd(const float &FadeEnd);
	const float &GetShadowTextureFadeEnd();

	void SetShadowTextureFadeStart(const float &FadeStart);
	const float &GetShadowTextureFadeStart();

	void SetShadowUseInfiniteFarPlane(bool Enable);
	bool GetShadowUseInfiniteFarPlane();

	void SetShadowUseLightClipPlanes(bool Enable);
	bool GetShadowUseLightClipPlanes();
	//Ogre::ShadowCameraSetupPtr *m_CurrentShadowCameraSetup;
private:
	bool m_ShadowUseInfiniteFarPlane;
	float m_ShadowTextureFadeEnd;
	float m_ShadowTextureFadeStart;
	ShadowCameraSetupType m_CurrentShadowCameraSetupType;
	Scene *m_Scene;
};
END_NAMESPACE