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

#include "ScintillaPageControl.h"


BEGIN_NAMESPACE
class ScriptPageControl : public ScintillaPageControl
{
public:
	ScriptPageControl(wxWindow *Parent, const wxString &FilePath = "");
	//~ScriptPageControl(void);

	bool Close(void);
	void SetFilePath(const wxString &Value);
};
END_NAMESPACE


#endif