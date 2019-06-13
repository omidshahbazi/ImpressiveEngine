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


#include "EditorCamera.h"
#include "ScenesHolder.h"
#include "Engine.h"


BEGIN_NAMESPACE
Ogre::String BuiltIn_TypeNames[] = { "Fly", "Top", "Left", "Right", "Front" };


EditorCamera::EditorCamera(EditorCameraType Type, Ogre::Camera *Camera) :
	//CameraBase(BuiltIn_TypeNames[(int)Type]),
	//m_IsActive(false),
	m_MoveSpeed(0.75f)//,
	//m_RightPressedDown(false)
{
	if (Camera)
		m_Camera = Camera;
	else
		m_Camera = ScenesHolder::GetInstance().GetCurrentScene()->GetSceneManager()->createCamera(BuiltIn_TypeNames[(int)Type]);

	m_Camera->setNearClipDistance(0.01f);

	//if (Engine::GetInstance().GetGraphic()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	//	m_Camera->setFarClipDistance(0);
	//else
	
	// Comment of 00 is for DeferredShadingSystem
		m_Camera->setFarClipDistance(100000/*00*/);

	SetType(Type);
}


//EditorCamera::~EditorCamera(void)
//{
//}


//void EditorCamera::Update(void)
//{
//	if (m_IsActive)
//	{
//		const HID &hid = HID::GetInstance();
//		const bool inMove = hid.IsMouseDown(3);
//
//		if (m_Type == ECT_FLY)
//		{
//			if (inMove)
//			{
//				if (!m_RightPressedDown)
//				{
//					ShowCursor(false);
//					m_RightPressedDown = true;
//
//					m_LastMouseDownPosition = hid.GetMousePosition();
//				}
//			}
//			else
//			{
//				if (m_RightPressedDown)
//				{
//					ShowCursor(true);
//					m_RightPressedDown = false;
//				}
//			}
//		}
//
//		float moveSpeed = m_MoveSpeed;
//		Ogre::Vector3 translate = Ogre::Vector3::ZERO;
//
//		if (hid.IsMouseDown(1))
//			moveSpeed *= 2;
//
//		if (hid.IsKeyDown('A'))
//		{
//			if (m_Type == ECT_FLY || m_Type == ECT_TOP || m_Type == ECT_FRONT)
//				translate.x += -moveSpeed;
//			else if (m_Type == ECT_LEFT)
//				translate.z += -moveSpeed;
//			else if (m_Type == ECT_RIGHT)
//				translate.z += +moveSpeed;
//		}
//
//		if (hid.IsKeyDown('D'))
//		{
//			if (m_Type == ECT_FLY || m_Type == ECT_TOP || m_Type == ECT_FRONT)
//				translate.x += moveSpeed;
//			else if (m_Type == ECT_LEFT)
//				translate.z += moveSpeed;
//			else if (m_Type == ECT_RIGHT)
//				translate.z -= moveSpeed;
//		}
//
//		if (hid.IsKeyDown('W'))
//		{
//			if (m_Type == ECT_FLY || m_Type == ECT_TOP)
//				translate.z += -moveSpeed;
//			else if (m_Type == ECT_LEFT || m_Type == ECT_RIGHT || m_Type == ECT_FRONT)
//				translate.y += moveSpeed;
//		}
//
//		if (hid.IsKeyDown('S'))
//		{
//			if (m_Type == ECT_FLY || m_Type == ECT_TOP)
//				translate.z += moveSpeed;
//			else if (m_Type == ECT_LEFT || m_Type == ECT_RIGHT || m_Type == ECT_FRONT)
//				translate.y -= moveSpeed;
//		}
//
//		if (hid.IsKeyDown('Q'))
//		{
//			if (m_Type == ECT_FLY || m_Type == ECT_TOP)
//				translate.y += -moveSpeed;
//			else if (m_Type == ECT_FRONT)
//				translate.z += moveSpeed;
//			else if (m_Type == ECT_LEFT)
//				translate.x += -moveSpeed;
//			else if (m_Type == ECT_RIGHT)
//				translate.x += moveSpeed;
//		}
//				// Move camera down
//
//		if (hid.IsKeyDown('E'))
//		{
//			if (m_Type == ECT_FLY || m_Type == ECT_TOP)
//				translate.y += moveSpeed;
//			else if (m_Type == ECT_FRONT)
//				translate.z += -moveSpeed;
//			else if (m_Type == ECT_LEFT)
//				translate.x += moveSpeed;
//			else if (m_Type == ECT_RIGHT)
//				translate.x += -moveSpeed;
//		}
//
//		ApplyTranslate(translate);
//		
//		if (inMove)
//		{
//			if (m_Type == ECT_FLY)
//			{
//				//if (hid.IsShiftDown())
//				{
//					const Ogre::Vector2 rel = (hid.GetMousePosition() - m_LastMouseDownPosition) * 0.005f;
//
//					m_Camera->yaw(Ogre::Degree(-rel.x));
//					m_Camera->pitch(Ogre::Degree(-rel.y));
//				}
//			}
//		}
//
//		if (hid.WasKeyPressed('F') && hid.IsControlDown())
//		{
//			GameObject *selected = ScenesHolder::GetInstance().GetCurrentScene()->GetSelectedObject();
//
//			if (selected)
//			{
//				m_Camera->setPosition(selected->GetPosition());
//			}
//		}
//	}
//}


bool EditorCamera::Serialize(Attributes *Attributes)
{
	Attributes->AddVector3("Position", m_Camera->getPosition());
	Attributes->AddQuaternion("Orientation", m_Camera->getOrientation());

	return true;
}


bool EditorCamera::Deserialize(Attributes *Attributes)
{
	m_Camera->setPosition(Attributes->GetVector3("Position"));
	m_Camera->setOrientation(Attributes->GetQuaternion("Orientation"));

	return true;
}


void EditorCamera::ApplyTranslate(const Ogre::Vector3 &Translation)
{
	if (Translation == Ogre::Vector3::ZERO)
		return;

	if (m_Type != ECT_FLY)
	{
		m_Camera->setPosition(m_Camera->getPosition() + Translation);
	}

	switch (m_Type)
	{
	case ECT_FLY:
		{
			m_Camera->moveRelative(Translation);
		} break;

	case ECT_TOP:
		{
			m_Camera->lookAt(m_Camera->getPosition() - Ogre::Vector3(0, 1000, 0));
		} break;

	case ECT_LEFT:
		{
			m_Camera->lookAt(m_Camera->getPosition() + Ogre::Vector3(1000, 0, 0));
		} break;

	case ECT_RIGHT:
		{
			m_Camera->lookAt(m_Camera->getPosition() - Ogre::Vector3(1000, 0, 0));
		} break;

	case ECT_FRONT:
		{
			m_Camera->lookAt(m_Camera->getPosition() - Ogre::Vector3(0, 0, 1000));
		} break;
	}
}


void EditorCamera::SetType(EditorCameraType Type)
{
	m_Type = Type;

	if (m_Type != ECT_FLY)
	{
		m_Camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
		m_Camera->setNearClipDistance(5);
	}

	switch (m_Type)
	{
	case ECT_FLY:
		{
		} break;

	case ECT_TOP:
		{
			m_Camera->lookAt(m_Camera->getPosition() - Ogre::Vector3(0, 1000, 0));
		} break;

	case ECT_LEFT:
		{
			m_Camera->lookAt(m_Camera->getPosition() + Ogre::Vector3(1000, 0, 0));
		} break;

	case ECT_RIGHT:
		{
			m_Camera->lookAt(m_Camera->getPosition() - Ogre::Vector3(1000, 0, 0));
		} break;

	case ECT_FRONT:
		{
			m_Camera->lookAt(m_Camera->getPosition() - Ogre::Vector3(0, 0, 1000));
		} break;
	}
}
END_NAMESPACE

#endif