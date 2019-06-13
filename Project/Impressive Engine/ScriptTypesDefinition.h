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

#include "Singleton.h"
#include "VirtualMachine.h"

#ifdef EDIT_MODE
#include "StdAfxwxWidgets.h"
#include <vector>
#endif

BEGIN_NAMESPACE
class VirtualMachine;

class ScriptTypesDefinition : public Singleton<ScriptTypesDefinition>
{
public:
	//ScriptTypesDefinition(void);
	//~ScriptTypesDefinition(void);

	void Initialize(void);

#ifdef EDIT_MODE
	void AddKeyword(const wxString &Keyword)
	{
		m_Keywords.push_back(Keyword);
	}

	const std::vector<wxString> &GetKeywords(void) const
	{
		return m_Keywords;
	}

private:
	std::vector<wxString> m_Keywords;
#endif
};
END_NAMESPACE