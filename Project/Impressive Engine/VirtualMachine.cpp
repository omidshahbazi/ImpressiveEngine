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
#include "VirtualMachine.h"
#include "ScriptTypesDefinition.h"
#include "Script.h"
#include "Engine.h"
#include "Utility.h"
#include "IO.h"
#include "Log.h"


BEGIN_NAMESPACE
VirtualMachine *Singleton<VirtualMachine>::m_Instance = NULL;


VirtualMachine::VirtualMachine(void) :
	m_State(NULL),
	m_Debugger(NULL),
	m_ScriptCounter(0)
{
	m_Debugger = new VirtualMachineDebugger();
}


VirtualMachine::~VirtualMachine(void)
{
	Clear();

	lua_close(m_State);
}


void VirtualMachine::Initialize(void)
{
	if (m_State)
	{
		Clear();

		lua_close(m_State);
	}

	m_State = lua_open();

	luaL_openlibs(m_State);

    open(m_State);

	ScriptTypesDefinition::GetInstance().Initialize();

	AddScriptLocation(Engine::PATH_SCRIPT.c_str());
}


void VirtualMachine::Clear(void)
{
	m_ScriptFiles.clear();

	//for (unsigned int i = 0; i < m_Scripts.size(); i++)
	//	delete m_Scripts[i];

	//m_Scripts.clear();

	m_ScriptCounter = 0;

	ClearItemsFromGlobalTable();
}


void VirtualMachine::AddScriptLocation(const char *Directory, bool Recursive)
{
	std::vector<Ogre::String> add = GetFiles(Directory);

	for (unsigned int i = 0; i < add.size(); i++)
	{
		if (!FileExistsInList(add[i]))
		{
			Ogre::StringUtil::toLowerCase(add[i]);
			m_ScriptFiles.push_back(add[i]);
		}
	}

	if (Recursive)
	{
		add = GetDirectories(Directory);

		for (unsigned int i = 0; i < add.size(); i++)
			AddScriptLocation(add[i].c_str(), Recursive);
	}
}


Script *VirtualMachine::AddScript(Ogre::String FileName)
{
	Ogre::String file = GetStandardPath(FileName, false);

	if (!file.size())
	{
		Log::GetInstance().AddError("Script file [" + FileName + "] doesn't exists");

		return NULL;
	}

	return new Script(file);

	//m_Scripts.push_back(new Script(file));

	//return m_Scripts[m_Scripts.size() - 1];
}


Ogre::String VirtualMachine::GetNewScriptName(void)
{
	return "Script" + Ogre::StringConverter::toString(m_ScriptCounter++);
}


bool VirtualMachine::ExecuteScript(Ogre::String FileName)
{
	Ogre::String file = GetStandardPath(FileName);

	if (!file.size())
	{
		Log::GetInstance().AddError("Script file [" + FileName + "] doesn't exists");

		return false;
	}

	int result = 0;
	if (ExecuteCommandCore(IO::GetInstance().ReadScript(file).c_str(), result))
	{
		Log::GetInstance().AddInfo("Script file [" + FileName + "] successfully executed");

		return true;
	}

	Log::GetInstance().AddError("In execution of [" + FileName + "] " + VirtualMachine::GetInstance().GetDebugger()->GetErrorText(result));

	return false;
}


bool VirtualMachine::ExecuteCommand(const char *Command)
{
	int result = 0;

	if (ExecuteCommandCore(Command, result))
		return true;
	
	m_Debugger->OnError(result);

	return false;
}


bool VirtualMachine::ExecuteCommandCore(const char *Command, int &Result)
{
	Result = luaL_dostring(m_State, Command);
	return (Result == 0);
}


void VirtualMachine::ClearItemsFromGlobalTable(void)
{
	//ClearItemFromGlobalTable("this");

	ClearItemFromGlobalTable("Start");

	ClearItemFromGlobalTable("Update");

	ClearItemFromGlobalTable("PreRender");
	ClearItemFromGlobalTable("PostRender");
	
	ClearItemFromGlobalTable("OnCollide");
	ClearItemFromGlobalTable("OnCollision");
}


void VirtualMachine::ClearItemFromGlobalTable(const char *Name)
{
	GetGlobalTable()[Name] = nil;
}


Ogre::String VirtualMachine::GetStandardPath(Ogre::String RelativePath, bool Absolute)
{
	if (!Ogre::StringUtil::endsWith(RelativePath, ".script", true))
		RelativePath += ".script";

	RelativePath = Ogre::StringUtil::replaceAll(RelativePath, "/", "\\");

	Ogre::StringUtil::toLowerCase(RelativePath);

	for (unsigned int i = 0; i < m_ScriptFiles.size(); i++)
	{
		if (Ogre::StringUtil::endsWith(m_ScriptFiles[i], RelativePath, false))
			return (Absolute ? m_ScriptFiles[i] : RelativePath);
	}
	
	return "";
}


bool VirtualMachine::FileExistsInList(Ogre::String Path)
{
	for (unsigned int i = 0; i < m_ScriptFiles.size(); i++)
		if (Ogre::StringUtil::match(m_ScriptFiles[i], Path, false))
			return true;
	
	return false;
}
END_NAMESPACE