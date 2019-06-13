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

#include "ISerializable.h"
#include "StdAfxOGRE.h"
#include <vector>
#include "ScriptRegister.h"


BEGIN_NAMESPACE
class Script;

class Scriptable : public ISerializable
{
public:
	Scriptable(void);
	~Scriptable(void);
	
#ifdef EDIT_MODE
	virtual bool Serialize(Attributes *Attributes);
#endif
	virtual bool Deserialize(Attributes *Attributes);
	
	virtual void SetInitializeState(void);
	virtual void ResetToInitializeState(void);

	void ClearScripts(void);

	//virtual void OnSetObject(Script *Script, const char *Name);

	void SetAsGameObject(void)
	{
		m_IsGameObject = true;
	}

	bool AddScript(const char *FileName);

	void CallFunction(const char *Name);

	void SetScript(unsigned int Index, Ogre::String Value);

	DECLARE_SCRIPT_REGISTATION()

private:
	bool AddScriptCore(const char *FileName);

public:
	//const std::vector<Ogre::String> &GetScripts(void) const
	//{
	//	return m_ScriptFiles;
	//}

	Script *GetScript(unsigned int Index)
	{
		return m_Scripts[Index];
	}

	const std::vector<Script*> &GetScripts(void) const
	{
		return m_Scripts;
	}

private:
	// Last night, I thinking about we should do this in GameObject and after initial state we should call start
	// and before that, reloading scripts (mostely for launcher and maybe we need in editor)
	//std::vector<Ogre::String> m_ScriptFiles;
	std::vector<Script*> m_Scripts;

	bool m_IsGameObject;
	
	//std::vector<Ogre::String> m_Init_ScriptFiles;
	//std::vector<Script*> m_Init_Scripts;
};

END_NAMESPACE
