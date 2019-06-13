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

#include "StdAfxOGRE.h"
#include "StdAfxLua.h"
#include "Common.h"

BEGIN_NAMESPACE
//class GameObject;

class Script
{
public:
	Script(Ogre::String FilePath);
	~Script(void);

	bool HasFunction(const char *Name);
	bool CallFunction(const char *Name);
	bool CallFunctionCore(const char *Name, bool LogError = false);

	//template<class T> void SetCellValue(const char *Name, void *Object);

	bool Load(void);
	void Unload(void);

	void RegisterThis(void);

private:	
	void CreateTable(void);
	void RegisterCellsFromGlobalTable(void);
	void RegisterCellFromGlobalTable(const char *Name);

public:
	const Ogre::String &GetName(void) const
	{
		return m_Name;
	}

	const Ogre::String &GetFilePath(void) const
	{
		return m_FilePath;
	}

	const Ogre::String &GetFileName(void) const
	{
		return m_FileName;
	}

	const bool GetWasLoaded(void) const
	{
		return m_WasLoaded;
	}

	const object GetTable(void) const
	{
		return m_Table;
	}

	void SetEnabled(bool Value)
	{
		m_Enabled = Value;
	}

	const bool GetEnabled(void) const
	{
		return m_Enabled;
	}

private:
	Ogre::String m_Name;
	Ogre::String m_FilePath;
	Ogre::String m_FileName;

	bool m_WasLoaded;
	
	object m_Table;

	bool m_Enabled;
};
END_NAMESPACE