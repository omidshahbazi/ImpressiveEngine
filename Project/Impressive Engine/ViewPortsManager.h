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

#include "Singleton.h"
#include "StdAfxOGRE.h"

#ifdef EDIT_MODE
#include "StdAfxwxWidgets.h"
#include "EditorCamera.h"

BEGIN_NAMESPACE
class RenderControlBase;
class RenderControl;

#else
class CameraGameObject;
#endif


class ViewPortsManager : public Singleton<ViewPortsManager>
{
public:
	ViewPortsManager(void);
	//~ViewPortsManager(void);
	
	void Initialize(void);

#ifdef EDIT_MODE
	void Update(void);
	void InitializeCameras(void);
	bool IsEditViewPort(wxWindow *Window);
	bool IsGameViewPort(wxWindow *Window);
	void SetViewPortCamera(EditorCamera::EditorCameraType Type);
	Ogre::Ray GetViewpPortRay(void);
	void NextCamera(void);
#endif

#ifdef EDIT_MODE
	EditorCamera *GetCamera(EditorCamera::EditorCameraType Type);
#endif
//#ifdef EDIT_MODE
//	EditorCamera
//#else
//	CameraGameObject
//#endif
//		*GetCamera(
//#ifdef EDIT_MODE
//			EditorCamera::EditorCameraType Type
//#endif
//	);

#ifdef EDIT_MODE
	EditorCamera *GetCurrentCamera(void)
	{
		return GetCamera((EditorCamera::EditorCameraType)m_CurrentCameraType);
	}

	const std::list<EditorCamera*> &GetCameras(void)
	{
		return m_Cameras;
	}

	RenderControl *GetEditRenderControl(void)
	{
		return m_EditRenderControl;
	}

	RenderControlBase *GetGameRenderControl(void)
	{
		return m_GameRenderControl;
	}
#endif
	
	Ogre::RenderWindow *GetRenderWindow(void);
	Ogre::Viewport *GetViewPort(void);

#ifndef EDIT_MODE
	const HWND &GetHWND(void) const
	{
		return m_HWND;
	}
#endif
	

private:
#ifdef EDIT_MODE
	std::list<EditorCamera*> m_Cameras;
#endif

//	std::list<
//#ifdef EDIT_MODE
//	EditorCamera
//#else
//	CameraGameObject
//#endif
//	*> m_Cameras;

#ifdef EDIT_MODE
	int m_CurrentCameraType;
	RenderControl *m_EditRenderControl;
	RenderControlBase *m_GameRenderControl;
#else
	Ogre::RenderWindow *m_RenderWindow;
	HWND m_HWND;
	Ogre::Viewport *m_ViewPort;
#endif
};
END_NAMESPACE