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

#include "StdAfxwxWidgets.h"
#include "StdAfxOGRE.h"
#include "Properties.h"

BEGIN_NAMESPACE
class GameObject;


class SceneControl : public wxWindow, public IPropertiesListener
{
public:
	SceneControl(wxWindow *Parent);
	//~SceneControl(void);

	void SelectItem(Ogre::String Name);
	//void SelectItem(Ogre::String Name);

	void ReloadContent(void);
	void RemoveSelectedItem(void);

	void RefreshPropertyGrid(void);
    
private:
	//void AddGPUParameters(const Ogre::String &Category, Ogre::Pass *Pass, const Ogre::GpuNamedConstants &Params);

	void AddChildSceneContent(wxTreeItemId Parent, GameObject *Object);

	wxTreeItemId GetItem(const char *Text, wxTreeItemId &Parent);

	bool IsAnGameObject(const wxTreeItemId &ID);

	/*unsigned */int GetGameObjectID(const wxTreeItemId &ID);

    GameObject *GetSelectedObject(void);

private:
	DECLARE_EVENT_TABLE()

	enum
	{
		ID_ADD_GAMEOBJECT = 1000,
		ID_DELETE
	};

	void OnSize(wxSizeEvent &event);

	void OnKey(wxTreeEvent &event);
    void OnDoubleClick(wxTreeEvent &event);
	void OnSelectedItem(wxTreeEvent &event);
	//void OnBeginDrag(wxTreeEvent &event);
	//void OnEngDrag(wxTreeEvent &event);
	void OnShowEntities(wxCommandEvent &event);

	void OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event);
	
	void OnDeleteGameObject(wxCommandEvent &event);
	void OnAddGameObject(wxCommandEvent &event);

private:
	wxToolBar *m_ToolBar;
	wxTreeCtrl *m_Tree;
	wxTreeItemId m_RootID;
	
	wxTreeItemId m_DragItem;
	bool m_CutItem;
	wxTreeItemId m_ItemToMove;

	GameObject *m_LastSelectedGameObject;
	Ogre::Entity *m_LastSelectedEntity;
};
END_NAMESPACE

#endif