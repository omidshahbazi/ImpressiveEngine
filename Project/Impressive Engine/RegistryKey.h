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
#pragma once

#include <windows.h>
#include "StdAfxwxWidgets.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class RegistryKey
{
public:
	RegistryKey(wxString Key, bool Create = false);
	RegistryKey(wxString Key, HKEY BaseKey, bool Create = false);
	~RegistryKey(void);

	bool Create(void);

	bool SetValue(const char *Value);
	bool SetValue(bool Value);
	bool SetValue(int Value);
	bool SetValue(float Value);
#ifdef EDIT_MODE
    bool SetValue(wxArrayString &Value);
#endif

	bool SetValue(const char *Name, const char *Value);
	bool SetValue(const char *Name, bool Value);
	bool SetValue(const char *Name, int Value);
	bool SetValue(const char *Name, float Value);
#ifdef EDIT_MODE
    bool SetValue(const char *Name, wxArrayString &Value);
#endif

	wxString GetValueAsString(const char *Name);
	bool GetValueAsBool(const char *Name);
	int GetValueAsInt(const char *Name);
	float GetValueAsFloat(const char *Name);
#ifdef EDIT_MODE
    wxArrayString GetValueAsArrayString(const char *Name);
#endif

	bool Exists(void)
	{
		return m_Exists;
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	void OpenKey(bool Create);

private:
	wxString m_SubKey;
	HKEY m_BaseKey;
	HKEY m_key;
	bool m_Exists;
};

END_NAMESPACE