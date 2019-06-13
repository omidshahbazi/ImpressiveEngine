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
#include <vector>


BEGIN_NAMESPACE
class ScintillaPageControl : public wxControl
{
public:
	ScintillaPageControl(wxWindow *Parent, int Language, const wxString &FilePath = "");
	~ScintillaPageControl(void);

public:
	void Save(void);
	void SaveAs(void);

	void SetKeywords(const std::vector<wxString> &Keywords);

	wxString GetText(void);
	void SetText(const wxString &Value);

	virtual bool Close(void);

	virtual void SetFilePath(const wxString &Value);

protected:
	void Initialize(void);
	void GetFileNameToSave(bool InSaveAs);
	void SaveToFile(void);

private:
	LRESULT SendMessageToEditor(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0);
	void SetEditorStyle(int Style, COLORREF Fore, COLORREF Back = RGB(0, 0, 0), int Size = -1, const char *FontName = 0);
	WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

public:
	const wxString &GetFilePath(void) const
	{
		return m_FilePath;
	}

protected:
	wxString m_FilePath;

private:
	HWND m_Handle;
	wxFileDialog *m_FileDialog;
};
END_NAMESPACE

#endif