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
#ifdef EDIT_MODE

#pragma once

#include "NodeBase.h"
#include "ISerializable.h"

BEGIN_NAMESPACE
class EditorCamera : /*public CameraBase, public NodeBase,*/ public ISerializable
{
public:
	enum EditorCameraType
	{
		ECT_FLY = 0,
		ECT_TOP,
		ECT_LEFT,
		ECT_RIGHT,
		ECT_FRONT,
		ECT_COUNT
	};

public:
	EditorCamera(EditorCameraType Type = ECT_FLY, Ogre::Camera *Camera = NULL);
	//~EditorCamera(void);

	//void Update(void);

	bool Serialize(Attributes *Attributes);
	bool Deserialize(Attributes *Attributes);
	
	void ApplyTranslate(const Ogre::Vector3 &Translation);

	EditorCameraType GetType(void)
	{
		return m_Type;
	}

	//void SetActive(bool Value)
	//{
	//	m_IsActive = Value;
	//}
	

	const Ogre::String &GetName(void)
	{
		return m_Camera->getName();
	}

	const Ogre::Vector3 &GetPosition(void)
	{
		return m_Camera->getPosition();
	}

	void SetPosition(const Ogre::Vector3& Position)
	{
		m_Camera->setPosition(Position);
	}
	
	void SetPosition(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_Camera->setPosition(X, Y, Z);
	}

	void LookAt(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_Camera->lookAt(X, Y, Z);
	}

	void LookAt(const Ogre::Vector3 &Target)
	{
		m_Camera->lookAt(Target);
	}

	void Pitch(const Ogre::Radian &Angle)
	{
		m_Camera->pitch(Angle);
	}

	void Yaw(const Ogre::Radian &Angle)
	{
		m_Camera->yaw(Angle);
	}

	void Roll(const Ogre::Radian &Angle)
	{
		m_Camera->roll(Angle);
	}

	const Ogre::Ray &GetCameraToViewportRay(Ogre::Real X, Ogre::Real Y)
	{
		m_ViewportRay = m_Camera->getCameraToViewportRay(X, Y);

		return m_ViewportRay;
	}

	Ogre::PolygonMode GetPolygonMode(void)
	{
		return m_Camera->getPolygonMode();
	}

	void SetPolygonMode(Ogre::PolygonMode Mode)
	{
		m_Camera->setPolygonMode(Mode);
	}

	void UpdateRenderWindowSize(unsigned int Width, unsigned int Height, bool UpdateOrtho = false)
	{
		m_Camera->setAspectRatio((float)Width / Height);

		if (UpdateOrtho && m_Type != EditorCamera::ECT_FLY)
			m_Camera->setOrthoWindowWidth(Width);
	}

	Ogre::Camera *_GetCamera(void)
	{
		return m_Camera;
	}

private:
	void SetType(EditorCameraType Type);

private:
	//bool m_IsActive;

	float m_MoveSpeed;
	
	//bool m_RightPressedDown;
	//Ogre::Vector2 m_LastMouseDownPosition;
	
	Ogre::Camera *m_Camera;
	EditorCameraType m_Type;

	Ogre::Ray m_ViewportRay;
};

END_NAMESPACE
#endif