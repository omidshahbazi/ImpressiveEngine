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

#include "Singleton.h"
#include "StdAfxwxWidgets.h"


BEGIN_NAMESPACE
class ScriptPageControl;

class ScriptControl : public wxWindow
{
public:
	ScriptControl(wxWindow *Parent);
	//~ScriptControl(void);

	void Initialize(void);
	void SaveAll(void);
	void Open(const wxString &FilePath);
	unsigned int GetScriptPageNumber(ScriptPageControl *Script);

private:
	ScriptPageControl *GetCurrentPage(void) const;

	ScriptPageControl *AddPage(const wxString &FilePath = "");

private:
	DECLARE_EVENT_TABLE()

	enum
	{
		IDC_NOTEBOOk = 1000,

		IDB_NEW,
		IDB_OPEN,
		IDB_SAVE,
		IDB_SAVEAS
	};

	void OnSize(wxSizeEvent &event);
	void OnRightClick(wxCommandEvent &event);
	void OnNewButton(wxCommandEvent &event);
	void OnOpenButton(wxCommandEvent &event);
	void OnSaveButton(wxCommandEvent &event);
	void OnSaveAsButton(wxCommandEvent &event);
    void OnCloseTab(wxAuiNotebookEvent &event);

public:
	static ScriptControl *Create(wxWindow *Parent)
	{
		if (!m_ThisPointer)
			m_ThisPointer = new ScriptControl(Parent);

		return m_ThisPointer;
	}

	static ScriptControl *GetPointer(void)
	{
		return m_ThisPointer;
	}

	wxString GetName(void)
	{
		return "Script";
	}
	
	wxString GetCaption(void)
	{
		return "Script Editor";
	}

private:
	static ScriptControl *m_ThisPointer;
	wxToolBar *m_ToolBar;
	wxAuiNotebook *m_Notebook;
	wxFileDialog *m_FileDialog;
};
END_NAMESPACE


#endif