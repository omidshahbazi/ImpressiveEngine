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
#include "Singleton.h"


BEGIN_NAMESPACE
class StatusBarControl: public wxStatusBar, public Singleton<StatusBarControl>
{

public:
    StatusBarControl(void);
    virtual ~StatusBarControl();

    wxStatusBar *GetControl(void)
	{
		return m_Control;
	}

    void Initialize();

    void ShowLastLog(const wxString &Text, const int &Type);
    void HideIcons();

private:
    void OnDclick(wxCommandEvent &event);

private:
    wxStatusBar    *m_Control;
    wxBitmapButton *m_Icon_Info;
    wxBitmapButton *m_Icon_Error;
    wxBitmapButton *m_Icon_Warning;
};
END_NAMESPACE
#endif