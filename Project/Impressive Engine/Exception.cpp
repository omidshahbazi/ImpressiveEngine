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
#include "Exception.h"
#include "Log.h"

#ifdef EDIT_MODE
#include "MainWindow.h"
#else
#include <Windows.h>
#endif

BEGIN_NAMESPACE
const char *builtInExceptionReason[] = { "Invalid Parameter", "File Not Found", "Unknown" };


Exception *Singleton<Exception>::m_Instance = NULL;

//Exception::Exception(void)
//{
//}
//
//
//Exception::~Exception(void)
//{
//}


void Exception::Throw(Ogre::Exception &Exception, bool Resume)
{
	Throw(ER_UNKNOWN, Exception.what(), Exception.getDescription(), Resume);
}


void Exception::Throw(const ExceptionReason &Reason, const Ogre::String &Title, const Ogre::String &Text, bool Resume)
{
	Ogre::String str = Title + "(" + builtInExceptionReason[Reason] + ") : " + Text;

	Log::GetInstance().AddError(str);

#ifndef DEBUG
	UINT style = MB_OK;

	if (Resume)
		style |= MB_ICONWARNING;
	else
		style |= MB_ICONSTOP;
	
	MessageBox(
#ifdef EDIT_MODE
		(HWND)MainWindow::GetPointer()->GetHWND()
#else
		NULL
#endif
		,Text.c_str(), Title.c_str(), style);
#endif

#ifdef EDIT_MODE
	if (!Resume)
		MainWindow::GetPointer()->CloseCommandFromException();
#endif
}
END_NAMESPACE