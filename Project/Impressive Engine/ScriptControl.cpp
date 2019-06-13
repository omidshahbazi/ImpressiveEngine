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

#include "ScriptControl.h"
#include "ScriptPageControl.h"
#include "Engine.h"
#include "MainWindow.h"
#include "ScriptRegister.h"
#include <Scintilla.h>


BEGIN_NAMESPACE
ScriptControl *ScriptControl::m_ThisPointer = 0;

BEGIN_EVENT_TABLE(ScriptControl, wxWindow)
	EVT_SIZE(ScriptControl::OnSize)
END_EVENT_TABLE()

ScriptControl::ScriptControl(wxWindow *Parent) :
	wxWindow(Parent, -1)
{
	wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);
	
	m_ToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_FLAT);
	m_ToolBar->AddTool(IDB_NEW, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "New.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "New Script");
	m_ToolBar->AddSeparator();
	m_ToolBar->AddTool(IDB_OPEN, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "OpenSmall.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Open Script");
	m_ToolBar->AddTool(IDB_SAVE, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "SaveSmall.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Save Script");
	m_ToolBar->AddTool(IDB_SAVEAS, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "SaveAs.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Save Script As");
	m_ToolBar->AddSeparator();
	m_ToolBar->Realize();
	bs->Add(m_ToolBar, 0, wxEXPAND, 5);
	
	m_Notebook = new wxAuiNotebook(this, IDC_NOTEBOOk, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE  | wxAUI_NB_WINDOWLIST_BUTTON  );
	bs->Add(m_Notebook, 1, wxEXPAND | wxALL, 5);
	
	SetSizer(bs);
	Layout();

	Connect(IDC_NOTEBOOk, wxEVT_COMMAND_RIGHT_CLICK, wxCommandEventHandler(ScriptControl::OnRightClick));

	Connect(IDB_NEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(ScriptControl::OnNewButton));
	Connect(IDB_OPEN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(ScriptControl::OnOpenButton));
	Connect(IDB_SAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(ScriptControl::OnSaveButton));
	Connect(IDB_SAVEAS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(ScriptControl::OnSaveAsButton));
    Connect(IDC_NOTEBOOk, wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler(ScriptControl::OnCloseTab));
}


//ScriptControl::~ScriptControl(void)
//{
//}


void ScriptControl::Initialize(void)
{
	Scintilla_RegisterClasses(Application::GetInstance());
	//LoadLibrary((Engine::PATH_PLUGINS + "Scintilla.dll").c_str());
}


void ScriptControl::SaveAll(void)
{
	for (unsigned int i = 0; i < m_Notebook->GetPageCount(); i++)
	{
		((ScintillaPageControl*)m_Notebook->GetPage(i))->Save();
	}
}


void ScriptControl::Open(const wxString &FilePath)
{
	AddPage(FilePath);
	
	((MainWindow*)GetParent())->ShowWindow(ScriptControl::GetName());
}


unsigned int ScriptControl::GetScriptPageNumber(ScriptPageControl *Script)
{
	for (unsigned int i = 0; i < m_Notebook->GetPageCount(); i++)
		if (m_Notebook->GetPage(i) == Script)
		{
			return i;
		}

	return -1;
}


ScriptPageControl *ScriptControl::GetCurrentPage(void) const
{
    if (m_Notebook->GetPageCount()){
        wxWindow *page = m_Notebook->FindFocus();
    
	    if (page)
		    return static_cast<ScriptPageControl*>(page);
    }
    return NULL;
}


ScriptPageControl *ScriptControl::AddPage(const wxString &FilePath)
{
	ScriptPageControl *page = NULL;
	wxString text = "New Script";

	if (FilePath.size() > 0)
	{
		text = wxFileNameFromPath(FilePath);

		for (unsigned int i = 0; i < m_Notebook->GetPageCount(); i++)
			if (((ScriptPageControl*)m_Notebook->GetPage(i))->GetFilePath() == FilePath)
				return (ScriptPageControl*)m_Notebook->GetPage(i);
	}

	page = new ScriptPageControl(m_Notebook, FilePath);

	m_Notebook->AddPage(page, text, true);

	return page;
}


void ScriptControl::OnSize(wxSizeEvent &event)
{
	m_ToolBar->SetSize(event.GetSize().x, m_ToolBar->GetSize().y);
	m_Notebook->SetSize(event.GetSize().x - 7, event.GetSize().y - m_ToolBar->GetSize().y - 5);
}


void ScriptControl::OnRightClick(wxCommandEvent &event)
{
	if (GetCurrentPage())
		GetCurrentPage()->Close();
}


void ScriptControl::OnNewButton(wxCommandEvent &event)
{
	ScriptPageControl *spc = AddPage();

	wxString text = "";
	wxArrayString list;
	list.Add("Start");
	list.Add("Update");
	//list.Add("PreRender");
	//list.Add("PostRender");

	text += "-------------------------------------";
	text += "\n--";
	text += "\n--	Writer : ";
	text += "\n--	Date : / /";
	text += "\n--	Description : ";
	text += "\n--";
	text += "\n-------------------------------------";

	for (unsigned int i = 0; i < list.size(); i++)
	{
		text += "\n\nfunction " + list[i] + "()";
		text += "\n\t--To Do";
		text += "\nend";
	}

	spc->SetText(text);

	spc = NULL;
}


void ScriptControl::OnOpenButton(wxCommandEvent &event)
{
	m_FileDialog = new wxFileDialog(this, "Open Script", Engine::PATH_SCRIPT, "", "Script File|*.script", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

	if (m_FileDialog->ShowModal() != wxID_CANCEL)
	{
		wxArrayString paths;
		m_FileDialog->GetPaths(paths);

		for (unsigned int i = 0; i < paths.size(); i++)
			AddPage(paths[i]);
	}

	delete m_FileDialog;
	m_FileDialog = 0;
}


void ScriptControl::OnSaveButton(wxCommandEvent &event)
{
	if (GetCurrentPage())
		GetCurrentPage()->Save();
}


void ScriptControl::OnSaveAsButton(wxCommandEvent &event)
{
	if (GetCurrentPage())
		GetCurrentPage()->SaveAs();
}

void ScriptControl::OnCloseTab(wxAuiNotebookEvent &event)
{
    
    wxWindow* page = m_Notebook->GetPage(event.GetSelection());
    ScriptPageControl* spc = static_cast <ScriptPageControl*> (page);
	if (!spc->Close())
        event.Veto();
    
        
}
END_NAMESPACE
#endif