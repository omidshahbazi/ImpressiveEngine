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
#ifdef EDIT_MODE

#include "StdAfxwxWidgets.h"
#include "StdAfxOGRE.h"
#include "IPropertiesListener.h"

BEGIN_NAMESPACE
class RenderControl;


class ModelEditorControl : public wxWindow, public IPropertiesListener
{
public:
	ModelEditorControl(wxWindow *Parent);
	//~ModelEditorControl(void);

	void Initialize(void);

	void Update(void);

	void SelectMesh(const wxString &Name);

	static ModelEditorControl *Create(wxWindow *Parent)
	{
		if (!m_ThisPointer)
			m_ThisPointer = new ModelEditorControl(Parent);

		return m_ThisPointer;
	}

	static ModelEditorControl *GetPointer(void)
	{
		return m_ThisPointer;
	}

	wxString GetName(void)
	{
		return "Model Editor";
	}
	
	wxString GetCaption(void)
	{
		return "Model Editor";
	}

private:
	DECLARE_EVENT_TABLE()

	enum
	{
		IDB_SAVE = 1000
	};
	
	void RefreshProperties(void);

	void OnSize(wxSizeEvent &event);
	void OnSubMeshSelected(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);

	void OnPropertyChanged(/*wxPropertyGridManager *Manager, */const wxPropertyGridEvent &event);

private:
	static ModelEditorControl *m_ThisPointer;

	wxToolBar *m_ToolBar;
	wxChoice *m_SubMeshList;

	Ogre::SceneManager *m_SceneManager;
	Ogre::SceneNode *m_SceneNode;
	
	RenderControl *m_RenderControl;

	Ogre::SubEntity *m_LastSelectedSubMesh;
	Ogre::MaterialPtr m_LastSelectedMaterial;
};

END_NAMESPACE

#endif