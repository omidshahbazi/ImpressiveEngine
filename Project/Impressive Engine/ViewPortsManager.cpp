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
#include "ViewPortsManager.h"
#include "Engine.h"
#include "Exception.h"

#ifdef EDIT_MODE
#include "MainWindow.h"
#include "EditorCamera.h"
#include "RenderControl.h"
#include "HID.h"
#include "ScenesHolder.h"
#include "Utility.h"
#else
#include "CameraGameObject.h"
#include "Scene.h"
#endif


BEGIN_NAMESPACE
ViewPortsManager *Singleton<ViewPortsManager>::m_Instance = NULL;

ViewPortsManager::ViewPortsManager(void) :
#ifdef EDIT_MODE
	m_EditRenderControl(NULL),
	m_GameRenderControl(NULL)
#else
	m_ViewPort(NULL)
#endif
{
}


//ViewPortsManager::~ViewPortsManager(void)
//{
//}


#ifdef EDIT_MODE
//void ViewPortsManager::Initialize(void)
//{
	//ViewPort *vp = new ViewPort(m_MainWindow, 10, "VP", "View Port");

	//m_MainWindow->AddPaneInCenter(vp, vp->GetName(), vp->GetCaption());
	//m_ViewPorts.Add(vp);

	//vp = 0;
//}
#endif


void ViewPortsManager::Initialize(void)
{
	try
	{
#ifdef EDIT_MODE
		MainWindow *mainWin = MainWindow::GetPointer();

		m_EditRenderControl = new RenderControl(mainWin, "EditViewPort", true);
		mainWin->AddPaneInCenter(m_EditRenderControl, m_EditRenderControl->GetName(), "");

		m_GameRenderControl = new RenderControlBase(mainWin, "GameViewPort");
		mainWin->AddPaneInCenter(m_GameRenderControl, m_GameRenderControl->GetName(), "");
		mainWin->GetPane(m_GameRenderControl->GetName()).CaptionVisible(false);
		mainWin->GetPane(m_GameRenderControl->GetName()).Hide();
#else
		Ogre::NameValuePairList params;
		params["colourDepth"] = "32";
		params["vsync"] = "false";
		params["antiAliasing"] = "true";

		//Ogre::Vector2 res = GetDesktopResolution();

		m_RenderWindow = Ogre::Root::getSingleton().createRenderWindow(Engine::ENGINE_NAME, 800, 600, false, &params);

		m_RenderWindow->getCustomAttribute("WINDOW", &m_HWND);

		//CameraGameObject *cam = Scene::GetInstance().CreateCameraGameObject(Scene::GetInstance().GetNewCameraName());

		m_ViewPort = m_RenderWindow->addViewport(NULL/*cam->_GetCamera()*/);
		m_ViewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

		//cam->_GetCamera()->setAspectRatio(Ogre::Real(m_ViewPort->getActualWidth()) / Ogre::Real(m_ViewPort->getActualHeight()));

		//m_Cameras.push_back(cam);
#endif
	}
	catch (Ogre::Exception &e)
	{
		Exception::GetInstance().Throw(e);
	}
}


#ifdef EDIT_MODE
void ViewPortsManager::Update(void)
{
	m_EditRenderControl->_Update();
	m_GameRenderControl->_Update();
}


#include "Scene.h"
void ViewPortsManager::InitializeCameras(void)
{
	m_CurrentCameraType = EditorCamera::ECT_FLY;

	EditorCamera *camera = new EditorCamera(EditorCamera::ECT_FLY);
	camera->SetPosition(100, 100, 100);
	camera->LookAt(0, 0, 0);
	m_Cameras.push_back(camera);

	camera = new EditorCamera(EditorCamera::ECT_TOP);
	camera->SetPosition(0, 200, 0);
	m_Cameras.push_back(camera);

	camera = new EditorCamera(EditorCamera::ECT_LEFT);
	camera->SetPosition(-100, 0, 0);
	m_Cameras.push_back(camera);

	camera = new EditorCamera(EditorCamera::ECT_RIGHT);
	camera->SetPosition(100, 0, 0);
	m_Cameras.push_back(camera);

	camera = new EditorCamera(EditorCamera::ECT_FRONT);
	camera->SetPosition(0, 0, 100);
	m_Cameras.push_back(camera);
	
	camera = 0;
	
	SetViewPortCamera((EditorCamera::EditorCameraType)m_CurrentCameraType);
}


bool ViewPortsManager::IsEditViewPort(wxWindow *Window)
{
	//for (unsigned int i = 0; i < m_ViewPorts.size(); i++)
	//	if (m_ViewPorts[i] == Window)
	//		return true;

	//return false;

	return (m_EditRenderControl == Window);
}


bool ViewPortsManager::IsGameViewPort(wxWindow *Window)
{
	return (m_GameRenderControl == Window);
}


void ViewPortsManager::SetViewPortCamera(EditorCamera::EditorCameraType Type)
{
	if (!MainWindow::GetPointer()->IsInEditMode())
		return;

	m_CurrentCameraType = (int)Type;

	EditorCamera *camera = GetCamera(Type);

	//if (!camera)
	//	BREAK_IF(camera == NULL);

	//if (!m_OgreViewPort)
	//{
	//	m_OgreViewPort = m_RenderWindow->addViewport(camera->_GetCamera());
	//	m_OgreViewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	//}
	//else
	//{
	////	m_OgreViewPort->setCamera(camera->GetCamera());
	//}
		
	//camera->SetActive(true);

	//GetViewPort()->SetAspectRatio();

	m_EditRenderControl->SetCamera(camera);

	MainWindow::GetPointer()->GetPane(m_EditRenderControl->GetName()).Caption(STRING_OGRE_TO_WX(camera->GetName() + " Camera"));
	MainWindow::GetPointer()->UpdatePanes();

	ScenesHolder::GetInstance().GetCurrentScene()->HandleViewportChange();
}


Ogre::Ray ViewPortsManager::GetViewpPortRay(void)
{
	Ogre::Vector2 mousePos = HID::GetInstance().GetNormalizedMousePosition();
	return GetCurrentCamera()->GetCameraToViewportRay(mousePos.x, mousePos.y);
}


void ViewPortsManager::NextCamera(void)
{
	//GetCurrentCamera()->SetActive(false);

	m_CurrentCameraType++;

	if (m_CurrentCameraType == EditorCamera::ECT_COUNT)
		m_CurrentCameraType = EditorCamera::ECT_FLY;

	SetViewPortCamera((EditorCamera::EditorCameraType)m_CurrentCameraType);
}
#endif

#ifdef EDIT_MODE
EditorCamera *ViewPortsManager::GetCamera(EditorCamera::EditorCameraType Type)
//#ifdef EDIT_MODE
//	EditorCamera
//#else
//	CameraGameObject
//#endif
//	*ViewPortsManager::GetCamera(
//#ifdef EDIT_MODE
//	EditorCamera::EditorCameraType Type
//#endif
//	)
{
	std::list<
#ifdef EDIT_MODE
	EditorCamera
#else
	CameraGameObject
#endif
	*>::iterator it = m_Cameras.begin();
		
#ifdef EDIT_MODE
	for ( ; it != m_Cameras.end(); it++)
	{
		if ((*it)->GetType() == Type)
			return *it;
	}

	return 0;
#else
	return *it;
#endif
}
#endif


Ogre::RenderWindow *ViewPortsManager::GetRenderWindow(void)
{
#ifdef EDIT_MODE
	return m_EditRenderControl->GetRenderWindow();
#else
	return m_RenderWindow;
#endif
}


Ogre::Viewport *ViewPortsManager::GetViewPort(void)
{
#ifdef EDIT_MODE
	return m_EditRenderControl->GetViewport();
#else
	return m_ViewPort;
#endif
}
END_NAMESPACE