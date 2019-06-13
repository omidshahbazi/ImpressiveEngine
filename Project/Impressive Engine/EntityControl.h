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

#include "Common.h"
#include "StdAfxwxWidgets.h"
#include "StdAfxOGRE.h"

BEGIN_NAMESPACE
class EntityControl : public wxWindow
{
public:
	EntityControl(wxWindow *Parent);
	//~EntityControl(void);

	void ReloadContent(void);

private:
	void AddFolderContent(const char *Path, const wxTreeItemId &ID, const char *Filter = NULL);
//	bool IsTextureFile(Ogre::String Name);

	bool IsAnEntity(const wxTreeItemId &ID);

	wxString GetTypeAsString(const wxTreeItemId &ID);

private:
	DECLARE_EVENT_TABLE()

	enum
	{
		IDB_REFRESH = 1000,
		IDB_OPEN_EXTERNALLY,
		IDB_OPEN_ANIMATION,
		IDB_OPEN_MATERIAL,
		IDB_OPEN_MODEL,
		IDB_OPEN_SCRIPT,
		IDB_USE
	};

	void OnSize(wxSizeEvent &event);
	void OnSelectedItem(wxTreeEvent &event);
	void OnRefreshButton(wxCommandEvent &event);
	void OnOpenExternallyButton(wxCommandEvent &event);
	void OnOpenAnimationButton(wxCommandEvent &event);
	void OnOpenMaterialButton(wxCommandEvent &event);
	void OnOpenModelButton(wxCommandEvent &event);
	void OnOpenScriptButton(wxCommandEvent &event);
	void OnUseButton(wxCommandEvent &event);

private:
	wxToolBar *m_ToolBar;
	wxToolBarToolBase *m_OpenExternalTool;
	wxToolBarToolBase *m_OpenAnimationTool;
	wxToolBarToolBase *m_OpenMaterialTool;
	wxToolBarToolBase *m_OpenModelTool;
	wxToolBarToolBase *m_OpenScriptTool;
	wxToolBarToolBase *m_UseTool;
	wxTreeCtrl *m_Tree;
	//wxStaticBitmap *m_ImageViewer;
	wxTextCtrl *m_TextBox;

	wxTreeItemId m_RootID, m_Prefabs, m_MeshesID, m_AnimationID, m_TextureID, m_MaterialID, m_ScriptID;
};

END_NAMESPACE

#endif