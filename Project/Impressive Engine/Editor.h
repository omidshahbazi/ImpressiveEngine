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

#include "Singleton.h"
#include "StdAfxOGRE.h"
#include <wx/dynarray.h>
#include "EditorListener.h"

BEGIN_NAMESPACE
class DoBase;
class GameObject;

class Editor : public Singleton<Editor>/*, public Ogre::RaySceneQueryListener*/
{
public:
	Editor(void);
	//~Editor(void);

	//bool queryResult(Ogre::MovableObject* obj, float distance);
	//bool queryResult(Ogre::SceneQuery::WorldFragment* fragment, float distance) { return true; }

	void Initialize(void);

	void Update(void);

	bool ExportGame(const Ogre::String &CurrentScene, const std::vector<Ogre::String> &Scenes);

	void ClearDoList(void);
	
	void DoRedo(void);
	void DoUndo(void);

    void FocusGameObject(void);

	void AddRemoveGameObjectDo(GameObject *Object);
	void AddCreateGameObjectDo(GameObject *Object);

	void LinkTranslateGizmoTo(Ogre::SceneNode *Node);
	void SeparateTranslateGizmo(Ogre::SceneNode *Node = NULL);

	enum TRANSFORM_TYPE
	{
		TT_NONE = 0,
		TT_TRANSLATE,
		TT_ROTATE,
		TT_SCALE,

		TT_COUNT
	};

	enum TRANSLATE_TYPE
	{
		TT_LOCAL = 0,
		TT_GLOBAL
	};


	DoBase *GetLastDo(void);
	void AddDoBase(DoBase *Do, bool DoIt = true);
	
	void SetListener(EditorListener *Listener, GameObject *GameObject);
	void SetListener(EditorListener *Listener, const Ogre::Vector3 &InitializePosition);

	void SetTransformType(TRANSFORM_TYPE Type);
	
	TRANSFORM_TYPE GetTransformType(void)
	{
		return m_TransformType;
	}

	void SetTranslateType(TRANSLATE_TYPE Type)
	{
		m_TranslateType = Type;
	}
	
	TRANSLATE_TYPE GetTranslateType(void)
	{
		return m_TranslateType;
	}

private:
	void SetCurrentAxisMaterialToIdle(void);
	void SetCurrentAxisMaterialToSelected(void);

public:
	static const unsigned int GIZMO_FLAG;

private:
	Ogre::RaySceneQuery *m_RaySceneQuery;


	TRANSFORM_TYPE m_TransformType;
	TRANSLATE_TYPE m_TranslateType;

	EditorListener *m_EditorListener;

	Ogre::SceneNode *m_CurrentGizmoNode,
		*m_CurrentGizmoNodeBase,

		*m_GizmoTranslateNode,
		*m_GizmoTranslateNodeX,
		*m_GizmoTranslateNodeY,
		*m_GizmoTranslateNodeZ,
		
		*m_GizmoRotateNode,
		*m_GizmoRotateNodeX,
		*m_GizmoRotateNodeY,
		*m_GizmoRotateNodeZ,
		
		*m_GizmoScaleNode,
		*m_GizmoScaleNodeX,
		*m_GizmoScaleNodeY,
		*m_GizmoScaleNodeZ;

	Ogre::Vector3 m_GizmoSelectOffset;

	//Ogre::ManualObject *m_GizmoTranslateX,
	//	*m_GizmoTranslateY,
	//	*m_GizmoTranslateZ,

	//	*m_GizmoRotateX,
	//	*m_GizmoRotateY,
	//	*m_GizmoRotateZ,

	//	*m_GizmoScaleX,
	//	*m_GizmoScaleY,
	//	*m_GizmoScaleZ;

	Ogre::SceneNode *m_LinkedNodeForTrans;

	wxArrayPtrVoid m_DoList;
	unsigned int m_CurrentDoItem;
};

END_NAMESPACE

#endif