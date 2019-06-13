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
#ifdef EDIT_MODE

#pragma once

#include "Scene.h"
#include "EditorListener.h"

BEGIN_NAMESPACE
class EditorScene : public Scene, public EditorListener
{
public:
	EditorScene(const Ogre::String &Name = "");
	~EditorScene(void);

	void Start(void);
	bool Update(void);
	
	bool Serialize(Attributes *Attributes);

	void OnTranslate(const Ogre::Vector3 &Value);
	void OnScale(const Ogre::Vector3 &Value);
	void OnRotate(const Ogre::Vector3 &Value);

#ifdef DEBUG
	virtual void InitializeStats(void);
#endif
	
	void ResetToInitializeStateScriptUsage(void);

	void Resume(void);

	void Destroy(void);
	
	void RemoveGameObject(GameObject *Object);
	void DestroyGameObject(GameObject *Object);

	void Clear(void);

	bool ConnectFirstCamera(void);
	void HandleViewportChange(void);
	
	void RemovePostProcessEffect(const Ogre::String &Name);
	void AddPostProcessEffect(const Ogre::String &Name, int Index = -1);
	
	void Load(const Ogre::String &FilePath);
	void Save(const Ogre::String &FilePath, bool ForEditor = true);

	void PlayGame(void);
	void StopGame(void);
	
	void SelectObject(GameObject *Object);
	void SelectObject(unsigned int ID);

	void RemoveSelectedGameObject(void);
	void RefreshSceneControlContent(void);

	GameObject *GetSelectedObject(void)
	{
		//return GetGameObject(m_SelectedNode);
		return m_SelectedGameObject;
	}

private:
	void ReadCamera(const TreeElement *Element);

	void WriteGameObject(TreeElement *Element, GameObject *Object, bool ForExport = false);

	//void OnMouseClick(void);

	//Ogre::MovableObject *GetSceneObject(OBJECT_TYPE Type);

private:
	bool m_IsMouseDown;
	Ogre::Vector2 m_LastMouseDownPos;
	Ogre::RaySceneQuery *m_RaySceneQuery;
	GameObject *m_SelectedGameObject;
};

END_NAMESPACE
#endif