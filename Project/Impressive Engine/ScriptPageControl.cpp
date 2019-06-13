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

#include "ScriptPageControl.h"
#include "StdAfxScintilla.h"
#include "ScriptControl.h"
#include "ScriptRegister.h"



BEGIN_NAMESPACE
ScriptPageControl::ScriptPageControl(wxWindow *Parent, const wxString &FilePath) :
	ScintillaPageControl(Parent, SCLEX_LUA, FilePath)
{
	SetKeywords(ScriptTypesDefinition::GetInstance().GetKeywords());
}


//ScriptPageControl::~ScriptPageControl(void)
//{
//}

	
bool ScriptPageControl::Close(void)
{
	return ScintillaPageControl::Close();

	//((wxAuiNotebook*)GetParent())->DeletePage(ScriptControl::GetPointer()->GetScriptPageNumber(this));
}

void ScriptPageControl::SetFilePath(const wxString &Value)
{
	ScintillaPageControl::SetFilePath(Value);

	if (m_FilePath.size() > 0)
	{
		((wxAuiNotebook*)GetParent())->SetPageText(ScriptControl::GetPointer()->GetScriptPageNumber(this), wxFileNameFromPath(m_FilePath));
	}
}
END_NAMESPACE

#endif