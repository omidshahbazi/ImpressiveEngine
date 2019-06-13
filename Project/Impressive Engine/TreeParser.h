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

#include "StdAfxBoost.h"
//#include "ScriptRegister.h"
#include "TreeElement.h"


BEGIN_NAMESPACE
class TreeParser
{
public:
	TreeParser(const std::string &FilePath = "", const std::string &StartPath = "");
	~TreeParser(void);

	void WriteToFile(const std::string &FilePath);

private:
	TreeElement *CreateElement(TreeElement *Parent, ptree::value_type Value);
	void CreateValue(ptree &tree, TreeElement *Current);

public:
//	DECLARE_SCRIPT_REGISTATION()

	TreeElement *GetRoot(void)
	{
		return m_Root;
	}

private:
	TreeElement *m_Root;
};
END_NAMESPACE