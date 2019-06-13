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

#include "StdAfxwxWidgets.h"
#include "Common.h"

BEGIN_NAMESPACE
/*static */class FileSecurity
{
public:
	static wxString EncryptContent(const wxString &Content, const wxString &Salt);
	
	static wxString DecryptContent(const wxString &Content, const wxString &Salt);

private:
	static int GetSalt(const wxString &Salt);

	static wxString Method(const wxString &Content, bool Encrypting);
};


END_NAMESPACE