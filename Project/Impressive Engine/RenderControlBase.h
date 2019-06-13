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

#include "StdAfxwxWidgets.h"
#include "StdAfxOGRE.h"
#include "Common.h"


BEGIN_NAMESPACE
class RenderControlBase : public wxPanel
{
public:
	RenderControlBase(wxWindow *Parent, const wxString &Name, Ogre::Camera *Camera = 0);
	//~RenderControlBase(void);

	virtual bool _Update(void);

	bool SetBackgroundColour(const wxColour &Color);
	
	void SetCamera(Ogre::Camera *Camera);

protected:
	virtual void ResetAspectRatio(void);

public:
	Ogre::RenderTarget &GetRenderTarget(void)
	{
		return *m_RenderWindow;
	}

	Ogre::RenderWindow *GetRenderWindow(void)
	{
		return m_RenderWindow;
	}

	Ogre::Viewport *GetViewport(void)
	{
		return m_Viewport;
	}

	Ogre::Camera *GetOgreCamera(void)
	{
		if (!m_Viewport)
			return NULL;

		return m_Viewport->getCamera();
	}

private:
	DECLARE_EVENT_TABLE()

	void OnSize(wxSizeEvent &event);

protected:
	Ogre::RenderWindow *m_RenderWindow;
	Ogre::Viewport *m_Viewport;
};
END_NAMESPACE

#endif