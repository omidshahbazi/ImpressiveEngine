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

#include "ScintillaPageControl.h"
#include "StdAfxScintilla.h"
#include "Application.h"
#include "IO.h"
#include "Entity.h"
#include "Engine.h"
#include "Utility.h"

BEGIN_NAMESPACE
struct SScintillaColors
{
	int Item;
	COLORREF RGB;
};

// A few basic colors
const COLORREF black = RGB(0, 0, 0);
const COLORREF white = RGB(255, 255, 255);

/// Default color scheme
static SScintillaColors syntaxColor[] = 
{
	{SCE_C_COMMENT, RGB(0, 128, 0)},
	{SCE_C_COMMENTLINE, RGB(0, 128, 0)},
	{SCE_C_COMMENTDOC, RGB(0, 128, 0)},
	{SCE_C_NUMBER, black},
	{SCE_C_STRING, RGB(163, 21, 21)},
	{SCE_C_CHARACTER, RGB(163, 21, 21)},
	{SCE_C_UUID, black},
	{SCE_C_OPERATOR, black},
	{SCE_C_PREPROCESSOR, RGB(0, 0, 255)},
	{SCE_C_WORD, RGB(43, 145, 175)/*RGB(0, 0, 255)*/},
	{-1, 0}
};


ScintillaPageControl::ScintillaPageControl(wxWindow *Parent, int Language, const wxString &FilePath) :
	wxControl(Parent, 0, wxDefaultPosition, wxDefaultSize, wxMB_DOCKABLE),
	m_FilePath(FilePath)
{
	Initialize();

	// Lua lexer
	SendMessageToEditor(SCI_SETLEXER, Language);
	
	if (m_FilePath.size() > 0)
	{
		SetText(IO::GetInstance().ReadScript(STRING_WX_TO_OGRE(m_FilePath)));
	}
}


ScintillaPageControl::~ScintillaPageControl(void)
{
	CloseWindow(m_Handle);

	m_Handle = 0;
}

	
void ScintillaPageControl::Initialize(void)
{
	m_Handle = CreateWindowEx(0, "Scintilla", "", 
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
		0, 0, 100, 100,
		(HWND)GetHWND(), NULL, 
		Application::GetInstance(), NULL);

	// Set number of style bits to use
	SendMessageToEditor(SCI_SETSTYLEBITS, 5);
	
	// Set tab width
	SendMessageToEditor(SCI_SETTABWIDTH, 4);

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetEditorStyle(STYLE_DEFAULT, black, white, 10, "Consolas");

	// Set all styles
	SendMessageToEditor(SCI_STYLECLEARALL);

	SendMessageToEditor(STYLE_LINENUMBER, 0, TRUE);

	// Set caret foreground color
	SendMessageToEditor(SCI_SETCARETFORE, black);

	// Set selection color
	SendMessageToEditor(SCI_SETSELBACK, TRUE, RGB(51, 153, 255));
	
	// Set syntax colors
	for (long i = 0; syntaxColor[i].Item != -1; i++) 
		SendMessageToEditor(SCI_STYLESETFORE, syntaxColor[i].Item, syntaxColor[i].RGB);
}


void ScintillaPageControl::GetFileNameToSave(bool InSaveAs)
{
	m_FileDialog = new wxFileDialog(this, wxString("Save Script") + (InSaveAs ? " As" : ""), Engine::PATH_SCRIPT, "", "Script File|*.script", wxFD_SAVE);

	if (m_FileDialog->ShowModal() != wxID_CANCEL)
	{
		SetFilePath(m_FileDialog->GetPath());
	}

	delete m_FileDialog;
	m_FileDialog = 0;
}


void ScintillaPageControl::SaveToFile(void)
{
	if (m_FilePath.size() > 0)
	{
		IO::GetInstance().WriteScript(STRING_WX_TO_OGRE(m_FilePath), STRING_WX_TO_OGRE(GetText()));

		Entity::GetInstance().Refresh();
	}
}


LRESULT ScintillaPageControl::SendMessageToEditor(UINT Msg, WPARAM wParam, LPARAM lParam) 
{	
	return SendMessage(m_Handle, Msg, wParam, lParam); 
}


void ScintillaPageControl::SetEditorStyle(int Style, COLORREF Fore, COLORREF Back, int Size, const char *FontName)
{
	SendMessageToEditor(SCI_STYLESETFORE, Style, Fore);

	SendMessageToEditor(SCI_STYLESETBACK, Style, Back);

	if (Size >= 1)
		SendMessageToEditor(SCI_STYLESETSIZE, Style, Size);

	if (FontName) 
		SendMessageToEditor(SCI_STYLESETFONT, Style, reinterpret_cast<LPARAM>(FontName));
}


WXLRESULT ScintillaPageControl::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
	if (nMsg == WM_SIZE)
	{
		MoveWindow(m_Handle, 0, 0, GetSize().x - 4, GetSize().y - 4, true);
	}
			
	return wxControl::MSWWindowProc(nMsg, wParam, lParam);
}


void ScintillaPageControl::Save(void)
{
	if (m_FilePath.size() == 0)
	{
		GetFileNameToSave(false);
	}
	
	SaveToFile();
}


void ScintillaPageControl::SaveAs(void)
{
	GetFileNameToSave(true);

	SaveToFile();
}


void ScintillaPageControl::SetKeywords(const std::vector<wxString> &Keywords)
{
	wxString keywords = "";

	BEGIN_VECTOR_ITERATOR(Keywords, wxString)
		keywords += current + " ";
	END_VECTOR_ITERATOR("")

	// Use LUA keywords
	SendMessageToEditor(SCI_SETKEYWORDS, 0, (LPARAM)keywords.mb_str());
}


wxString ScintillaPageControl::GetText(void)
{
	long len = SendMessageToEditor(SCI_GETLENGTH, 0, 0) + 1;

	if (len > 0)
	{
		char *str = new char[len];

		SendMessageToEditor(SCI_GETTEXT, len, (long)str);
			
		return wxString(str);
	}

	return "";
}


void ScintillaPageControl::SetText(const wxString &Value)
{
	SendMessageToEditor(SCI_SETTEXT, 0, (LPARAM)Value.mb_str());
}


bool ScintillaPageControl::Close(void)
{
	int result = wxMessageBox("Do you want to save the script before closing it ?", "Closing Script", wxYES_NO | wxCANCEL | wxICON_WARNING, this);

	switch (result)
	{
	case wxYES:
		{
			Save();
		} break;

	case wxCANCEL:
		{
			return false;
		}

	//case wxNo:
	//	{
	//	} break;
	}

    return true;
}


void ScintillaPageControl::SetFilePath(const wxString &Value)
{
	m_FilePath = Value;
}
END_NAMESPACE

#endif