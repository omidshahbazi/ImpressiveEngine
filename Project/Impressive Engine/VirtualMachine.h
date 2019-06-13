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
#include "StdAfxOGRE.h"
#include "StdAfxLua.h"
#include "VirtualMachineDebugger.h"


BEGIN_NAMESPACE
class Script;

class VirtualMachine : public Singleton<VirtualMachine>
{
public:
	VirtualMachine(void);
	~VirtualMachine(void);

	void Initialize(void);

	void Clear(void);

	void AddScriptLocation(const char *Directory, bool Recursive = false);

	Script *AddScript(Ogre::String FileName);

	Ogre::String GetNewScriptName(void);
	
	bool ExecuteScript(Ogre::String FileName);

	bool ExecuteCommand(const char *Command);
	bool ExecuteCommandCore(const char *Command, int &Result);

	void ClearItemsFromGlobalTable(void);
	void ClearItemFromGlobalTable(const char *Name);

	Ogre::String GetStandardPath(Ogre::String RelativePath, bool Absolute = true);

	void AddItemToRegister(const char *Name)
	{
		m_ItemsToRegister.push_back(Name);
	}

	std::vector<Ogre::String> &GetItemsToRegister(void)
	{
		return m_ItemsToRegister;
	}

public:
	lua_State *GetState(void) const
	{
		return m_State;
	}

	object GetGlobalTable(void) const
	{
		return globals(m_State);
	}

	VirtualMachineDebugger *GetDebugger(void) const
	{
		return m_Debugger;
	}

private:
	bool FileExistsInList(Ogre::String Path);
	
private:
	lua_State *m_State;
	VirtualMachineDebugger *m_Debugger;
	unsigned int m_ScriptCounter;
	
	std::vector<Ogre::String> m_ScriptFiles;
	//std::vector<Script*> m_Scripts;

	std::vector<Ogre::String> m_ItemsToRegister;
};
END_NAMESPACE