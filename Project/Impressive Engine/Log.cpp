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
#include "Log.h"
#include <OgreStringConverter.h>
#include "SystemTime.h"
#include "ScriptRegister.h"
#include "Utility.h"

#ifdef EDIT_MODE
#include "StatusBarControl.h"
#endif

#ifndef EDIT_MODE
#include <iostream>

using namespace std;
#endif


BEGIN_NAMESPACE
Log *Singleton<Log>::m_Instance = NULL;


//Log::Log(void)
//{
//}


Log::~Log(void)
{
	if (m_File->IsOpen())
		m_File->Close();
}


void Log::Initialize(
#ifdef EDIT_MODE
	wxWindow *Parent
#endif
	)
{
#ifdef EDIT_MODE
	m_Control = new LogControl(Parent);
#endif

	m_File = new File("Engine Log.log", false);
}


void Log::AddInfo(const Ogre::String &Text)
{
	Ogre::String time = GetPrefixText() + Text;

#ifdef EDIT_MODE
    m_Control->Add(STRING_OGRE_TO_WX(time), LogControl::LT_INFO);
    StatusBarControl::GetInstance().ShowLastLog(STRING_OGRE_TO_WX(time), 0);
#endif

	LogInFile(time);
}


void Log::AddError(const Ogre::String &Text)
{
	Ogre::String time = GetPrefixText();

#ifdef EDIT_MODE
	m_Control->Add(STRING_OGRE_TO_WX(time + Text), LogControl::LT_ERROR);
    StatusBarControl::GetInstance().ShowLastLog(STRING_OGRE_TO_WX(time + "ERROR : " + Text), 1);
#endif

	LogInFile(time + "ERROR : " + Text);
}

void Log::AddWarning(const Ogre::String &Text)
{
	Ogre::String time = GetPrefixText();

#ifdef EDIT_MODE
    m_Control->Add(STRING_OGRE_TO_WX(time + Text), LogControl::LT_WARNING);
    StatusBarControl::GetInstance().ShowLastLog(STRING_OGRE_TO_WX(time + "Warning : " + Text), 2);
#endif

	LogInFile(time + "Warning : " + Text);
}


#ifdef EDIT_MODE
void Log::Clear(void)
{
	m_Control->Clear();
}
#endif

void Log::LogInFile(const Ogre::String &Text)
{
	//if (m_File.is_open())
	//{
	//	m_File << Text << "\n";
	//	m_File.flush();
	//}

	if (m_File->IsOpen())
	{
		m_File->WriteText(Text + "\r\n");
	}
	
#ifndef EDIT_MODE
	cout << Text << "\n";
#endif
}


Ogre::String Log::GetPrefixText(void)
{
	Ogre::String h = Ogre::StringConverter::toString(SystemTime::GetHours());
	if (h.size() < 2)
		h = "0" + h;
	
	Ogre::String m = Ogre::StringConverter::toString(SystemTime::GetMinutes());
	if (m.size() < 2)
		m = "0" + m;
	
	Ogre::String s = Ogre::StringConverter::toString(SystemTime::GetSeconds());
	if (s.size() < 2)
		s = "0" + s;

	return h + ":" + m + ":" + s + " ";
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(Log)
	BEGIN_DEFINE_TYPE(Log, "Log")
		DEFINE_FUNCTION("AddInfo", &Log::AddInfo)
		DEFINE_FUNCTION("AddError", &Log::AddError)
        DEFINE_FUNCTION("AddWarning", &Log::AddWarning)
		
#ifdef EDIT_MODE
		DEFINE_FUNCTION("Clear", &Log::Clear)
#endif
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE