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

#include "MainWindow.h"
#include "AboutWindow.h"
#include "TerrainWindow.h"
#include "NavGraphGenWindow.h"
#include "Engine.h"
#include "Log.h"
#include "Properties.h"
#include "ScenesHolder.h"
#include "Editor.h"
#include "Entity.h"
#include "AnimationControl.h"
#include "ScriptControl.h"
#include "IO.h"
#include "ViewPortsManager.h"
#include "RenderControl.h"
#include "RegistryKey.h"
#include "GameObject.h"
#include "CameraGameObject.h"
#include "MaterialEditorControl.h"
#include "ModelEditorControl.h"
#include "Utility.h"
#include "StatusBarControl.h"


BEGIN_NAMESPACE
#define BEGINE_PANE_ITERATOR(PaneList) int count = PaneList.GetCount(); \
	for (int i = 0; i < count; i++) \
	{ \
		wxAuiPaneInfo &Pane = PaneList.Item(i);

#define END_PANE_ITERATOR }


BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_CLOSE(MainWindow::OnClose)

	//EVT_KEY_UP(MainWindow::OnKeyUp)

	EVT_TIMER(ID_RENDER_TIMER, MainWindow::OnTimer)

	EVT_MENU(ID_MENU_NEW, MainWindow::MenuNew)
	EVT_MENU(ID_MENU_LOAD, MainWindow::MenuLoad)
	EVT_MENU(ID_MENU_AUTO_LOAD, MainWindow::MenuAutoLoad)
	EVT_MENU(ID_MENU_SAVE, MainWindow::MenuSave)
	EVT_MENU(ID_MENU_SAVEAS, MainWindow::MenuSaveAs)
	EVT_MENU(ID_MENU_EXPORT, MainWindow::MenuExport)
	EVT_MENU(ID_MENU_EXIT, MainWindow::MenuExit)

	EVT_MENU(ID_MENU_UNDO, MainWindow::MenuUndo)
	EVT_MENU(ID_MENU_REDO, MainWindow::MenuRedo)
	EVT_MENU(ID_MENU_REMOVE, MainWindow::MenuRemove)
	
	EVT_MENU(ID_MENU_POLYGONMODE_SOLID, MainWindow::MenuPolygonMode)
	EVT_MENU(ID_MENU_POLYGONMODE_WIREFRAME, MainWindow::MenuPolygonMode)
	EVT_MENU(ID_MENU_POLYGONMODE_POINTS, MainWindow::MenuPolygonMode)
	
	EVT_MENU(ID_MENU_VIEWPORTCAMERA_FLY, MainWindow::MenuViewPortCameraType)
	EVT_MENU(ID_MENU_VIEWPORTCAMERA_TOP, MainWindow::MenuViewPortCameraType)
	EVT_MENU(ID_MENU_VIEWPORTCAMERA_LEFT, MainWindow::MenuViewPortCameraType)
	EVT_MENU(ID_MENU_VIEWPORTCAMERA_RIGHT, MainWindow::MenuViewPortCameraType)
	EVT_MENU(ID_MENU_VIEWPORTCAMERA_FRONT, MainWindow::MenuViewPortCameraType)
	EVT_MENU(ID_MENU_LOGS, MainWindow::MenuLogs)
	EVT_MENU(ID_MENU_PROPERTIES, MainWindow::MenuProperties)
	EVT_MENU(ID_MENU_ENTITY, MainWindow::MenuEntity)
	EVT_MENU(ID_MENU_MATERIALEDITOR, MainWindow::MenuMaterialEditor)
	EVT_MENU(ID_MENU_ANIMATION, MainWindow::MenuAnimation)
	EVT_MENU(ID_MENU_MODELEDITOR, MainWindow::MenuModelEditor)
	EVT_MENU(ID_MENU_SCENE, MainWindow::MenuScene)
	EVT_MENU(ID_MENU_SCRIPT, MainWindow::MenuScript)
	EVT_MENU(ID_MENU_TOOLBAR, MainWindow::MenuToolbar)
    EVT_MENU(ID_MENU_SAVE_CURRENT_TEMPLATE, MainWindow::MenuSaveTemplate)
	EVT_MENU(ID_MENU_DEFAULT_STYLE, MainWindow::MenuDefaultStyle)
	EVT_MENU(ID_MENU_RIGHT_ALL_PANES, MainWindow::MenuRightAllPanes)
	EVT_MENU(ID_MENU_LEFT_ALL_PANES, MainWindow::MenuLeftAllPanes)
	EVT_MENU(ID_MENU_TOP_ALL_PANES, MainWindow::MenuTopAllPanes)
	EVT_MENU(ID_MENU_BOTTOM_ALL_PANES, MainWindow::MenuBottomAllPanes)
	
	EVT_MENU(ID_MENU_TERRAIN, MainWindow::MenuTerrain)

    EVT_MENU(ID_MENU_NAVMESHGEN, MainWindow::MenuNavMeshGen)

	EVT_MENU(ID_MENU_ABOUT_EDITOR, MainWindow::MenuAboutEditor)

	EVT_TIMER(ID_TIMER_AUTO_SAVE, MainWindow::OnAutoSave)
    
    EVT_MENU_RANGE(ID_MENU_CUSTOM_PERSPECTIVE, ID_MENU_CUSTOM_PERSPECTIVE + 10000, MainWindow::MenuLoadTemplate)
        
END_EVENT_TABLE()


MainWindow *MainWindow::m_ThisPointer = 0;


MainWindow::MainWindow(void) :
	wxFrame(0, -1, Engine::ENGINE_NAME),
	m_ExitReasonIsCrashing(false),
	m_FileDialog(NULL),
	m_LoadedScenePath("")
{
	m_Timer = new wxTimer(this, ID_RENDER_TIMER);
	m_Timer->Start(1);
}


MainWindow::~MainWindow(void)
{
	delete m_Timer;

	if (!m_InEditMode)
		ShowGUI();

	HideGUI();

	m_FrameManager.UnInit();
	//Destroy();
}


void MainWindow::Initialize(void)
{
	SetIcon(wxIcon(Engine::PATH_EDITOR_IMAGE + "Icon.png", wxBITMAP_TYPE_ANY));

	InitializeGUI();
	
	if (!Engine::GetInstance().Initialize())
	{
		return;
	}

	m_DefaultStyle = m_FrameManager.SavePerspective();

	ShowGUI();

	RefreshTransformTool();

	RefreshCameraType();

	wxString autoLoadFile = IO::GetInstance().GetEnvironmentRegistryKey()->GetValueAsString("AutoLoad");
	if (autoLoadFile.size() > 0)
		LoadCore(autoLoadFile);

	m_AutoSaveTimer = new wxTimer(this, ID_TIMER_AUTO_SAVE);
	m_AutoSaveTimer->Start(300000);
}


void MainWindow::CloseCommandFromException(void)
{
	m_ExitReasonIsCrashing = true;

	if (m_LoadedScenePath.size())
	{
		wxString path, fileName;
		wxSplitPath(m_LoadedScenePath, &path, &fileName, 0);

		m_LoadedScenePath = path + "\\" + fileName + "-AfterCrashed.s";
	}
	else
		m_LoadedScenePath = Engine::PATH_SCENE + "AfterCrashed.s";

	SaveCore(m_LoadedScenePath);

	Close(true);
}


void MainWindow::InitializeGUI(void)
{
	InitializeMenuBar();

	InitializeAUI();

	SetMinSize(wxSize(800, 600));

	Maximize();

	SetSize(GetSize());
    
    InitializeStatusBar();
}


void MainWindow::InitializeStatusBar(void)
{
    StatusBarControl::GetInstance().Initialize();
    SetStatusBar(StatusBarControl::GetInstance().GetControl());
}


void MainWindow::InitializeMenuBar(void)
{
	m_MenuBar = new wxMenuBar;

	wxMenu *menu = new wxMenu();
	m_MenuBar->Append(menu, "&File");
	menu->Append(ID_MENU_NEW, "&New");
	menu->AppendSeparator();
	menu->Append(ID_MENU_LOAD, "&Load");
	menu->Append(ID_MENU_AUTO_LOAD, "&Auto Load");

	menu->AppendSeparator();
	menu->Append(ID_MENU_SAVE, "&Save");
	menu->Append(ID_MENU_SAVEAS, "&Save As");
	menu->AppendSeparator();
	menu->Append(ID_MENU_EXPORT, "&Export");
	menu->AppendSeparator();
	menu->Append(ID_MENU_EXIT, "&Exit");

	menu = new wxMenu();
	m_MenuBar->Append(menu, "&Edit");
	menu->Append(ID_MENU_UNDO, "&Undo");
	menu->Append(ID_MENU_REDO, "&Redo");
	menu->AppendSeparator();
	menu->Append(ID_MENU_REMOVE, "Re&move");

	m_MenuDisplay = new wxMenu();
	m_MenuBar->Append(m_MenuDisplay, "&Display");
	
	wxMenu *polygonMode = new wxMenu();
	polygonMode->Append(ID_MENU_POLYGONMODE_SOLID, "&Solid", "", true);
	polygonMode->Append(ID_MENU_POLYGONMODE_WIREFRAME, "&Wireframe", "", true);
	polygonMode->Append(ID_MENU_POLYGONMODE_POINTS, "&Points", "", true);

	m_MenuDisplay->Append(-1, "Polygon Mode", polygonMode);
	
	m_MenuDisplay->AppendSeparator();

	wxMenu *cameras = new wxMenu();
	cameras->Append(ID_MENU_VIEWPORTCAMERA_FLY, "Fly", "", true);
	cameras->Append(ID_MENU_VIEWPORTCAMERA_TOP, "Top", "", true);
	cameras->Append(ID_MENU_VIEWPORTCAMERA_LEFT, "Left", "", true);
	cameras->Append(ID_MENU_VIEWPORTCAMERA_RIGHT, "Right", "", true);
	cameras->Append(ID_MENU_VIEWPORTCAMERA_FRONT, "Front", "", true);

	m_MenuDisplay->Append(-1, "View-Port Camera", cameras);

	m_MenuDisplay->AppendSeparator();

    m_MenuDisplay->Append(ID_MENU_LOGS, Log::GetInstance().GetCaption()/*"&Logs"*/, "", true);
    m_MenuDisplay->Append(ID_MENU_PROPERTIES, Properties::GetInstance().GetCaption()/*"&Properties"*/, "" , true);
    m_MenuDisplay->Append(ID_MENU_ENTITY, Entity::GetInstance().GetCaption()/*"&Entity"*/, "" , true);
    m_MenuDisplay->Append(ID_MENU_MATERIALEDITOR, MaterialEditorControl::GetPointer()->GetCaption()/*"&Material Editor"*/, "" , true);
    m_MenuDisplay->Append(ID_MENU_ANIMATION, AnimationControl::GetPointer()->GetCaption()/*"&Animation"*/, "" , true);
    m_MenuDisplay->Append(ID_MENU_MODELEDITOR, ModelEditorControl::GetPointer()->GetCaption(), "" , true);
    m_MenuDisplay->Append(ID_MENU_SCENE, ScenesHolder::GetInstance().GetCaption()/*"&Scene"*/, "" , true);
    m_MenuDisplay->Append(ID_MENU_SCRIPT, ScriptControl::GetPointer()->GetCaption()/*"S&cript"*/, "", true);
    
	m_MenuDisplay->AppendSeparator();
	
    m_MenuDisplay->Append(ID_MENU_TOOLBAR, "&Toolbar", "" , true);
    
	m_MenuDisplay->AppendSeparator();

    m_MenuDisplay->Append(ID_MENU_SAVE_CURRENT_TEMPLATE, "&Save Current Template");

    m_MenuDisplay->AppendSeparator();

	m_MenuDisplay->Append(ID_MENU_DEFAULT_STYLE, "&Default Style");
	m_MenuDisplay->Append(ID_MENU_RIGHT_ALL_PANES, "&Right All Panes");
	m_MenuDisplay->Append(ID_MENU_LEFT_ALL_PANES, "&Left All Panes");
	m_MenuDisplay->Append(ID_MENU_TOP_ALL_PANES, "&Top All Panes");
	m_MenuDisplay->Append(ID_MENU_BOTTOM_ALL_PANES, "&Bottom All Panes");

	menu = new wxMenu();
	m_MenuBar->Append(menu, "&Object");

	menu->Append(ID_TOOL_PLAY_GAME, "&Camera");
	menu->Append(ID_TOOL_PLAY_GAME, "&Light");

	menu->AppendSeparator();

	menu->Append(ID_MENU_TERRAIN, "&Terrain");

	menu->AppendSeparator();

    menu = new wxMenu();
    m_MenuBar->Append(menu,"&AI");

    menu->Append(ID_MENU_NAVMESHGEN, "&Navigation Graph Generator");

	menu = new wxMenu();
	m_MenuBar->Append(menu, "&Game");

	menu->Append(ID_TOOL_PLAY_GAME, "&Play");

	menu = new wxMenu();
	m_MenuBar->Append(menu, "&Help");
	menu->Append(ID_MENU_ABOUT_EDITOR, "&About Editor");
}


void MainWindow::InitializeToolBar(void)
{
    m_ToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORZ_LAYOUT);
    m_ToolBar->SetName("Toolbar");
    m_ToolBar->SetToolBitmapSize(wxSize(16,16));
		
    m_ToolBar->AddTool(ID_MENU_LOAD, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "OpenLarge.png", wxBITMAP_TYPE_ANY), "Load", wxITEM_NORMAL);
	m_ToolBar->AddTool(ID_MENU_SAVE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "SaveLarge.png", wxBITMAP_TYPE_ANY), "Save", wxITEM_NORMAL);
	m_ToolBar->AddSeparator();
	m_ToolBar->AddTool(ID_MENU_UNDO, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Undo.png", wxBITMAP_TYPE_ANY), "Undo", wxITEM_NORMAL);
	m_ToolBar->AddTool(ID_MENU_REDO, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Redo.png", wxBITMAP_TYPE_ANY), "Redo", wxITEM_NORMAL);
	m_ToolBar->AddSeparator();
	m_ToolBar->AddTool(ID_TOOL_GIZMO_SELECT, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Arrow.png", wxBITMAP_TYPE_ANY), "Select", wxITEM_CHECK);
	m_ToolBar->AddTool(ID_TOOL_GIZMO_TRANSLATE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "GizmoTranslate.png", wxBITMAP_TYPE_ANY), "Translate", wxITEM_CHECK);
	m_ToolBar->AddTool(ID_TOOL_GIZMO_ROTATE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "GizmoRotate.png", wxBITMAP_TYPE_ANY), "Rotate", wxITEM_CHECK);
	m_ToolBar->AddTool(ID_TOOL_GIZMO_SCALE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "GizmoScale.png", wxBITMAP_TYPE_ANY), "Scale", wxITEM_CHECK);
	m_ToolBar->AddSeparator();
	m_ToolBar->AddTool(ID_TOOL_TRANSLATE_LOCAL, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "TranslateLocal.png", wxBITMAP_TYPE_ANY), "Local Translation", wxITEM_CHECK);
	m_ToolBar->AddTool(ID_TOOL_TRANSLATE_GLOBAL, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "TranslateGlobal.png", wxBITMAP_TYPE_ANY), "Global Translation", wxITEM_CHECK);
	m_ToolBar->AddSeparator();
    m_ToolBar->AddTool(ID_TOOL_PLAY_GAME, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Play.png", wxBITMAP_TYPE_ANY), "Play Game", wxITEM_NORMAL);

	m_ToolBar->Realize();
	
	Connect(ID_TOOL_GIZMO_SELECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::MenuGizmoType));
	Connect(ID_TOOL_GIZMO_TRANSLATE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::MenuGizmoType));
	Connect(ID_TOOL_GIZMO_ROTATE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::MenuGizmoType));
	Connect(ID_TOOL_GIZMO_SCALE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::MenuGizmoType));
	Connect(ID_TOOL_TRANSLATE_LOCAL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::MenuTranslateType));
	Connect(ID_TOOL_TRANSLATE_GLOBAL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::MenuTranslateType));
	Connect(ID_TOOL_PLAY_GAME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::MenuPlayGame));

	m_FrameManager.AddPane(m_ToolBar, wxAuiPaneInfo()
        .Name(m_ToolBar->GetName())
        .ToolbarPane()
        .Caption("Toolbar")
        .Top()
        .Row(1));
    
}


void MainWindow::InitializeAUI(void)
{
    m_FrameManager.SetManagedWindow(this);
	//m_FrameManager.SetFlags(m_FrameManager.GetFlags() ^ wxAUI_MGR_TRANSPARENT_DRAG);
	m_FrameManager.SetFlags(wxAUI_MGR_DEFAULT /*| wxAUI_MGR_ALLOW_ACTIVE_PANE */| wxAUI_MGR_TRANSPARENT_DRAG);
	
	wxAuiDockArt *art = m_FrameManager.GetArtProvider();
    art->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE, 1);
    art->SetMetric(wxAUI_DOCKART_SASH_SIZE, 4);
    art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE, 17);
    art->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, wxColour(220, 226, 243));
	//art->SetColour(wxAUI_ART_ACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(90, 98, 110));
    art->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR, wxColour(0, 0, 0));
	//art->SetColour(wxAUI_ART_INACTIVE_CAPTION_COLOUR, wxColour(211, 218, 237));
	//art->SetColour(wxAUI_ART_INACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(225, 230, 245));
	//art->SetColour(wxAUI_ART_INACTIVE_CAPTION_TEXT_COLOUR, wxColour(0, 0, 0));

	Log::GetInstance().Initialize(this);
	Properties::GetInstance().Initialize(this);
	ScenesHolder::GetInstance().Initialize(this);
	Entity::GetInstance().Initialize(this);
	MaterialEditorControl::Create(this)/*->Initialize()*/;
	ModelEditorControl::Create(this);
	AnimationControl::Create(this);
	//ViewPortsManager::GetInstance().Initialize(this);
	ScriptControl::Create(this)->Initialize();


	AddPane(Log::GetInstance().GetControl(), Log::GetInstance().GetName(), Log::GetInstance().GetCaption(), wxAUI_DOCK_BOTTOM, 200, 50);
	AddPane(Properties::GetInstance().GetControl(), Properties::GetInstance().GetName(), Properties::GetInstance().GetCaption(), wxAUI_DOCK_RIGHT, 200, 100);
	AddPane(ScenesHolder::GetInstance().GetControl(), ScenesHolder::GetInstance().GetName(), ScenesHolder::GetInstance().GetCaption(), wxAUI_DOCK_RIGHT, 200, 100);
	AddPane(Entity::GetInstance().GetControl(), Entity::GetInstance().GetName(), Entity::GetInstance().GetCaption(), wxAUI_DOCK_RIGHT, 200, 100);
	AddPane(MaterialEditorControl::GetPointer(), MaterialEditorControl::GetPointer()->GetName(), MaterialEditorControl::GetPointer()->GetCaption(), wxAUI_DOCK_LEFT, 200, 100);
	AddPane(ModelEditorControl::GetPointer(), ModelEditorControl::GetPointer()->GetName(), ModelEditorControl::GetPointer()->GetCaption(), wxAUI_DOCK_LEFT, 200, 100);
	AddPane(AnimationControl::GetPointer(), AnimationControl::GetPointer()->GetName(), AnimationControl::GetPointer()->GetCaption(), wxAUI_DOCK_LEFT, 200, 100);
	AddPane(ScriptControl::GetPointer(), ScriptControl::GetPointer()->GetName(), ScriptControl::GetPointer()->GetCaption(), wxAUI_DOCK_LEFT, 200, 100);

    m_FrameManager.Connect(wxEVT_UPDATE_UI,wxUpdateUIEventHandler(MainWindow::OnUpdateUI));
    m_FrameManager.Connect(wxEVT_AUI_PANE_MAXIMIZE,wxAuiManagerEventHandler(MainWindow::OnMaximizePane));
    m_FrameManager.Connect(wxEVT_AUI_PANE_RESTORE,wxAuiManagerEventHandler(MainWindow::OnRestorePane));
	InitializeToolBar();
}


void MainWindow::AddPaneInCenter(wxWindow *Pane, wxString Name, wxString Caption)
{
	wxAuiPaneInfo &paneInfo = wxAuiPaneInfo().CenterPane();
	paneInfo.Name(Name);
	paneInfo.Caption(Caption);
	paneInfo.CaptionVisible();

	m_FrameManager.AddPane(Pane, paneInfo);
	
	UpdatePanes();
}


void MainWindow::AddPane(wxWindow *Pane, wxString Name, wxString Caption, int Direction, unsigned int MinWidth, unsigned int MinHeight)
{
	wxAuiPaneInfo paneInfo;
	paneInfo.Name(Name);
	paneInfo.Caption(Caption);
	paneInfo.MinSize(MinWidth, MinHeight);
	paneInfo.Direction(Direction);
    paneInfo.MaximizeButton();
    paneInfo.DestroyOnClose(false);
    paneInfo.Layer(1);

	m_FrameManager.AddPane(Pane, paneInfo);
	
	UpdatePanes();
}

void MainWindow::New(void)
{
	m_LoadedScenePath = "";

	ScenesHolder::GetInstance().GetCurrentScene()->Clear();
		
	RefreshTitle();
}


void MainWindow::Load(void)
{
	m_FileDialog = new wxFileDialog(this, "Load Scene", Engine::PATH_SCENE, "", "Scene File|*.s", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (m_FileDialog->ShowModal() != wxID_CANCEL)
	{
		if (wxMessageBox("Do you want to save current scene before loading another one ?", "Scene Saveing", wxYES_NO, this) == wxYES)
			Save();

		LoadCore(m_FileDialog->GetPath());
	}

	delete m_FileDialog;
	m_FileDialog = 0;
}


bool MainWindow::Save(void)
{
	if (m_LoadedScenePath.size() == 0)
	{
		m_LoadedScenePath = GetFileNameToSave(false);
	}

	if (m_LoadedScenePath.size() > 0)
	{
		SaveCore(m_LoadedScenePath);
		
		RefreshTitle();

		return true;
	}

	MaterialEditorControl::GetPointer()->Save();

	return false;
}


void MainWindow::SaveAs(void)
{
	m_LoadedScenePath = GetFileNameToSave(true);

	if (m_LoadedScenePath.size() > 0)
	{
		SaveCore(m_LoadedScenePath);
		
		RefreshTitle();
	}
}


void MainWindow::ToggleMode(void)
{
	if (m_InEditMode)
	{
		m_InEditMode = false;

		HideGUI();
		
		GetPane(ViewPortsManager::GetInstance().GetGameRenderControl()->GetName()).Show();

		ScenesHolder::GetInstance().GetCurrentScene()->PlayGame();
	}
	else
	{
		m_InEditMode = true;

		ScenesHolder::GetInstance().GetCurrentScene()->StopGame();

		GetPane(ViewPortsManager::GetInstance().GetGameRenderControl()->GetName()).Hide();

		ShowGUI();
	}
	
	//ShowCursor(m_InEditMode);
	ShowCursor(true);

	UpdatePanes();
}


void MainWindow::ToggleGizmoType(void)
{
	int type = Editor::GetInstance().GetTransformType();
	
	type++;

	if (type == Editor::TT_COUNT)
		type = Editor::TT_NONE;

	Editor::GetInstance().SetTransformType((Editor::TRANSFORM_TYPE)type);

	RefreshTransformTool();
}


void MainWindow::ShowWindow(const wxString &Name)
{
	wxAuiPaneInfo &info = m_FrameManager.GetPane(Name);

	if (info.IsShown())
	{
		info.Hide();
        
		UpdatePanes();
	}
    else
    {
        info.Show();
        
		UpdatePanes();
    }
}


void MainWindow::UpdatePanes(void)
{
	try
	{
		m_FrameManager.Update();
   
	}
	catch (...)
	{
		Log::GetInstance().AddError("dffsd");
	}    
}


void MainWindow::SetAllPaneVisible(bool Value)
{
	//if (!Value)
	//	m_Windows.clear();

	//int i = 0;

	BEGINE_PANE_ITERATOR(m_FrameManager.GetAllPanes())
		if (!ViewPortsManager::GetInstance().IsGameViewPort(Pane.window))
		{
			if (Value)
			{
				//Pane.Show(m_Windows[i]);
				Pane.Show();
                
				//i++;
			}
			else
			{
				//m_Windows.push_back(Pane.IsShown());

				Pane.Hide();
			}
		}
	END_PANE_ITERATOR

	
	UpdatePanes();
}


void MainWindow::SetAllDockAlign(int Direction)
{
	BEGINE_PANE_ITERATOR(m_FrameManager.GetAllPanes())
		if (!ViewPortsManager::GetInstance().IsEditViewPort(Pane.window) && !Pane.IsToolbar())
			Pane.Direction(Direction);
	END_PANE_ITERATOR

	UpdatePanes();
}


void MainWindow::ShowGUI(void)
{
	SetMenuBar(m_MenuBar);
	
	SetAllPaneVisible(true);

	wxString aui = IO::GetInstance().GetEnvironmentRegistryKey()->GetValueAsString("AUI");
    
	try
	{
        if (aui.size() > 0)
        {
			m_FrameManager.LoadPerspective(aui);
            UpdatePanes();
        }
		else
			UpdatePanes();
	}
	catch (...)
	{
		Log::GetInstance().AddError("Can not Initilize wxAui manager");
	}

    m_PerspectiveNames = IO::GetInstance().GetEnvironmentRegistryKey()->GetValueAsArrayString("Perspectives");
    if (m_PerspectiveNames.GetCount() > 0)
    {
        m_MenuDisplay->AppendSeparator();
        for (unsigned int i = 0; i < m_PerspectiveNames.GetCount(); i++)
        {
            m_MenuDisplay->Append(ID_MENU_CUSTOM_PERSPECTIVE + i, m_PerspectiveNames.Item(i));
        }
    }
}


void MainWindow::HideGUI(void)
{
	SetMenuBar(0);

    //StatusBarControl::GetInstance().GetControl()->Hide();

	IO::GetInstance().GetEnvironmentRegistryKey()->SetValue("AUI", m_FrameManager.SavePerspective().mb_str());
	
	SetAllPaneVisible(false);
}


void MainWindow::RefreshPolygonMode(void)
{
	int currType = ViewPortsManager::GetInstance().GetCurrentCamera()->GetPolygonMode();
	const int displayMenuIndex = 2;

	m_MenuDisplay->FindItem(ID_MENU_POLYGONMODE_SOLID)->Check(currType == Ogre::PM_SOLID);
	m_MenuDisplay->FindItem(ID_MENU_POLYGONMODE_WIREFRAME)->Check(currType == Ogre::PM_WIREFRAME);
	m_MenuDisplay->FindItem(ID_MENU_POLYGONMODE_POINTS)->Check(currType == Ogre::PM_POINTS);
}


void MainWindow::RefreshCameraType(void)
{
	int currType = ViewPortsManager::GetInstance().GetCurrentCamera()->GetType();
	const int displayMenuIndex = 2;

	m_MenuDisplay->FindItem(ID_MENU_VIEWPORTCAMERA_FLY)->Check(currType == EditorCamera::ECT_FLY);
	m_MenuDisplay->FindItem(ID_MENU_VIEWPORTCAMERA_TOP)->Check(currType == EditorCamera::ECT_TOP);
	m_MenuDisplay->FindItem(ID_MENU_VIEWPORTCAMERA_LEFT)->Check(currType == EditorCamera::ECT_LEFT);
	m_MenuDisplay->FindItem(ID_MENU_VIEWPORTCAMERA_RIGHT)->Check(currType == EditorCamera::ECT_RIGHT);
	m_MenuDisplay->FindItem(ID_MENU_VIEWPORTCAMERA_FRONT)->Check(currType == EditorCamera::ECT_FRONT);

	RefreshPolygonMode();
}


void MainWindow::RefreshTransformTool(void)
{
	m_ToolBar->ToggleTool(ID_TOOL_GIZMO_SELECT, (Editor::GetInstance().GetTransformType() == Editor::TT_NONE));
	m_ToolBar->ToggleTool(ID_TOOL_GIZMO_TRANSLATE, (Editor::GetInstance().GetTransformType() == Editor::TT_TRANSLATE));
	m_ToolBar->ToggleTool(ID_TOOL_GIZMO_ROTATE, (Editor::GetInstance().GetTransformType() == Editor::TT_ROTATE));
	m_ToolBar->ToggleTool(ID_TOOL_GIZMO_SCALE, (Editor::GetInstance().GetTransformType() == Editor::TT_SCALE));
	
	m_ToolBar->ToggleTool(ID_TOOL_TRANSLATE_LOCAL, (Editor::GetInstance().GetTranslateType() == Editor::TT_LOCAL));
	m_ToolBar->ToggleTool(ID_TOOL_TRANSLATE_GLOBAL, (Editor::GetInstance().GetTranslateType() == Editor::TT_GLOBAL));
}


void MainWindow::LoadCore(const wxString &FilePath)
{
	m_LoadedScenePath = FilePath;

	ScenesHolder::GetInstance().GetCurrentScene()->Load(STRING_WX_TO_OGRE(m_LoadedScenePath));
		
	RefreshTitle();
}
	

void MainWindow::SaveCore(const wxString &FilePath)
{
	ScriptControl::GetPointer()->SaveAll();

	//MaterialEditorControl::GetPointer()->Save();

	EditorScene *scene = ScenesHolder::GetInstance().GetCurrentScene();
	if (scene)
	{
		scene->Save(STRING_WX_TO_OGRE(FilePath));
		scene = NULL;
	}
}


wxString MainWindow::GetFileNameToSave(bool IsSaveAs)
{
	wxString filePath = "";

	m_FileDialog = new wxFileDialog(this, wxString("Save Scene") + (IsSaveAs ? " As" : ""), Engine::PATH_SCENE, "", "Scene File|*.s", wxFD_SAVE);

	if (m_FileDialog->ShowModal() != wxID_CANCEL)
	{
		filePath = m_FileDialog->GetPath();
	}
		
	delete m_FileDialog;
	m_FileDialog = 0;

	return filePath;
}


void MainWindow::RefreshTitle(void)
{
	SetTitle(Engine::ENGINE_NAME + (m_LoadedScenePath.size() > 0 ? " [" + wxFileNameFromPath(m_LoadedScenePath) + "]" : ""));
}


void MainWindow::OnClose(wxCloseEvent& event)
{
	if (!m_InEditMode)
	{
		event.StopPropagation();
		return;
	}

	if (!m_ExitReasonIsCrashing)
	{
		int result = wxMessageBox("Would you like to save scene, before exiting engine ?", "Save And Exit", wxYES_NO | wxCANCEL, this);

		if (result == wxYES)
		{
			if (!Save())
				return;
		}
		else if (result == wxCANCEL)
		{
			event.StopPropagation();
			return;
		}

		Engine::GetInstance().Shutdown();
	}

	Destroy();
}


void MainWindow::OnUpdateUI(wxUpdateUIEvent& event)
{
    MainWindow *mainWin = MainWindow::GetPointer();
    
    mainWin->m_MenuDisplay->Check(ID_MENU_LOGS,mainWin->m_FrameManager.GetPane(Log::GetInstance().GetName()).IsShown());
    mainWin->m_MenuDisplay->Check(ID_MENU_PROPERTIES,mainWin->m_FrameManager.GetPane(Properties::GetInstance().GetName()).IsShown());
	mainWin->m_MenuDisplay->Check(ID_MENU_ENTITY,mainWin->m_FrameManager.GetPane(Entity::GetInstance().GetName()).IsShown());
	mainWin->m_MenuDisplay->Check(ID_MENU_MATERIALEDITOR,mainWin->m_FrameManager.GetPane(MaterialEditorControl::GetPointer()->GetName()).IsShown());
	mainWin->m_MenuDisplay->Check(ID_MENU_ANIMATION,mainWin->m_FrameManager.GetPane(AnimationControl::GetPointer()->GetName()).IsShown());
	mainWin->m_MenuDisplay->Check(ID_MENU_MODELEDITOR,mainWin->m_FrameManager.GetPane(ModelEditorControl::GetPointer()->GetName()).IsShown());
	mainWin->m_MenuDisplay->Check(ID_MENU_MODELEDITOR,mainWin->m_FrameManager.GetPane(ModelEditorControl::GetPointer()->GetName()).IsShown());
	mainWin->m_MenuDisplay->Check(ID_MENU_SCENE,mainWin->m_FrameManager.GetPane(ScenesHolder::GetInstance().GetName()).IsShown());
	mainWin->m_MenuDisplay->Check(ID_MENU_SCRIPT,mainWin->m_FrameManager.GetPane(ScriptControl::GetPointer()->GetName()).IsShown());
    mainWin->m_MenuDisplay->Check(ID_MENU_TOOLBAR,mainWin->m_FrameManager.GetPane(mainWin->m_ToolBar->GetName()).IsShown());
    StatusBarControl::GetInstance().HideIcons();
}


void MainWindow::OnMaximizePane(wxAuiManagerEvent& event)
{
    
}

void MainWindow::OnRestorePane(wxAuiManagerEvent& event)
{
    /*MainWindow *mainWin = MainWindow::GetPointer();
    BEGINE_PANE_ITERATOR(mainWin->m_FrameManager.GetAllPanes())
        if (ViewPortsManager::GetInstance().IsEditViewPort(Pane.window) || ViewPortsManager::GetInstance().IsGameViewPort(Pane.window))
            Pane.Show();
	END_PANE_ITERATOR*/
 //   MainWindow *mainWin = MainWindow::GetPointer();
 //   BEGINE_PANE_ITERATOR(mainWin->m_FrameManager.GetAllPanes())
	//	if (!ViewPortsManager::GetInstance().IsEditViewPort(Pane.window) && !Pane.IsToolbar() && !ViewPortsManager::GetInstance().IsGameViewPort(Pane.window))
 //       {
 //           wxString aui = IO::GetInstance().GetEnvironmentRegistryKey()->GetValueAsString(Pane.name);
 //   
	//        try
	//        {
 //               if (aui.size() > 0)
 //               {
	//		        mainWin->m_FrameManager.LoadPaneInfo(aui,Pane);
 //                   //UpdatePanes();
 //               }
	//	        //else
	//		        //UpdatePanes();
	//        }
	//        catch (...)
	//        {
	//	        Log::GetInstance().AddError("Can not Initilize wxAui manager");
	//        }
 //       }
	//END_PANE_ITERATOR
}


//void MainWindow::OnKeyUp(wxKeyEvent &event)
//{
//	SwitchMode();
//}


void MainWindow::OnTimer(wxTimerEvent &event)
{
	Engine::GetInstance().Tick();
}


void MainWindow::MenuNew(wxCommandEvent &event)
{
	New();
}


void MainWindow::MenuLoad(wxCommandEvent &event)
{
	Load();
}


void MainWindow::MenuLoadTemplate(wxCommandEvent &event)
{
    if (m_PerspectiveNames.GetCount() > 0)
        m_FrameManager.LoadPerspective(IO::GetInstance().GetEnvironmentRegistryKey()->GetValueAsString(m_PerspectiveNames.Item(event.GetId() - ID_MENU_CUSTOM_PERSPECTIVE).mb_str()));
}


void MainWindow::MenuAutoLoad(wxCommandEvent &event)
{
	wxString filePath = "";

	m_FileDialog = new wxFileDialog(this, "Auto Load Scene", Engine::PATH_SCENE, "", "Scene File|*.s", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (m_FileDialog->ShowModal() != wxID_CANCEL)
	{
		filePath = m_FileDialog->GetPath();

		if (wxMessageBox("Would you like to load selected scene file ?", "Auto Load", wxYES_NO, this) == wxYES)
		{
			LoadCore(filePath);
		}
	}

	delete m_FileDialog;
	m_FileDialog = 0;
	
	IO::GetInstance().GetEnvironmentRegistryKey()->SetValue("AutoLoad", filePath.c_str());
}


void MainWindow::MenuSave(wxCommandEvent &event)
{
	Save();
}


void MainWindow::MenuSaveAs(wxCommandEvent &event)
{
	SaveAs();
}


void MainWindow::MenuExport(wxCommandEvent &event)
{
	m_FileDialog = new wxFileDialog(this, "Scenes to export", Engine::PATH_SCENE, "", "Scene File|*.s", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

	if (m_FileDialog->ShowModal() != wxID_CANCEL)
	{
		std::vector<Ogre::String> scenes;

		wxArrayString files;
		m_FileDialog->GetPaths(files);
		
		if (!files.size())
		{
			Log::GetInstance().AddError("There's no scene file to export");
			goto Cleanup;
		}

		for (unsigned int i = 0; i <  files.size(); i++)
			scenes.push_back(STRING_WX_TO_OGRE(files[i]));

		files.Clear();

		if (m_LoadedScenePath.size())
			Save();

		if (Editor::GetInstance().ExportGame(STRING_WX_TO_OGRE(m_LoadedScenePath), scenes))
			Log::GetInstance().AddInfo("Game successfully exported");
		else
			Log::GetInstance().AddError("Game exporting has an error");

		scenes.clear();
	}

Cleanup:
	delete m_FileDialog;
	m_FileDialog = NULL;
}


void MainWindow::MenuExit(wxCommandEvent &event)
{
	Close();
}


void MainWindow::OnAutoSave(wxTimerEvent &event)
{
	if (m_LoadedScenePath.size() > 0 && m_InEditMode)
	{
		wxString path, name, ext;
		wxSplitPath(m_LoadedScenePath, &path, &name, &ext);

		SaveCore(path + "\\AutoSave\\" + name + "." + ext);
	}
}


void MainWindow::MenuUndo(wxCommandEvent &event)
{
	Editor::GetInstance().DoUndo();
}


void MainWindow::MenuRedo(wxCommandEvent &event)
{
	Editor::GetInstance().DoRedo();
}


void MainWindow::MenuRemove(wxCommandEvent &event)
{
	ScenesHolder::GetInstance().GetCurrentScene()->RemoveSelectedGameObject();
}


void MainWindow::MenuPolygonMode(wxCommandEvent &event)
{
	switch (event.GetId())
	{
	case ID_MENU_POLYGONMODE_SOLID:
		{
			ViewPortsManager::GetInstance().GetCurrentCamera()->SetPolygonMode(Ogre::PM_SOLID);
		} break;

	case ID_MENU_POLYGONMODE_WIREFRAME:
		{
			ViewPortsManager::GetInstance().GetCurrentCamera()->SetPolygonMode(Ogre::PM_WIREFRAME);
		} break;
		
	case ID_MENU_POLYGONMODE_POINTS:
		{
			ViewPortsManager::GetInstance().GetCurrentCamera()->SetPolygonMode(Ogre::PM_POINTS);
		} break;
	}

	RefreshCameraType();
}


void MainWindow::MenuViewPortCameraType(wxCommandEvent &event)
{
	switch (event.GetId())
	{
	case ID_MENU_VIEWPORTCAMERA_FLY:
		{
			ViewPortsManager::GetInstance().SetViewPortCamera(EditorCamera::ECT_FLY);
		} break;

	case ID_MENU_VIEWPORTCAMERA_TOP:
		{
			ViewPortsManager::GetInstance().SetViewPortCamera(EditorCamera::ECT_TOP);
		} break;
		
	case ID_MENU_VIEWPORTCAMERA_LEFT:
		{
			ViewPortsManager::GetInstance().SetViewPortCamera(EditorCamera::ECT_LEFT);
		} break;
		
	case ID_MENU_VIEWPORTCAMERA_RIGHT:
		{
			ViewPortsManager::GetInstance().SetViewPortCamera(EditorCamera::ECT_RIGHT);
		} break;
		
	case ID_MENU_VIEWPORTCAMERA_FRONT:
		{
			ViewPortsManager::GetInstance().SetViewPortCamera(EditorCamera::ECT_FRONT);
		} break;
	}

	RefreshCameraType();
}


void MainWindow::MenuLogs(wxCommandEvent &event)
{
	ShowWindow(Log::GetInstance().GetName());
}


void MainWindow::MenuProperties(wxCommandEvent &event)
{
	ShowWindow(Properties::GetInstance().GetName());
}


void MainWindow::MenuEntity(wxCommandEvent &event)
{
	ShowWindow(Entity::GetInstance().GetName());
}


void MainWindow::MenuMaterialEditor(wxCommandEvent &event)
{
	ShowWindow(MaterialEditorControl::GetPointer()->GetName());
}


void MainWindow::MenuAnimation(wxCommandEvent &event)
{
	ShowWindow(AnimationControl::GetPointer()->GetName());
}


void MainWindow::MenuModelEditor(wxCommandEvent &event)
{
	ShowWindow(ModelEditorControl::GetPointer()->GetName());
}


void MainWindow::MenuScene(wxCommandEvent &event)
{
	ShowWindow(ScenesHolder::GetInstance().GetName());
}


void MainWindow::MenuScript(wxCommandEvent &event)
{
	ShowWindow(ScriptControl::GetPointer()->GetName());
}


void MainWindow::MenuToolbar(wxCommandEvent &event)
{
	ShowWindow(m_ToolBar->GetName());
}

void MainWindow::MenuSaveTemplate(wxCommandEvent &event)
{
	wxTextEntryDialog dlg_TemplateName(this, wxT("Enter a name for your template"), wxT("Template Name"));
    if (dlg_TemplateName.ShowModal() != wxID_OK)
        return;
    if (dlg_TemplateName.GetValue() == "")
    {
        Log::GetInstance().AddError("Please Enter a name for your template");
        return;
    }
    m_PerspectiveNames.Add(dlg_TemplateName.GetValue());
    if (m_PerspectiveNames.GetCount() == 1)
        m_MenuDisplay->AppendSeparator();
    m_MenuDisplay->Append(ID_MENU_CUSTOM_PERSPECTIVE + m_PerspectiveNames.GetCount() - 1, dlg_TemplateName.GetValue());
    IO::GetInstance().GetEnvironmentRegistryKey()->SetValue(dlg_TemplateName.GetValue().mb_str(), m_FrameManager.SavePerspective().mb_str());
    IO::GetInstance().GetEnvironmentRegistryKey()->SetValue("Perspectives",m_PerspectiveNames);

}


void MainWindow::MenuGizmoType(wxCommandEvent &event)
{
	switch (event.GetId())
	{
	case ID_TOOL_GIZMO_SELECT:
		{
			Editor::GetInstance().SetTransformType(Editor::TT_NONE);
		} break;

	case ID_TOOL_GIZMO_TRANSLATE:
		{
			Editor::GetInstance().SetTransformType(Editor::TT_TRANSLATE);
		} break;
		
	case ID_TOOL_GIZMO_ROTATE:
		{
			Editor::GetInstance().SetTransformType(Editor::TT_ROTATE);
		} break;
		
	case ID_TOOL_GIZMO_SCALE:
		{
			Editor::GetInstance().SetTransformType(Editor::TT_SCALE);
		} break;
	}

	RefreshTransformTool();
}


void MainWindow::MenuTranslateType(wxCommandEvent &event)
{
	switch (event.GetId())
	{
	case ID_TOOL_TRANSLATE_LOCAL:
		{
			Editor::GetInstance().SetTranslateType(Editor::TT_LOCAL);
		} break;

	case ID_TOOL_TRANSLATE_GLOBAL:
		{
			Editor::GetInstance().SetTranslateType(Editor::TT_GLOBAL);
		} break;
	}

	RefreshTransformTool();
}


void MainWindow::MenuPlayGame(wxCommandEvent &event)
{
	ToggleMode();
}


void MainWindow::MenuDefaultStyle(wxCommandEvent &event)
{
	m_FrameManager.LoadPerspective(m_DefaultStyle);
}


void MainWindow::MenuRightAllPanes(wxCommandEvent &event)
{
	SetAllDockAlign(wxAUI_DOCK_RIGHT);
}


void MainWindow::MenuLeftAllPanes(wxCommandEvent &event)
{
	SetAllDockAlign(wxAUI_DOCK_LEFT);
}


void MainWindow::MenuTopAllPanes(wxCommandEvent &event)
{
	SetAllDockAlign(wxAUI_DOCK_TOP);
}


void MainWindow::MenuBottomAllPanes(wxCommandEvent &event)
{
	SetAllDockAlign(wxAUI_DOCK_BOTTOM);
}


void MainWindow::MenuTerrain(wxCommandEvent &event)
{
	TerrainWindow *tw = new TerrainWindow;
	tw->ShowModal();
}


void MainWindow::MenuNavMeshGen(wxCommandEvent &event)
{
	NavMeshGenWindow *nmgw = new NavMeshGenWindow;
	nmgw->ShowModal();
}


void MainWindow::MenuAboutEditor(wxCommandEvent &event)
{
	(new AboutWindow(this))->ShowModal();
}
END_NAMESPACE

#endif