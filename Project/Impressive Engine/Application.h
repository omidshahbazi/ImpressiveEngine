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

#include "StdAfxwxWidgets.h"

#ifndef EDIT_MODE
#include <Windows.h>
#endif

class Application
#ifdef EDIT_MODE
	: public wxApp
#endif
{
public:
	//Application(void);
	//~Application(void);

	int Go(
#ifndef EDIT_MODE
		HINSTANCE Instance
#endif
		);

	static HINSTANCE GetInstance(void)
	{
		return m_Instance;
	}
	
#ifdef EDIT_MODE
private:
	bool OnInit();
	//void OnTick(void);

	int MainLoop();
	void OnUnhandledException(void);

	int FilterEvent(wxEvent &event);
#endif

public:
	static wxString EXECUTABLE_NAME;
	static wxString EXECUTABLE_PATH;

private:
	static HINSTANCE m_Instance;
};