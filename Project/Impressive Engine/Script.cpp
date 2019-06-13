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
#include "Script.h"
#include "VirtualMachine.h"
#include "Engine.h"
#include "IO.h"
#include "Log.h"
#include "Utility.h"


BEGIN_NAMESPACE
Script::Script(Ogre::String FilePath) :
	m_FilePath(FilePath),
	m_WasLoaded(false),
	m_Enabled(true)
{
	m_Name = VirtualMachine::GetInstance().GetNewScriptName();

	Ogre::StringUtil::splitFullFilename(m_FilePath, m_FileName, Ogre::String(), Ogre::String());

	CreateTable();
	//Load();
}


Script::~Script(void)
{
	//Log::GetInstance().AddInfo("Script file [" + m_FilePath + "] with name [" + m_Name + "] was removed");
	Unload();
}


bool Script::HasFunction(const char *Name)
{
	if (!m_WasLoaded)
		return false;

	object func = m_Table[Name];

	int i = type(func);
	if (func && i == LUA_TFUNCTION)
		return true;

	return false;
}


bool Script::CallFunction(const char *Name)
{
	return CallFunctionCore(Name, true);
}


bool Script::CallFunctionCore(const char *Name, bool LogError)
{
	if (!m_WasLoaded)
		return false;

	if (!HasFunction(Name))
	{
		if (LogError)
			Log::GetInstance().AddError("Script file [" + m_FilePath + "] with name [" + m_Name + "] has no function with name [" + Name + "]");

		return false;
	}

	RegisterThis();

#ifdef DEBUG
	VirtualMachine::GetInstance().GetGlobalTable()["DEBUG_FILE_NAME"] = m_FileName + ".script";
	VirtualMachine::GetInstance().GetGlobalTable()["DEBUG_FUNCTION"] = Name;
#endif

	try
	{
		m_Table[Name]();
		//m_Table[Name](m_Table);
	}
	catch (std::exception &e)
	{
		Log::GetInstance().AddError(Ogre::String("Calling function [") + Name + "] in script file [" + m_FilePath + "] with name [" + Name + "] had an error details : " + e.what());

		return false;
	}

	return true;
}


//template<class T> void Script::SetCellValue(const char *Name, void *Object)
//{
//	try
//	{
//		// void * not registered in classes, so there's an exception on runtime
//		m_Table[Name] = (T*)Object;
//	}
//	catch (std::exception &e)
//	{
//		Log::GetInstance().AddError(e.what());
//	}
//}


bool Script::Load(void)
{
	VirtualMachine::GetInstance().ClearItemsFromGlobalTable();

	int result = 0;
	if (VirtualMachine::GetInstance().ExecuteCommandCore(IO::GetInstance().ReadScript(VirtualMachine::GetInstance().GetStandardPath(m_FilePath)).c_str(), result))
	{
		RegisterCellsFromGlobalTable();

		m_WasLoaded = true;

		Log::GetInstance().AddInfo("Script file [" + m_FilePath + "] successfully loaded with name [" + m_Name + "]");
	}
	else
	{
		VirtualMachine::GetInstance().GetDebugger()->OnError(result, this);

		m_WasLoaded = false;
	}

	return m_WasLoaded;
}


void Script::Unload(void)
{
	VirtualMachine::GetInstance().GetGlobalTable()[m_Name.c_str()] = nil;

	m_WasLoaded = false;
}


void Script::RegisterThis(void)
{
	VirtualMachine::GetInstance().GetGlobalTable()["this"] = m_Table;
}


void Script::CreateTable(void)
{
	m_Table = newtable(VirtualMachine::GetInstance().GetState());
	VirtualMachine::GetInstance().GetGlobalTable()[m_Name.c_str()] = m_Table;
}

	
void Script::RegisterCellsFromGlobalTable(void)
{
	RegisterCellFromGlobalTable("Start");

	RegisterCellFromGlobalTable("Update");

	RegisterCellFromGlobalTable("PreRender");
	RegisterCellFromGlobalTable("PostRender");
	
	RegisterCellFromGlobalTable("OnCollide");
	RegisterCellFromGlobalTable("OnCollision");

	BEGIN_VECTOR_ITERATOR(VirtualMachine::GetInstance().GetItemsToRegister(), Ogre::String)
		RegisterCellFromGlobalTable(current.c_str());
		VirtualMachine::GetInstance().ClearItemFromGlobalTable(current.c_str());
	END_VECTOR_ITERATOR("")
	VirtualMachine::GetInstance().GetItemsToRegister().clear();
}


void Script::RegisterCellFromGlobalTable(const char *Name)
{
	m_Table[Name] = VirtualMachine::GetInstance().GetGlobalTable()[Name];
}
END_NAMESPACE