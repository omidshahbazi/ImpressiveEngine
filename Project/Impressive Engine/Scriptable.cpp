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
#include "Scriptable.h"
#include "VirtualMachine.h"
#include "Script.h"
#include "MeshGameObject.h"
#include "CameraGameObject.h"
#include "LightGameObject.h"
#include "TerrainGameObject.h"
#include "OceanGameObject.h"
#include "ParticleGameObject.h"
#include "Log.h"
#include "Utility.h"
#include "ScriptRegister.h"


BEGIN_NAMESPACE
Scriptable::Scriptable(void) :
	m_IsGameObject(false)
{
}


Scriptable::~Scriptable(void)
{
	ClearScripts();
}

#ifdef EDIT_MODE
bool Scriptable::Serialize(Attributes *Attributes)
{
	BEGIN_VECTOR_ITERATOR(m_Scripts, Script*)
		Attributes->AddString("Script" + Ogre::StringConverter::toString(i), current->GetFilePath().c_str());
		Attributes->AddBoolean("Script" + Ogre::StringConverter::toString(i) + "Enabled", current->GetEnabled());
	END_VECTOR_ITERATOR(NULL)

	return true;
}
#endif

bool Scriptable::Deserialize(Attributes *Attributes)
{
	int i = 0;
	Ogre::String Value = "";
	while (true)
	{
		Value = Attributes->GetString((Ogre::String("Script") + Ogre::StringConverter::toString(i)).c_str());

		if (Value.size() == 0)
			break;

		if (AddScript(Value.c_str()))
			m_Scripts[i]->SetEnabled(Attributes->GetBoolean(((Ogre::String("Script") + Ogre::StringConverter::toString(i) + "Enabled").c_str())));

		i++;
	}

	return true;
}


void Scriptable::SetInitializeState(void)
{
	//m_Init_Scripts = m_Scripts;
	//m_Init_ScriptFiles = m_ScriptFiles;
	
	BEGIN_VECTOR_ITERATOR(m_Scripts, Script*)
		current->Load();
	END_VECTOR_ITERATOR(NULL)
}


void Scriptable::ResetToInitializeState(void)
{
	//m_ScriptFiles = m_Init_ScriptFiles;
	//m_Scripts = m_Init_Scripts;

	//m_Init_ScriptFiles.clear();
	//m_Init_Scripts.clear();

	//
	//BEGIN_VECTOR_ITERATOR(m_Scripts, Script*)
	//	current->Unload();
	//	current->Load();
	//END_VECTOR_ITERATOR(NULL)
	//
	//m_Scripts.clear();
}


void Scriptable::ClearScripts(void)
{
	//m_ScriptFiles.clear();

	BEGIN_VECTOR_ITERATOR(m_Scripts, Script*)
		delete current;
	END_VECTOR_ITERATOR(NULL)
	m_Scripts.clear();

	//m_Init_Scripts.clear();

	//m_Init_ScriptFiles.clear();
}


//void Scriptable::OnSetObject(Script *Script, const char *Name)
//{
//	Script->GetTable()[Name] = this;
//}


bool Scriptable::AddScript(const char *FileName)
{
	Ogre::String path = VirtualMachine::GetInstance().GetStandardPath(FileName/*, true*/);

	if (!path.size())
	{
		Log::GetInstance().AddError(Ogre::String("Script file [") + FileName + "] doesn't exists");
		return false;
	}

	//BEGIN_VECTOR_ITERATOR(m_ScriptFiles, Ogre::String)
	//	if (current == file)
	//		return false;
	//END_VECTOR_ITERATOR("")

	BEGIN_VECTOR_ITERATOR(m_Scripts, Script*)
		if (current->GetFilePath() == path)
			return false;
	END_VECTOR_ITERATOR(NULL)

	return AddScriptCore(FileName);
}


void Scriptable::CallFunction(const char *Name)
{
	BEGIN_VECTOR_ITERATOR(m_Scripts, Script*)
		if (current->GetEnabled())
			current->CallFunctionCore(Name);
	END_VECTOR_ITERATOR(NULL)
}


void Scriptable::SetScript(unsigned int Index, Ogre::String Value)
{
	if (Index >= m_Scripts.size())
	{
		AddScript(Value.c_str());
		return;
	}

	if (Value.size())
	{
		Ogre::StringUtil::toLowerCase(Value);
		
		if (m_Scripts[Index]->GetFilePath() != Value)
		{
			delete m_Scripts[Index];
			m_Scripts[Index] = VirtualMachine::GetInstance().AddScript(Value);
		}
	}
	else
	{
		std::vector<Script*> oldList = m_Scripts;
		m_Scripts.clear();

		BEGIN_VECTOR_ITERATOR(oldList, Script*)
			if (i != Index)
				m_Scripts.push_back(current);
		END_VECTOR_ITERATOR(NULL)

		oldList.clear();

		//m_Init_ScriptFiles = m_ScriptFiles;
		//m_ScriptFiles.clear();

		//BEGIN_VECTOR_ITERATOR(m_Init_ScriptFiles, Ogre::String)
		//	if (i != Index)
		//		m_ScriptFiles.push_back(current);
		//END_VECTOR_ITERATOR("")

		//m_Init_ScriptFiles.clear();
	}
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(Scriptable)
	BEGIN_DEFINE_TYPE(Scriptable, "Scriptable")
		//DEFINE_FUNCTION("AddScript", &Scriptable::AddScriptAndExcute)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()


bool Scriptable::AddScriptCore(const char *FileName)
{
	//Ogre::String file = VirtualMachine::GetInstance().GetStandardPath(FileName, false);

	//if (!file.size())
	//{
	//	Log::GetInstance().AddError("Script file [" + file + "] doesn't exists");
	//	return;
	//}

	Script *script = VirtualMachine::GetInstance().AddScript(FileName);

	if (script)
	{
		m_Scripts.push_back(script);

		script->RegisterThis();

		//script->SetCellValue("Object", this);

		if (m_IsGameObject)
		{
			switch (((GameObject*)this)->GetType())
			{
			case GameObject::GOT_EMPTY:
				script->GetTable()["Object"] = (GameObject*)this;
				break;
			case GameObject::GOT_MESH:
				script->GetTable()["Object"] = (MeshGameObject*)this;
				break;
			case GameObject::GOT_CAMERA:
				script->GetTable()["Object"] = (CameraGameObject*)this;
				break;
			case GameObject::GOT_LIGHT:
				script->GetTable()["Object"] = (LightGameObject*)this;
				break;
			case GameObject::GOT_TERRAIN:
				script->GetTable()["Object"] = (TerrainGameObject*)this;
				break;
			case GameObject::GOT_OCEAN:
				script->GetTable()["Object"] = (OceanGameObject*)this;
				break;
			case GameObject::GOT_PARTICLE:
				script->GetTable()["Object"] = (ParticleGameObject*)this;
				break;
			}
		}

		//script->SetCellValue<GameObject>("Object", this);

		//OnSetObject(script, "Object");

		//script->Load();

		script = NULL;

		return true;
	}

	return false;
}
END_NAMESPACE