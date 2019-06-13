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

#include "StatusBarControl.h"
#include "MainWindow.h"
#include "Log.h"
#include "Engine.h"


BEGIN_NAMESPACE
StatusBarControl *Singleton<StatusBarControl>::m_Instance = NULL;

StatusBarControl::StatusBarControl(void)
{
    
}


StatusBarControl::~StatusBarControl()
{

}


void StatusBarControl::Initialize()
{
    m_Control = new wxStatusBar(MainWindow::GetPointer(), wxID_ANY, wxSB_FLAT | wxST_SIZEGRIP);
    m_Control->SetFieldsCount(2);
    m_Control->Show();

    wxBitmap info = wxBitmap(Engine::PATH_EDITOR_IMAGE + "Info.png", wxBITMAP_TYPE_ANY);
    wxBitmap error = wxBitmap(Engine::PATH_EDITOR_IMAGE + "Error.png", wxBITMAP_TYPE_ANY);
    wxBitmap warning = wxBitmap(Engine::PATH_EDITOR_IMAGE + "Warning.png", wxBITMAP_TYPE_ANY);

    m_Icon_Info = new wxBitmapButton(m_Control, wxID_ANY, info, wxPoint(0, 1), wxDefaultSize, wxBU_EXACTFIT);
    m_Icon_Error = new wxBitmapButton(m_Control, wxID_ANY, error, wxPoint(0, 1), wxDefaultSize, wxBU_EXACTFIT);
    m_Icon_Warning = new wxBitmapButton(m_Control, wxID_ANY, warning, wxPoint(0, 1), wxDefaultSize, wxBU_EXACTFIT);

    m_Icon_Info->Hide();
    m_Icon_Error->Hide();
    m_Icon_Warning->Hide();

    m_Icon_Info->Connect(wxEVT_LEFT_DCLICK, wxCommandEventHandler(StatusBarControl::OnDclick));
    m_Icon_Error->Connect(wxEVT_LEFT_DCLICK, wxCommandEventHandler(StatusBarControl::OnDclick));
    m_Icon_Warning->Connect(wxEVT_LEFT_DCLICK, wxCommandEventHandler(StatusBarControl::OnDclick));
    m_Control->Connect(wxEVT_LEFT_DCLICK, wxCommandEventHandler(StatusBarControl::OnDclick));
}


void StatusBarControl::ShowLastLog(const wxString &Text, const int &Type)
{
    m_Icon_Info->Show(Type == 0);
    m_Icon_Error->Show(Type == 1);
    m_Icon_Warning->Show(Type == 2);

    m_Control->SetStatusText("        " + Text, 0);
}


void StatusBarControl::HideIcons()
{
    if(m_Control->GetStatusText(0) == "")
    {
        m_Icon_Warning->Hide();
        m_Icon_Error->Hide();
        m_Icon_Info->Hide();
    }
}


void StatusBarControl::OnDclick(wxCommandEvent &event)
{
    MainWindow::GetPointer()->GetPane(Log::GetInstance().GetName()).Show();
    MainWindow::GetPointer()->GetPane(Log::GetInstance().GetName()).Float();
    MainWindow::GetPointer()->UpdatePanes();
}
END_NAMESPACE
#endif