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

#include "RenderControl.h"
#include "MainWindow.h"
#include "ViewPortsManager.h"
#include "EditorCamera.h"
#include "ScenesHolder.h"
#include "GameObject.h"
#include "HID.h"

#include "Log.h"


BEGIN_NAMESPACE
RenderControl::RenderControl(wxWindow *Parent, const wxString &Name, bool IsMain, EditorCamera *Camera) :
	RenderControlBase(Parent, Name),
	m_IsMain(IsMain),
	m_Camera(Camera),
	m_RightPressedDown(false),
	m_TotalStepsCount(100),
	m_StepsCount(m_TotalStepsCount)
{
	SetCamera(m_Camera);

	//Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(RenderControl::OnMouse));
	//Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(RenderControl::OnMouse));
	//Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(RenderControl::OnMouse));
}


//RenderControl::~RenderControl(void)
//{
//}


void RenderControl::FocusOn(GameObject *Object)
{
	Ogre::Vector3 target = Object->GetPosition() - m_Camera->GetPosition();
	target.normalise();

	float radius = (Object->GetWorldAABB().getCenter() - Object->GetWorldAABB().getMaximum()).length() + 30;

	target = Object->GetWorldAABB().getCenter() + (target * Ogre::Vector3(-radius));

	m_FlyStep = (target - m_Camera->GetPosition()) / (float)m_TotalStepsCount;
	
	m_LookAtPoint = Object->GetWorldAABB().getCenter();

	m_StepsCount = 0;
}


bool RenderControl::_Update(void)
{
	if (!RenderControlBase::_Update())
		return false;

	if (!m_Camera)
		return false;

	if (m_StepsCount < m_TotalStepsCount)
	{
		m_Camera->SetPosition(m_Camera->GetPosition() + m_FlyStep);

		m_Camera->LookAt(m_LookAtPoint);

		m_StepsCount++;

		return true;
	}

	if (!HID::GetInstance().IsThisActive(this))
		return false;

	const HID &hid = HID::GetInstance();
	const bool inMove = hid.IsMouseDown(HID::M_RIGHT);
	const EditorCamera::EditorCameraType camType = m_Camera->GetType();

	if (camType == EditorCamera::ECT_FLY)
	{
		if (inMove)
		{
			SetFocus();

			if (!m_RightPressedDown)
			{
				//ShowCursor(false);
				m_RightPressedDown = true;

				m_LastMouseDownPosition = hid.GetMousePosition();
			}
		}
		else
		{
			if (m_RightPressedDown)
			{
				//ShowCursor(true);
				m_RightPressedDown = false;
			}
		}
	}

	float moveSpeed = 1.75f;//m_MoveSpeed;
	Ogre::Vector3 translate = Ogre::Vector3::ZERO;

	if (/*hid.IsMouseDown(HID::M_LEFT)*/hid.IsKeyDown(HID::K_LSHIFT))
		moveSpeed *= 2;

	if (hid.IsKeyDown(HID::K_A))
	{
		if (camType == EditorCamera::ECT_FLY || camType == EditorCamera::ECT_TOP || camType == EditorCamera::ECT_FRONT)
			translate.x += -moveSpeed;
		else if (camType == EditorCamera::ECT_LEFT)
			translate.z += -moveSpeed;
		else if (camType == EditorCamera::ECT_RIGHT)
			translate.z += +moveSpeed;
	}

	if (hid.IsKeyDown(HID::K_D))
	{
		if (camType == EditorCamera::ECT_FLY || camType == EditorCamera::ECT_TOP || camType == EditorCamera::ECT_FRONT)
			translate.x += moveSpeed;
		else if (camType == EditorCamera::ECT_LEFT)
			translate.z += moveSpeed;
		else if (camType == EditorCamera::ECT_RIGHT)
			translate.z -= moveSpeed;
	}

	if (hid.IsKeyDown(HID::K_W))
	{
		if (camType == EditorCamera::ECT_FLY || camType == EditorCamera::ECT_TOP)
			translate.z += -moveSpeed;
		else if (camType == EditorCamera::ECT_LEFT || camType == EditorCamera::ECT_RIGHT || camType == EditorCamera::ECT_FRONT)
			translate.y += moveSpeed;
	}

	if (hid.IsKeyDown(HID::K_S))
	{
		if (camType == EditorCamera::ECT_FLY || camType == EditorCamera::ECT_TOP)
			translate.z += moveSpeed;
		else if (camType == EditorCamera::ECT_LEFT || camType == EditorCamera::ECT_RIGHT || camType == EditorCamera::ECT_FRONT)
			translate.y -= moveSpeed;
	}

	if (hid.IsKeyDown(HID::K_Q))
	{
		if (camType == EditorCamera::ECT_FLY || camType == EditorCamera::ECT_TOP)
			translate.y += -moveSpeed;
		else if (camType == EditorCamera::ECT_FRONT)
			translate.z += moveSpeed;
		else if (camType == EditorCamera::ECT_LEFT)
			translate.x += -moveSpeed;
		else if (camType == EditorCamera::ECT_RIGHT)
			translate.x += moveSpeed;
	}

	if (hid.IsKeyDown(HID::K_E))
	{
		if (camType == EditorCamera::ECT_FLY || camType == EditorCamera::ECT_TOP)
			translate.y += moveSpeed;
		else if (camType == EditorCamera::ECT_FRONT)
			translate.z += -moveSpeed;
		else if (camType == EditorCamera::ECT_LEFT)
			translate.x += moveSpeed;
		else if (camType == EditorCamera::ECT_RIGHT)
			translate.x += -moveSpeed;
	}

	m_Camera->ApplyTranslate(translate);
		
	if (inMove)
	{
		if (camType == EditorCamera::ECT_FLY)
		{
			//if (hid.IsShiftDown())
			//{
				const Ogre::Vector2 &mousePos = hid.GetMousePosition();

				if (mousePos.x > 0 && mousePos.x < GetSize().x &&
					mousePos.y > 0 && mousePos.y < GetSize().y)
				{
					const Ogre::Vector2 rel = (mousePos - m_LastMouseDownPosition) * 0.005f;

					m_Camera->Yaw(Ogre::Degree(-rel.x));
					m_Camera->Pitch(Ogre::Degree(-rel.y));

					//wxPoint pos = ClientToScreen(wxPoint(m_LastMouseDownPosition.x, m_LastMouseDownPosition.x));
					//WarpPointer(pos.x, pos.y);
				}
			//}
		}
	}

	//if (hid.WasKeyPressed(HID::K_F) && hid.IsControlDown())
	//{
	//	GameObject *selected = ScenesHolder::GetInstance().GetCurrentScene()->GetSelectedObject();

	//	if (selected)
	//	{
	//		m_Camera->SetPosition(selected->GetPosition());
	//	}

	//	selected = NULL;
	//}

	return true;
}
	

void RenderControl::SetCamera(EditorCamera *Camera)
{
	if (!Camera)
		return;

	m_Camera = Camera;

	RenderControlBase::SetCamera(Camera->_GetCamera());
}


void RenderControl::ResetAspectRatio(void)
{
	if (m_Viewport->getCamera())
	{
		int w, h;
		GetSize(&w, &h);

		m_Camera->UpdateRenderWindowSize(w, h, m_IsMain);
	}
}
END_NAMESPACE

#endif