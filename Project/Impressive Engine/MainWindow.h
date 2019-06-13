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
#include <vector>
#include "Common.h"

BEGIN_NAMESPACE
class MainWindow : public wxFrame
{
public:
	MainWindow(void);
	~MainWindow(void);

	void Initialize(void);
	void CloseCommandFromException(void);
	
	void AddPaneInCenter(wxWindow *Pane, wxString Name, wxString Caption);
	void AddPane(wxWindow *Pane, wxString Name, wxString Caption, int Direction, unsigned int MinWidth = 0, unsigned int MinHeight = 0);
	
	void New(void);
	void Load(void);
	bool Save(void);
	void SaveAs(void);
	
	void ToggleMode(void);
	void ToggleGizmoType(void);

	void ShowWindow(const wxString &Name);

	void UpdatePanes(void);

	wxAuiPaneInfo &GetPane(const wxString &Name)
	{
		return m_FrameManager.GetPane(Name);
	}

	bool IsInEditMode(void)
	{
		return m_InEditMode;
	}

	static MainWindow *Create(void)
	{
		if (!m_ThisPointer)
			m_ThisPointer = new MainWindow();

		return MainWindow::GetPointer();
	}

	static MainWindow *GetPointer(void)
	{
		return m_ThisPointer;
	}

    inline wxAuiPaneInfo GetPane(wxString &name)
    {
        return m_FrameManager.GetPane(name);
    }

private:
	void InitializeGUI(void);
	void InitializeMenuBar(void);
    void InitializeStatusBar(void);
	void InitializeToolBar(void);
	void InitializeAUI(void);
	void SetAllPaneVisible(bool Value);
	void SetAllDockAlign(int Direction);
	void ShowGUI(void);
	void HideGUI(void);
	void RefreshPolygonMode(void);
	void RefreshCameraType(void);
	void RefreshTransformTool(void);
	void LoadCore(const wxString &FilePath);
	void SaveCore(const wxString &FilePath);
	wxString GetFileNameToSave(bool IsSaveAs);
	void RefreshTitle(void);

private:
    enum
    {
		ID_RENDER_TIMER = 1000,
        ID_MENU_NEW,
        ID_MENU_LOAD,
        ID_MENU_AUTO_LOAD,
        ID_MENU_SAVE,
        ID_MENU_SAVEAS,
        ID_MENU_EXPORT,
        ID_MENU_EXIT,

        ID_MENU_UNDO,
        ID_MENU_REDO,
        ID_MENU_REMOVE,
		
        ID_MENU_POLYGONMODE_SOLID,
        ID_MENU_POLYGONMODE_WIREFRAME,
        ID_MENU_POLYGONMODE_POINTS,

        ID_MENU_VIEWPORTCAMERA_FLY,
        ID_MENU_VIEWPORTCAMERA_TOP,
        ID_MENU_VIEWPORTCAMERA_LEFT,
        ID_MENU_VIEWPORTCAMERA_RIGHT,
        ID_MENU_VIEWPORTCAMERA_FRONT,
        ID_MENU_LOGS,
        ID_MENU_PROPERTIES,
        ID_MENU_ENTITY,
        ID_MENU_MATERIALEDITOR,
        ID_MENU_ANIMATION,
        ID_MENU_MODELEDITOR,
        ID_MENU_SCENE,
        ID_MENU_SCRIPT,
        ID_MENU_TOOLBAR,
        ID_MENU_SAVE_CURRENT_TEMPLATE,
        ID_MENU_DEFAULT_STYLE,
        ID_MENU_RIGHT_ALL_PANES,
        ID_MENU_LEFT_ALL_PANES,
        ID_MENU_TOP_ALL_PANES,
        ID_MENU_BOTTOM_ALL_PANES,

		ID_MENU_TERRAIN,

        ID_MENU_NAVMESHGEN,

		ID_TOOL_GIZMO_SELECT,
		ID_TOOL_GIZMO_TRANSLATE,
		ID_TOOL_GIZMO_ROTATE,
		ID_TOOL_GIZMO_SCALE,

		ID_TOOL_TRANSLATE_LOCAL,
		ID_TOOL_TRANSLATE_GLOBAL,
		
		ID_TOOL_PLAY_GAME,

        ID_MENU_ABOUT_EDITOR,


		ID_TIMER_AUTO_SAVE,

        ID_MENU_CUSTOM_PERSPECTIVE
    };

	DECLARE_EVENT_TABLE()
	
	void OnClose(wxCloseEvent& event);
    void OnUpdateUI(wxUpdateUIEvent& event);
    void OnMaximizePane(wxAuiManagerEvent& event);
    void OnRestorePane(wxAuiManagerEvent& event);
	//void OnKeyUp(wxKeyEvent &event);
	
	void OnTimer(wxTimerEvent &event);

	void MenuNew(wxCommandEvent &event);
	void MenuLoad(wxCommandEvent &event);
	void MenuAutoLoad(wxCommandEvent &event);
	void MenuSave(wxCommandEvent &event);
	void MenuSaveAs(wxCommandEvent &event);
	void MenuExport(wxCommandEvent &event);
	void MenuExit(wxCommandEvent &event);

	void OnAutoSave(wxTimerEvent &event);
	
	void MenuUndo(wxCommandEvent &event);
	void MenuRedo(wxCommandEvent &event);
	void MenuRemove(wxCommandEvent &event);
	
	void MenuPolygonMode(wxCommandEvent &event);
	void MenuViewPortCameraType(wxCommandEvent &event);
	void MenuLogs(wxCommandEvent &event);
	void MenuProperties(wxCommandEvent &event);
	void MenuEntity(wxCommandEvent &event);
	void MenuMaterialEditor(wxCommandEvent &event);
	void MenuAnimation(wxCommandEvent &event);
	void MenuModelEditor(wxCommandEvent &event);
	void MenuScene(wxCommandEvent &event);
	void MenuScript(wxCommandEvent &event);
	void MenuToolbar(wxCommandEvent &event);
    void MenuSaveTemplate(wxCommandEvent &event);
    void MenuLoadTemplate(wxCommandEvent &event);
	void MenuGizmoType(wxCommandEvent &event);
	void MenuTranslateType(wxCommandEvent &event);
	void MenuPlayGame(wxCommandEvent &event);
	void MenuDefaultStyle(wxCommandEvent &event);
	void MenuRightAllPanes(wxCommandEvent &event);
	void MenuLeftAllPanes(wxCommandEvent &event);
	void MenuTopAllPanes(wxCommandEvent &event);
	void MenuBottomAllPanes(wxCommandEvent &event);

	void MenuTerrain(wxCommandEvent &event);

    void MenuNavMeshGen(wxCommandEvent &event);
	
	void MenuAboutEditor(wxCommandEvent &event);

private:
	static MainWindow *m_ThisPointer;

	wxTimer *m_Timer;

	bool m_ExitReasonIsCrashing;

	wxTimer *m_AutoSaveTimer;

	wxAuiManager m_FrameManager;
	wxString m_DefaultStyle;
	wxMenuBar *m_MenuBar;
	wxMenu *m_MenuDisplay;
	wxAuiToolBar *m_ToolBar;
	bool m_InEditMode;
	//std::vector<bool> m_Windows;

	wxFileDialog *m_FileDialog;
	wxString m_LoadedScenePath;
    wxArrayString m_PerspectiveNames;
};
END_NAMESPACE

#endif