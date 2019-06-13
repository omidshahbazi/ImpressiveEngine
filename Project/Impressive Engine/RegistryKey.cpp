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
#include <stdio.h>
#include <string>
#include "RegistryKey.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
RegistryKey::RegistryKey(wxString SubKey, bool Create) :
	m_SubKey(SubKey),
	m_BaseKey(HKEY_CURRENT_USER)
{
	OpenKey(Create);
}


RegistryKey::RegistryKey(wxString SubKey, HKEY BaseKey, bool Create) :
	m_SubKey(SubKey),
	m_BaseKey(BaseKey)
{
	OpenKey(Create);
}


RegistryKey::~RegistryKey(void)
{
	RegCloseKey(m_key);
}


bool RegistryKey::Create(void)
{
	m_Exists = (RegCreateKeyEx(m_BaseKey, m_SubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &m_key, NULL) == ERROR_SUCCESS);

	return m_Exists;
}


bool RegistryKey::SetValue(const char *Value)
{
	return SetValue("", Value);
}


bool RegistryKey::SetValue(bool Value)
{
	return SetValue("", Value);
}


bool RegistryKey::SetValue(int Value)
{
	return SetValue("", Value);
}


bool RegistryKey::SetValue(float Value)
{
	return SetValue("", Value);
}

#ifdef EDIT_MODE
bool RegistryKey::SetValue(wxArrayString &Value)
{
	return SetValue("", Value);
}
#endif

bool RegistryKey::SetValue(const char *Name, const char *Value)
{
	if (lstrlen(Value) == 0)
		Value = "NULL";

	return (RegSetValueEx(
		m_key, Name, 0, REG_SZ, 
		(unsigned char*)Value,  lstrlen(Value) * sizeof(TCHAR)) == ERROR_SUCCESS);
}


bool RegistryKey::SetValue(const char *Name, bool Value)
{
	return (RegSetValueEx(
		m_key, Name, 0, REG_BINARY, 
		(unsigned char*)&Value, sizeof(Value)) == ERROR_SUCCESS);
}


bool RegistryKey::SetValue(const char *Name, int Value)
{
	return (RegSetValueEx(
		m_key, Name, 0, REG_DWORD, 
		(unsigned char*)&Value, sizeof(Value)) == ERROR_SUCCESS);
}


bool RegistryKey::SetValue(const char *Name, float Value)
{
	char buffer[32];
	_snprintf_s(buffer, sizeof(buffer), "%f", Value);

	long res = RegSetValueEx(
		m_key, Name, 0, REG_DWORD, 
		(unsigned char*)&buffer, sizeof(buffer));

	return (res == ERROR_SUCCESS);
}

#ifdef EDIT_MODE
bool RegistryKey::SetValue(const char *Name, wxArrayString &Value)
{
    wxString buffer;
    
    for (unsigned int i = 0; i < Value.GetCount(); i++)
    {
        buffer = buffer +  Value.Item(i) + ",";
    }
	if (buffer.size() == 0)
		buffer = "NULL";

	return SetValue(Name, buffer.mb_str());
}
#endif

wxString RegistryKey::GetValueAsString(const char *Name)
{
    char buff[3096];
    DWORD dwBufferSize = sizeof(buff);
    ULONG res = RegQueryValueEx(m_key, Name, NULL, NULL, (LPBYTE)buff, &dwBufferSize);
    
	if (res != ERROR_SUCCESS)
		return "";

	if (wxString(buff) == "NULL")
		return wxString();
		
	return wxString(buff);

}


bool RegistryKey::GetValueAsBool(const char *Name)
{
    return (GetValueAsInt(Name) == 1);
}


int RegistryKey::GetValueAsInt(const char *Name)
{
    DWORD size(sizeof(DWORD));
    DWORD res(0);

    RegQueryValueEx(m_key, Name, NULL, NULL, reinterpret_cast<LPBYTE>(&res), &size);

    return res;
}


float RegistryKey::GetValueAsFloat(const char *Name)
{
	return atof(GetValueAsString(Name).c_str());
}

#ifdef EDIT_MODE
wxArrayString RegistryKey::GetValueAsArrayString(const char *Name)
{
	////
	//// in faghat baraye string hayi be dard mikhore ke , toshon nabashe!!!!!!!!!!
	////
    wxString buffer;
    wxArrayString Value;

    buffer = GetValueAsString(Name);
    while (!buffer.IsNull())
    {
        Value.Add(buffer.substr(0, buffer.find(",")));
        buffer = buffer.substr(buffer.find(",") + 1);
    }
    return Value;
}
#endif

void RegistryKey::OpenKey(bool Create)
{
	m_Exists = (RegOpenKeyEx(m_BaseKey, m_SubKey.c_str(), NULL, KEY_ALL_ACCESS, &m_key) == ERROR_SUCCESS);

	if (!m_Exists && Create)
		this->Create();
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(RegistryKey)
	BEGIN_DEFINE_TYPE(RegistryKey, "RegistryKey")
		//.enum_("RegistryKeyHandles")
		//[
		//	value("RKH_Class_Root", (int)HKEY_CLASSES_ROOT),
		//	value("RKH_Current_User", (int)HKEY_CURRENT_USER),
		//	value("RKH_Local_Machine", (int)HKEY_LOCAL_MACHINE),
		//	value("RKH_Users", (int)HKEY_USERS)
		//]

		.def(constructor<const char*, bool>())
		//.def(constructor<const char*, HKEY, bool>())

		DEFINE_FUNCTION("SetValueAsString", (bool(RegistryKey::*)(const char*))&RegistryKey::SetValue)
		DEFINE_FUNCTION("SetValueAsBool", (bool(RegistryKey::*)(bool))&RegistryKey::SetValue)
		DEFINE_FUNCTION("SetValueAsInt", (bool(RegistryKey::*)(int))&RegistryKey::SetValue)
		DEFINE_FUNCTION("SetValueAsFloat", (bool(RegistryKey::*)(float))&RegistryKey::SetValue)

		DEFINE_FUNCTION("SetValueAsString", (bool(RegistryKey::*)(const char*, const char*))&RegistryKey::SetValue)
		DEFINE_FUNCTION("SetValueAsBool", (bool(RegistryKey::*)(const char*, bool))&RegistryKey::SetValue)
		DEFINE_FUNCTION("SetValueAsInt", (bool(RegistryKey::*)(const char*, int))&RegistryKey::SetValue)
		DEFINE_FUNCTION("SetValueAsFloat", (bool(RegistryKey::*)(const char*, float))&RegistryKey::SetValue)

		DEFINE_FUNCTION("GetValueAsString", &RegistryKey::GetValueAsString)
		DEFINE_FUNCTION("GetValueAsBool", &RegistryKey::GetValueAsBool)
		DEFINE_FUNCTION("GetValueAsInt", &RegistryKey::GetValueAsInt)
		DEFINE_FUNCTION("GetValueAsFloat", &RegistryKey::GetValueAsFloat)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE