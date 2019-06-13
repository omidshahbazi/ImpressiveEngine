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
#include "Engine.h"
#include "Scene.h"
#include "Log.h"
#include "ScenesHolder.h"
#include "CameraGameObject.h"
#include "VirtualMachine.h"
#include "ScriptRegister.h"



BEGIN_NAMESPACE
class CoreFunctionality : public Singleton<CoreFunctionality>
{
public:
	//CoreFunctionality(void);
	//~CoreFunctionality(void)

	bool GetInEditMode(void)
	{
#ifdef EDIT_MODE
		return true;
#else
		return false;
#endif
	}

	Ogre::String GetEngineName(void)
	{
		return Engine::ENGINE_NAME;
	}

	Ogre::String GetExecutablePath(void)
	{
		return Application::EXECUTABLE_PATH.mb_str();
	}

	Ogre::String GetExecutableName(void)
	{
		return Application::EXECUTABLE_NAME.mb_str();
	}

	Ogre::String GetPathData(void)
	{
		return Engine::PATH_DATA;
	}

	Ogre::String GetPathLevel(void)
	{
		return Engine::PATH_LEVEL;
	}

	Ogre::String GetPathMaterial(void)
	{
		return Engine::PATH_MATERIAL;
	}

	Ogre::String GetPathModel(void)
	{
		return Engine::PATH_MODEL;
	}

	Ogre::String GetPathPlugIns(void)
	{
		return Engine::PATH_PLUGINS;
	}

	Ogre::String GetPathScene(void)
	{
		return Engine::PATH_SCENE;
	}

	Ogre::String GetPathScript(void)
	{
		return Engine::PATH_SCRIPT;
	}

	Ogre::String GetPathTexture(void)
	{
		return Engine::PATH_TEXTURE;
	}

	Scene *LoadScene(const Ogre::String &Name, const Ogre::String &FilePath)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::LoadScene(\"") + Name + "\", \"" + FilePath + "\") called by script");
		return NULL;
#else
		return ScenesHolder::GetInstance().LoadScene(Name, FilePath);
#endif
	}
	
	Scene *GetScene(const Ogre::String &Name)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::GetScene(\"") + Name + "\") called by script");
		return NULL;
#else
		return ScenesHolder::GetInstance().GetScene(Name);
#endif
	}

	Scene *GetScene(unsigned int Index)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::GetScene(") + Ogre::StringConverter::toString(Index) + ") called by script");
		return NULL;
#else
		return ScenesHolder::GetInstance().GetScene(Index);
#endif
	}
	
	void SetCurrentScene(Scene *Current)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::SetCurrentScene(") + (Current ? "\"" + Current->GetName() + "\"" : "NULL") + ") called by script");
#else
		if (Current)
			Current->Resume();
#endif
	}

	Scene *GetCurrentScene(void)
	{
		return ScenesHolder::GetInstance().GetCurrentScene();
	}
	
	void DestroyScene(const Ogre::String &Name)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::DestroyScene(\"") + Name + "\") called by script");
#else
		ScenesHolder::GetInstance().DestroyScene(Name);
#endif
	}

	void DestroyScene(unsigned int Index)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::DestroyScene(") + Ogre::StringConverter::toString(Index) + ") called by script");
#else
		ScenesHolder::GetInstance().DestroyScene(Index);
#endif
	}

	void DestroyScene(Scene *Current)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::DestroyScene(") + (Current ? "\"" + Current->GetName() + "\"" : "NULL") + ") called by script");
#else
		ScenesHolder::GetInstance().DestroyScene(Current);
#endif
	}

	void SetCamera(CameraGameObject *Camera)
	{
#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().GetGameRenderControl()->SetCamera(Camera->_GetCamera());
#else
		ViewPortsManager::GetInstance().GetViewPort()->setCamera(Camera->_GetCamera());
#endif

		GetCurrentScene()->HandleViewportChange();

		//GetCurrentScene()->UpdateDSS();
	}

	CameraGameObject *GetCamera(void)
	{
#ifdef EDIT_MODE
		return ScenesHolder::GetInstance().GetCameraGameObject(ViewPortsManager::GetInstance().GetGameRenderControl()->GetOgreCamera());
#else
		return ScenesHolder::GetInstance().GetCameraGameObject(ViewPortsManager::GetInstance().GetViewPort()->getCamera());
#endif
	}

	void AddScriptLocation(const char *Directory, bool Recursive)
	{
		VirtualMachine::GetInstance().AddScriptLocation(Directory, Recursive);
	}

	void DefineFunction(const char *Name)
	{
		VirtualMachine::GetInstance().AddItemToRegister(Name);
	}

	bool ExecuteScript(Ogre::String FileName)
	{
		return VirtualMachine::GetInstance().ExecuteScript(FileName);
	}

	bool ExecuteCommand(const char *Command)
	{
		return VirtualMachine::GetInstance().ExecuteCommand(Command);
	}

	void SetSize(unsigned int Width, unsigned int Height)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::SetSize(") + Ogre::StringConverter::toString(Width) + ", "+ Ogre::StringConverter::toString(Height) + ") called by script");
#else
		ViewPortsManager::GetInstance().GetRenderWindow()->resize(Width, Height);
#endif
	}

	void SetSize(unsigned int Width, unsigned int Height, bool FullScreen)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::SetSize(") + Ogre::StringConverter::toString(Width) + ", "+ Ogre::StringConverter::toString(Height) + ", " + Ogre::StringConverter::toString(FullScreen) + ") called by script");
#else
		ViewPortsManager::GetInstance().GetRenderWindow()->setFullscreen(FullScreen, Width, Height);
#endif
	}
	
	void SetPosition(unsigned int Left, unsigned int Top)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::SetPosition(") + Ogre::StringConverter::toString(Left) + ", "+ Ogre::StringConverter::toString(Top) + ") called by script");
#else
		ViewPortsManager::GetInstance().GetRenderWindow()->reposition(Left, Top);
#endif
	}

	unsigned int GetWidth(void)
	{
		return ViewPortsManager::GetInstance().GetRenderWindow()->getWidth();
	}

	unsigned int GetHeight(void)
	{
		return ViewPortsManager::GetInstance().GetRenderWindow()->getHeight();
	}

	void SetText(const Ogre::String &Text)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo(Ogre::String("CoreFunctionality::SetText(\"") + Text + "\") called by script");
#else
		SendMessage(ViewPortsManager::GetInstance().GetHWND(), WM_SETTEXT, 0, LPARAM(Text.c_str()));
#endif
	}

	void TakeScreenshot(const Ogre::String &Path)
	{
#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().GetGameRenderControl()->GetRenderWindow()
#else
		ViewPortsManager::GetInstance().GetRenderWindow()
#endif
		->writeContentsToFile(Path);
	}

	void Close(void)
	{
#ifdef EDIT_MODE
		Log::GetInstance().AddInfo("CoreFunctionality::Close() called by script");
#else
		Engine::GetInstance().Shutdown();
#endif
	}

	DECLARE_SCRIPT_REGISTATION()
};
END_NAMESPACE