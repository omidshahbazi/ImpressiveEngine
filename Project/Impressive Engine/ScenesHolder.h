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
#include <vector>

#ifdef EDIT_MODE
#include "SceneControl.h"
#include "EditorScene.h"
#else
#include "Scene.h"
#endif

BEGIN_NAMESPACE
class ScenesHolder : public NodeBase, public Singleton<ScenesHolder>
{
public:
	ScenesHolder(void);
	//~ScenesHolder(void);

	void Initialize(
#ifdef EDIT_MODE
	wxWindow *Parent
#endif
	);
	
#ifdef EDIT_MODE
	void InitializeScene(void);
#endif
	
	//void Start(void);
	bool Update(void);
	
#ifndef EDIT_MODE
	Scene *LoadScene(const Ogre::String &Name, const Ogre::String &FilePath);
	
	Scene *GetScene(const Ogre::String &Name);
	Scene *GetScene(unsigned int Index);
	
	void SetCurrentScene(Scene *Current);
#endif
	
#ifdef EDIT_MODE
	EditorScene
#else
	Scene
#endif
		*GetCurrentScene(void);
	
#ifndef EDIT_MODE
	void DestroyScene(const Ogre::String &Name);
	void DestroyScene(unsigned int Index);
	void DestroyScene(Scene *Current);

	void DestroyAllScenes(void);
#endif
	
	CameraGameObject *GetCameraGameObject(Ogre::Camera *Camera);

private:
#ifndef EDIT_MODE
	void ProcessPendingCurrentScene(void);
	void ProcessPendingDestroy(void);
#endif
	
#ifdef EDIT_MODE
public:
	wxString GetName(void)
	{
		return "Scene";
	}
	
	wxString GetCaption(void)
	{
		return "Scene";
	}

	SceneControl *GetControl(void)
	{
		return m_Control;
	}
#endif
	
private:
#ifdef EDIT_MODE
	SceneControl *m_Control;
#else
	Scene *m_PendingToSetAsCurrentScene;
	Scene *m_PendingToDestroy;
#endif

	std::vector<Scene*> m_Scenes;
	int m_Current;
};
END_NAMESPACE