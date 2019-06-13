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

#include "StdAfxLua.h"
#include "StdAfxOGRE.h"
#include "Common.h"



BEGIN_NAMESPACE
class Script;

class VirtualMachineDebugger
{
public:
	//VirtualMachineDebugger(void);
	//~VirtualMachineDebugger(void);

	Ogre::String GetErrorText(int Code, Script *Script = 0);

	//void OnError(const error &Error);
	void OnError(int Code, Script *Script = 0);

	void SetHooksFlag(int Mask);

private:
	static void Hook(lua_State *State, lua_Debug *Args);
	static void HookCall(lua_State *State);

private:
	enum DEBUG_MASK
	{
		DM_CALL = LUA_MASKCALL,
		DM_RETURN = LUA_MASKRET,
		DM_LINE = LUA_MASKLINE,
		DM_COUNT = LUA_MASKCOUNT
	};
	
	int m_MaskCount;
};
END_NAMESPACE

