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
#include "Application.h"

#ifdef EDIT_MODE
#include "MainWindow.h"
#include "Engine.h"
#include "HID.h"
#include "ViewPortsManager.h"
#include "Editor.h"
#include "ScriptControl.h"
#include "RenderControl.h"

#include <wx/msw/private.h>
#else
#include "Engine.h"
#include "ScenesHolder.h"
#include "Log.h"
#endif

#include <wx/filefn.h>

//using namespace IE;

wxString Application::EXECUTABLE_NAME = "";
wxString Application::EXECUTABLE_PATH = "";
HINSTANCE Application::m_Instance = NULL;

//LONG WINAPI CrashHandler(EXCEPTION_POINTERS * /*ExceptionInfo*/)
//{
//	std::cout << "Gotcha!" << std::endl;
//
//	//return EXCEPTION_CONTINUE_SEARCH;
//	return EXCEPTION_EXECUTE_HANDLER;
//}

//Application::Application(void)
//{
//}
//
//
//Application::~Application(void)
//{
//}

int Application::Go(
#ifndef EDIT_MODE
		HINSTANCE Instance
#endif
		)
{
	//SetUnhandledExceptionFilter(CrashHandler);

#ifdef EDIT_MODE
	m_Instance = wxGetInstance();
#else
	m_Instance = Instance;
#endif

	char path[2048];

	GetModuleFileName(m_Instance, path, 2048);

	wxSplitPath(wxString(path), &EXECUTABLE_PATH, &EXECUTABLE_NAME, 0);

	//m_ExecutableName += ".exe";
	EXECUTABLE_PATH += "\\";
	
	Engine::GetInstance().InitializePaths();

#ifdef EDIT_MODE
	wxInitAllImageHandlers();

	MainWindow::Create();
	MainWindow::GetPointer()->Initialize();
	MainWindow::GetPointer()->Show();

	//SetTopWindow(MainWindow::GetPointer());
#else
	if (Engine::GetInstance().Initialize())
	{
		if (VirtualMachine::GetInstance().ExecuteScript("Main.script"))
		{
			Engine::GetInstance().StartRendering();
			ScenesHolder::GetInstance().DestroyAllScenes();
		}
		else
			Log::GetInstance().AddError("Main.script doesn't exists, so there's no entry-point to start.");

		//Scene *scene = ScenesHolder::GetInstance().LoadScene("Level 0", Engine::PATH_LEVEL + "Level0.level");

		//if (scene)
		//{
		//	scene->Resume();

		//	Engine::GetInstance().StartRendering();
		//}
		//else
		//{
		//	Log::GetInstance().AddError("There's no Level0.level to starting engine based on it.");
		//}

		//scene = NULL;

		//ScenesHolder::GetInstance().DestroyAllScenes();
	}
#endif

	return 0;
}


#ifdef EDIT_MODE
bool Application::OnInit()
{
	wxApp::OnInit();

	return (Go() == 0);
}


//void Application::OnTick(void)
//{
//	Engine::GetInstance().Tick();
//}


#include "FrameListener.h"
#include "AVIRecorder.h"
int Application::MainLoop()
{
	int result = wxApp::MainLoop();

	//FrameListener::GetInstance().m_Avi->ReleaseEngine();
	return result;
}


void Application::OnUnhandledException(void)
{
	MainWindow::GetPointer()->CloseCommandFromException();
}


int Application::FilterEvent(wxEvent &event)
{
	if (event.GetEventType() == wxEVT_KEY_DOWN)
	{
		const wxKeyEvent& evt = (wxKeyEvent&)event;
		const int keyCode = evt.GetKeyCode();

		if (keyCode == HID::K_F5)
		{
			MainWindow::GetPointer()->ToggleMode();

			return 1;
		}
		
		if (MainWindow::GetPointer()->IsInEditMode())
		{
			//if (keyCode == HID::K_R)
			//{
			//	FrameListener::GetInstance().initilizeRecorder();
			//	FrameListener::GetInstance().recording = true;
			//}

			if (keyCode == HID::K_F1)
			{
				MainWindow::GetPointer()->ToggleGizmoType();

				return 1;
			}
			else if (keyCode == HID::K_F3)
			{
				ViewPortsManager::GetInstance().NextCamera();

				return 1;
			}
			else if (keyCode == HID::K_N)
			{
				if (evt.ControlDown())
				{
					MainWindow::GetPointer()->New();

					return 1;
				}
			}
			else if (keyCode == HID::K_L)
			{
				if (evt.ControlDown())
				{
					MainWindow::GetPointer()->Load();

					return 1;
				}
			}
			else if (keyCode == HID::K_S)
			{
				if (evt.ControlDown())
				{
					if (evt.ShiftDown())
					{
						MainWindow::GetPointer()->SaveAs();
					}
					else
					{
						MainWindow::GetPointer()->Save();
					}

					ScriptControl::GetPointer()->SaveAll();

					return 1;
				}
			}
			else if (keyCode == HID::K_Z)
			{
				if (evt.ControlDown())
				{
					Editor::GetInstance().DoUndo();

					return 1;
				}
			}
			else if (keyCode == HID::K_Y)
			{
				if (evt.ControlDown())
				{
					Editor::GetInstance().DoRedo();

					return 1;
				}
			}
            else if (keyCode == HID::K_F)
			{
				if (evt.ControlDown())
				{
					Editor::GetInstance().FocusGameObject();

					return 1;
				}
			}
		}
	}

	//if (MainWindow::GetPointer() && !MainWindow::GetPointer()->IsInEditMode())
	//{
		wxEventType type = event.GetEventType();

		if (type == wxEVT_KEY_DOWN || type == wxEVT_KEY_UP)
		{
			HID::GetInstance().UpdateKeyboard((wxKeyEvent&)event);
			//HID::GetInstance().UpdateKeyboard(evt.GetKeyCode(), (evt.GetEventType() == wxEVT_KEY_DOWN), evt.ControlDown(), evt.ShiftDown(), evt.AltDown());
		}
		else if (type == wxEVT_LEFT_DOWN || type == wxEVT_LEFT_UP || type == wxEVT_MIDDLE_DOWN || type == wxEVT_MIDDLE_UP || type == wxEVT_RIGHT_DOWN || type == wxEVT_RIGHT_UP ||
				type == wxEVT_MOTION || type == wxEVT_LEFT_DCLICK || type == wxEVT_MIDDLE_DCLICK || type == wxEVT_RIGHT_DCLICK || type == wxEVT_LEAVE_WINDOW || type == wxEVT_ENTER_WINDOW ||
				type == wxEVT_MOUSEWHEEL)
		{
			//RenderControl *rc = dynamic_cast<RenderControl*>((wxEvtHandler*)event.GetEventObject());
			//IE::RenderControlBase *rcb = dynamic_cast<IE::RenderControlBase*>(event.GetEventObject());
			RenderControlBase *rcb = dynamic_cast<RenderControlBase*>(event.GetEventObject());
			
			HID::GetInstance().SetActiveRenderControl(rcb);

			if (rcb)
				HID::GetInstance().UpdateMouse((wxMouseEvent&)event);

			rcb = NULL;
		}
	//}
 
    return -1;
}
#endif