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
#include "VirtualMachineDebugger.h"
#include "VirtualMachine.h"
#include "Script.h"
#include "Log.h"


BEGIN_NAMESPACE
//VirtualMachineDebugger::VirtualMachineDebugger(void)
//{
//}
//
//
//VirtualMachineDebugger::~VirtualMachineDebugger(void)
//{
//}


Ogre::String VirtualMachineDebugger::GetErrorText(int Code, Script *Script)
{
	Ogre::String text = "";

	switch (Code)
	{
	case 0: 
		return "";

	case LUA_YIELD:
		text += "Yield";
		break;

	case LUA_ERRRUN:
		text += "Run Error";
		break;

	case LUA_ERRSYNTAX:
		text += "Syntax Error";
		break;

	case LUA_ERRMEM:
		text += "Memory Allocation Error";
		break;

	case LUA_ERRERR:
		text += "Run Again Error";
		break;
	}

	text += " raised";

	if (Script)
		text += " in script [" + Script->GetFilePath() + "]";

	text += " detail : ";
	text += lua_tostring(VirtualMachine::GetInstance().GetState(), -1);

	return text;
}


//void VirtualMachineDebugger::OnError(const error &Error)
//{
//	Log(Error.what());
//}


void VirtualMachineDebugger::OnError(int Code, Script *Script)
{	
	Log::GetInstance().AddError(GetErrorText(Code, Script));
}


void VirtualMachineDebugger::SetHooksFlag(int Mask)
{
	lua_sethook(VirtualMachine::GetInstance().GetState(), Hook, Mask, m_MaskCount);
}


void VirtualMachineDebugger::Hook(lua_State *State, lua_Debug *Args)
{
	switch (Args->event)
	{
	case LUA_HOOKCALL:
	case LUA_HOOKRET:
	case LUA_HOOKLINE:
	case LUA_HOOKCOUNT:
		HookCall(State);
		break;
	}
}


void VirtualMachineDebugger::HookCall(lua_State *State)
{
	lua_Debug ar;

	for (int i = 0; lua_getstack(State, i, &ar) != 0; i++)
	{
		if (lua_getinfo(State, "SnlufL", &ar) != 0)
		{
			//printf("%d %s %s %d @%d %s\n", i, ar.namewhat, ar.name, ar.nups, ar.linedefined, ar.short_src);
			
			Ogre::String str = "";
			
			if (Ogre::String(ar.namewhat).size() > 0 )
				str += Ogre::String(ar.namewhat) + " - ";
			
			if (Ogre::String(ar.name).size() > 0 )
				str += Ogre::String(ar.name) + " - ";

			str += Ogre::String("Line ") + Ogre::StringConverter::toString(ar.linedefined) + " ";

			str += Ogre::String("[") + ar.short_src + "]";
			
			Log::GetInstance().AddInfo(str);
		}
	}
}
END_NAMESPACE