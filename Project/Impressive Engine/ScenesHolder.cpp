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
#include "ScenesHolder.h"
#include "Engine.h"
#include "Utility.h"
#include "VirtualMachine.h"
#include "CameraGameObject.h"
#include "IO.h"
#include "Log.h"
#include "Common.h"
#include <stdio.h>


BEGIN_NAMESPACE
ScenesHolder *Singleton<ScenesHolder>::m_Instance = NULL;


ScenesHolder::ScenesHolder(void) :
#ifndef EDIT_MODE
	m_PendingToSetAsCurrentScene(NULL),
	m_PendingToDestroy(NULL),
#endif
	m_Current(-1)
{
	// This class is Singleton and will use in Terrain generation
	Ogre::TerrainGlobalOptions *tgo = new Ogre::TerrainGlobalOptions();
	tgo->setDefaultResourceGroup(Engine::RESOURCES_TERRAIN);
	tgo = NULL;
}


//ScenesHolder::~ScenesHolder(void)
//{
//}


void ScenesHolder::Initialize(
#ifdef EDIT_MODE
	wxWindow *Parent
#endif
	)
{
#ifdef EDIT_MODE
	m_Control = new SceneControl(Parent);
#endif

	// This line has been commented without testing it in Launch
	//VirtualMachine::GetInstance().Initialize();
}


#ifdef EDIT_MODE
void ScenesHolder::InitializeScene(void)
{
	m_Scenes.push_back(new EditorScene("GenericScene"));
	m_Current = 0;
}
#endif


//void ScenesHolder::Start(void)
//{
//	if (m_Current > -1)
//		m_Scenes[m_Current]->Start();
//}


bool ScenesHolder::Update(void)
{
#ifndef EDIT_MODE
	// Process pending scene to set as current scene
	ProcessPendingCurrentScene();

	ProcessPendingDestroy();
#endif

	if (m_Current > -1)
		return m_Scenes[m_Current]->Update();

	return true;
}

#ifndef EDIT_MODE
Scene *ScenesHolder::LoadScene(const Ogre::String &Name, const Ogre::String &FilePath)
{
	if (IO::GetInstance().FileExists(FilePath))
	{
		Scene *scene = new Scene(Name);

		m_Scenes.push_back(scene);

		//scene->IniDSS();

		scene->Load(FilePath);

		scene->SetInitializeState();

		return scene;
	}

	return NULL;
}


Scene *ScenesHolder::GetScene(const Ogre::String &Name)
{
	BEGIN_VECTOR_ITERATOR(m_Scenes, Scene*)
		if (current->GetName() == Name)
			return current;
	END_VECTOR_ITERATOR(NULL)

		return NULL;
}


Scene *ScenesHolder::GetScene(unsigned int Index)
{
	if (Index < m_Scenes.size())
		return m_Scenes[Index];

	return NULL;
}


void ScenesHolder::SetCurrentScene(Scene *Current)
{
	if (!Current)
	{
		m_Current = -1;
		return;
	}

	m_PendingToSetAsCurrentScene = Current;
}
#endif


#ifdef EDIT_MODE
	EditorScene
#else
	Scene
#endif
		*ScenesHolder::GetCurrentScene(void)
{
	if (m_Current > -1)
		return
#ifdef EDIT_MODE
	(EditorScene*)
#else
	(Scene*)
#endif
		m_Scenes[m_Current];

	return NULL;
}

#ifndef EDIT_MODE
void ScenesHolder::DestroyScene(const Ogre::String &Name)
{
	BEGIN_VECTOR_ITERATOR(m_Scenes, Scene*)
		if (current->GetName() == Name)
		{
			DestroyScene(current);
			break;
		}
	END_VECTOR_ITERATOR(NULL)
}


void ScenesHolder::DestroyScene(unsigned int Index)
{
	if (Index < m_Scenes.size())
		DestroyScene(m_Scenes[Index]);
}


void ScenesHolder::DestroyScene(Scene *Current)
{
	m_PendingToDestroy = Current;
}


void ScenesHolder::DestroyAllScenes(void)
{
	BEGIN_VECTOR_ITERATOR(m_Scenes, Scene*)
		delete current;
	END_VECTOR_ITERATOR(NULL)

	m_Scenes.clear();
}
#endif

CameraGameObject *ScenesHolder::GetCameraGameObject(Ogre::Camera *Camera)
{
	CameraGameObject *cgo = NULL;

	BEGIN_VECTOR_ITERATOR(m_Scenes, Scene*)
		cgo = current->GetCamera(Camera);

	if (cgo)
		return cgo;
	END_VECTOR_ITERATOR(NULL) 

	return NULL;
}

#ifndef EDIT_MODE
void ScenesHolder::ProcessPendingCurrentScene(void)
{
	if (!m_PendingToSetAsCurrentScene)
		return;

	BEGIN_VECTOR_ITERATOR(m_Scenes, Scene*)
		if (current == m_PendingToSetAsCurrentScene)
		{
			m_Current = (int)i;
			break;
		}
	END_VECTOR_ITERATOR(NULL)

	m_PendingToSetAsCurrentScene->ConnectFirstCamera();

	m_PendingToSetAsCurrentScene->HandleViewportChange();

	m_PendingToSetAsCurrentScene->InitializePostProcessEffects();

	//VirtualMachine::GetInstance().Initialize();

	//m_PendingToSetAsCurrentScene->GetRootGameObject()->SetInitializeState();

	m_PendingToSetAsCurrentScene->Start();

	m_PendingToSetAsCurrentScene = NULL;
}


void ScenesHolder::ProcessPendingDestroy(void)
{
	Ogre::Camera *cam = ViewPortsManager::GetInstance().GetViewPort()->getCamera();

	Scene *currScene = GetCurrentScene();

	if (!m_PendingToDestroy)
		return;

	if (GetCurrentScene() == m_PendingToDestroy)
	{
		Log::GetInstance().AddError("You cannot destroy current scene, you must change it first by calling Core:SetCurrentScene(Scene)");
		return;
	}

	m_PendingToDestroy->Clear();
	//Engine::GetInstance().GetGraphic()->destroySceneManager(m_PendingToDestroy->GetSceneManager());

	std::vector<Scene*> list = m_Scenes;
	m_Scenes.clear();
	BEGIN_VECTOR_ITERATOR(list, Scene*)
		if (current != m_PendingToDestroy)
			m_Scenes.push_back(current);
	END_VECTOR_ITERATOR(NULL)
	list.clear();

	delete m_PendingToDestroy;

	m_PendingToDestroy = NULL;

	//
	// Reset current Scene index
	//
	BEGIN_VECTOR_ITERATOR(m_Scenes, Scene*)
		if (current == currScene)
		{
			m_Current = (int)i;
			break;
		}
	END_VECTOR_ITERATOR(NULL)


	//
	// Conncet a cammera again, because in Destorying last scene, we set current camera with NULLL
	//
	ViewPortsManager::GetInstance().GetViewPort()->setCamera(cam);

	GetCurrentScene()->HandleViewportChange();
}
#endif
END_NAMESPACE