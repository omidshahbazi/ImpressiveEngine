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

#include "RenderControlBase.h"
#include "Engine.h"
#include "ScenesHolder.h"
#include "Utility.h"


BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(RenderControlBase, wxPanel)
	EVT_SIZE(RenderControlBase::OnSize)
	//EVT_CHAR(RenderControl::OnKeyDown)
	//EVT_KEY_UP(RenderControl::OnKeyUp)
	//EVT_MOUSE_EVENTS(RenderControl::OnMouse)
	//EVT_LEFT_DOWN(func)
    //EVT_LEFT_UP(func)
    //EVT_RIGHT_DOWN(func)
    //EVT_RIGHT_UP(func)
    //EVT_MOTION(func)
END_EVENT_TABLE()


RenderControlBase::RenderControlBase(wxWindow *Parent, const wxString &Name, Ogre::Camera *Camera) :
	wxPanel(Parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL),
	m_RenderWindow(NULL),
	m_Viewport(NULL)
{
	SetName(Name);

	Ogre::NameValuePairList params;
	params["colourDepth"] = "32";
	params["vsync"] = "false";
	params["antiAliasing"] = "true";
	params["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)GetHWND());

	m_RenderWindow = Engine::GetInstance().GetGraphic()->createRenderWindow(STRING_WX_TO_OGRE(Name), 1, 1, false, &params);

	SetCamera(Camera);
}


//RenderControlBase::~RenderControlBase(void)
//{
//}


bool RenderControlBase::_Update(void)
{
	if (GetParent()->IsShown() && IsShown() && (!GetParent()->GetParent() || GetParent()->GetParent()->IsShown()))
	{
		m_RenderWindow->setActive(true);
	}
	else
	{
		m_RenderWindow->setActive(false);
		return false;
	}

	return true;
}


bool RenderControlBase::SetBackgroundColour(const wxColour &Color)
{
	m_Viewport->setBackgroundColour(Ogre::ColourValue(Color.Red() / 255.f, Color.Green() / 255.f, Color.Blue() / 255.f));

	return true;
}


void RenderControlBase::SetCamera(Ogre::Camera *Camera)
{
	if (m_Viewport)
		m_Viewport->setCamera(Camera);
	else
		m_Viewport = m_RenderWindow->addViewport(Camera);

	m_Viewport->setShadowsEnabled(true);

	SetBackgroundColour(wxColor("Gray"));

	ResetAspectRatio();
}


void RenderControlBase::ResetAspectRatio(void)
{
	if (m_Viewport->getCamera())
	{
		int w, h;
		GetSize(&w, &h);

		m_Viewport->getCamera()->setAspectRatio((float)w / h);
	}
}

	
void RenderControlBase::OnSize(wxSizeEvent &event)
{
	ResetAspectRatio();

	//m_Viewport->mActWidth = event.GetSize().GetX();
	//m_Viewport->mActHeight = event.GetSize().GetY();
	m_RenderWindow->resize(event.GetSize().GetX(), event.GetSize().GetY());
	//m_Viewport->_updateDimensions();
}
END_NAMESPACE

#endif