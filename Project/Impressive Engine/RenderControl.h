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

#include "StdAfxOGRE.h"
#include "RenderControlBase.h"

BEGIN_NAMESPACE
class EditorCamera;
class GameObject;


class RenderControl : public RenderControlBase
{
public:
	RenderControl(wxWindow *Parent, const wxString &Name, bool IsMain, EditorCamera *Camera = 0);
	//~RenderControl(void);

	void FocusOn(GameObject *Object);

	bool _Update(void);

	void SetCamera(EditorCamera *Camera);
	
	void ResetAspectRatio(void);

	EditorCamera *GetCamera(void)
	{
		return m_Camera;
	}

private:
	bool m_IsMain;
	EditorCamera *m_Camera;
	bool m_RightPressedDown;
	Ogre::Vector2 m_LastMouseDownPosition;

	int m_TotalStepsCount;
	int m_StepsCount;
	Ogre::Vector3 m_LookAtPoint;
	Ogre::Vector3 m_FlyStep;
};
END_NAMESPACE

#endif