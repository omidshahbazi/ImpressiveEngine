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


#include "LogControl.h"
#include "Engine.h"
#include "Utility.h"


BEGIN_NAMESPACE
BEGIN_EVENT_TABLE(LogControl, wxWindow)
	EVT_SIZE(LogControl::OnSize)
END_EVENT_TABLE()


LogControl::LogControl(wxWindow *Parent) :
	wxWindow(Parent, -1)
{
	wxFlexGridSizer *fgs = new wxFlexGridSizer(2, 1, 0, 0);
	fgs->SetFlexibleDirection(wxBOTH);
	fgs->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    wxBitmap info = wxBitmap(Engine::PATH_EDITOR_IMAGE + "Info.png", wxBITMAP_TYPE_ANY);
    wxBitmap error = wxBitmap(Engine::PATH_EDITOR_IMAGE + "Error.png", wxBITMAP_TYPE_ANY);
    wxBitmap warning = wxBitmap(Engine::PATH_EDITOR_IMAGE + "Warning.png", wxBITMAP_TYPE_ANY);

	m_ToolBar = new wxToolBar(this, wxID_ANY);

    m_ToolBar->AddTool(IDB_SHOWHIDE_INFO, "", info, "", wxITEM_CHECK);
    m_ToolBar->AddTool(IDB_SHOWHIDE_ERROR, "", error, "", wxITEM_CHECK);
    m_ToolBar->AddTool(IDB_SHOWHIDE_WARNING, "", warning, "", wxITEM_CHECK);

    m_ToolBar->AddSeparator();
	m_ToolBar->AddTool(IDB_CLEAR, "", wxBitmap(Engine::PATH_EDITOR_IMAGE + "Delete.png", wxBITMAP_TYPE_ANY), wxNullBitmap, wxITEM_NORMAL, "Clear");
    
    m_ToolBar->ToggleTool(IDB_SHOWHIDE_INFO,true);
    m_ToolBar->ToggleTool(IDB_SHOWHIDE_ERROR,true);
    m_ToolBar->ToggleTool(IDB_SHOWHIDE_WARNING,true);

	m_ToolBar->Realize();
	fgs->Add(m_ToolBar, 0, wxALL);
	
	//m_Box = new wxListBox(this, -1, wxDefaultPosition, wxDefaultSize, 0, 0, wxHSCROLL);
	//fgs->Add(m_Box, 0, wxALL);       

    m_Iconlst = new wxImageList(16,16,true);
    m_Iconlst->Add(info,info);
    m_Iconlst->Add(error,error);
    m_Iconlst->Add(warning,warning);


    m_Box = new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxDefaultSize, wxLC_REPORT | wxLC_ALIGN_LEFT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
    m_Box->SetImageList(m_Iconlst,wxIMAGE_LIST_SMALL);
    m_Box->SetMinSize(wxSize(200,50));
    m_Box->Show();
    m_Box->InsertColumn(0,"");
    m_Box->SetColumnWidth(0,1200);
    
	//m_Box = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	fgs->Add(m_Box, 0, wxALL);
	
	SetSizer(fgs);
	Layout();

	Connect(IDB_CLEAR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(LogControl::OnClearButton));
    Connect(IDB_SHOWHIDE_INFO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(LogControl::OnToggleToolButton));
    Connect(IDB_SHOWHIDE_ERROR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(LogControl::OnToggleToolButton));
    Connect(IDB_SHOWHIDE_WARNING, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(LogControl::OnToggleToolButton));
}


//LogControl::~LogControl(void)
//{
//}
	

void LogControl::Clear(void)
{
	//m_Box->Clear();
	//m_Box->SetValue("");
    m_Box->DeleteAllItems();
    //m_Items->Clear();
	m_Logs.clear();
}


void LogControl::Add(const std::string &Text, const LogType& Type)
{
	//m_Box->Insert(Text, 0);
	//m_Box->SetValue(wxString(Text) + "\n" + m_Box->GetValue());
    
    //m_Items->Add();
	m_Logs.push_back(LogItem(Text, Type));
    RefreshList();

    //m_Box->InsertItem(0,Text,Type);
}


void LogControl::RefreshList()
{
    m_Box->DeleteAllItems();

	BEGIN_VECTOR_ITERATOR(m_Logs, LogItem)
        if (current.second == LT_INFO && !m_ToolBar->FindById(IDB_SHOWHIDE_INFO)->IsToggled())
			continue;

		if (current.second == LT_ERROR && !m_ToolBar->FindById(IDB_SHOWHIDE_ERROR)->IsToggled())
			continue;

		if (current.second == LT_WARNING && !m_ToolBar->FindById(IDB_SHOWHIDE_WARNING)->IsToggled())
			continue;

		m_Box->InsertItem(0,current.first, current.second);
	END_VECTOR_ITERATOR(LogItem())
}


void LogControl::OnSize(wxSizeEvent &event)
{
	m_ToolBar->SetSize(event.GetSize().x, m_ToolBar->GetSize().y);
	m_Box->SetSize(event.GetSize().x, event.GetSize().y - m_ToolBar->GetSize().y + 3);
}


void LogControl::OnClearButton(wxCommandEvent &event)
{
	Clear();
}


void LogControl::OnToggleToolButton(wxCommandEvent &event)
{
    switch (event.GetId())
    {
    case IDB_SHOWHIDE_INFO:
		{
            m_ToolBar->ToggleTool(IDB_SHOWHIDE_INFO,event.IsChecked());
		} break;

    case IDB_SHOWHIDE_ERROR:
		{
			m_ToolBar->ToggleTool(IDB_SHOWHIDE_ERROR,event.IsChecked());
		} break;
		
    case IDB_SHOWHIDE_WARNING:
		{
			m_ToolBar->ToggleTool(IDB_SHOWHIDE_WARNING,event.IsChecked());
		} break;
    }
    RefreshList();
}
END_NAMESPACE
#endif