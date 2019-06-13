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
#include "Common.h"
#include <map>
#include <vector>


BEGIN_NAMESPACE
class LogControl : public wxWindow
{
public:
    enum LogType
    {
        LT_INFO = 0,
        LT_ERROR,
        LT_WARNING
    };

public:
	LogControl(wxWindow *Parent);
	//~LogControl(void);

	void Clear(void);

    void Add(const std::string &Text, const LogType &Type);

private:
	DECLARE_EVENT_TABLE()

	enum
	{
		IDB_CLEAR = 1000,
        IDB_SHOWHIDE_INFO,
        IDB_SHOWHIDE_ERROR,
        IDB_SHOWHIDE_WARNING,
        IDB_ADD_INFO,
        IDB_ADD_ERROR,
        IDB_ADD_WARNING
	};

	void OnSize(wxSizeEvent &event);
	void OnClearButton(wxCommandEvent &event);
    void OnToggleToolButton(wxCommandEvent &event);

    void RefreshList();

private:
	wxToolBar *m_ToolBar;
    wxListCtrl *m_Box;
	//wxListBox *m_Box;
	//wxTextCtrl *m_Box;
    wxImageList *m_Iconlst;

	typedef std::pair<std::string, LogType> LogItem;
	std::vector<LogItem> m_Logs;
};
END_NAMESPACE

#endif