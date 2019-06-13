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
#include "File.h"
#include <OgreString.h>
#include "ScriptRegister.h"

#ifdef EDIT_MODE
#include "LogControl.h"
#endif


BEGIN_NAMESPACE
class Log : public Singleton<Log>
{
public:
	//Log(void);
	~Log(void);

	void Initialize(
#ifdef EDIT_MODE
	wxWindow *Parent
#endif
	);

	void AddInfo(const Ogre::String &Text);
	void AddError(const Ogre::String &Text);
    void AddWarning(const Ogre::String &Text);
	
#ifdef EDIT_MODE
	void Clear(void);
#endif

private:
	void LogInFile(const Ogre::String &Text);
	Ogre::String GetPrefixText(void);
	
public:
#ifdef EDIT_MODE
	wxString GetName(void)
	{
		return "Logs";
	}
	
	wxString GetCaption(void)
	{
		return "Logs";
	}

	LogControl *GetControl(void)
	{
		return m_Control;
	}
#endif

	DECLARE_SCRIPT_REGISTATION()

#ifdef EDIT_MODE
private:
	LogControl *m_Control;
#endif

	File *m_File;
};
END_NAMESPACE