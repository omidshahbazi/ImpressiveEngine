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

#include <string>
#include <vector>
#include "Common.h"


BEGIN_NAMESPACE
typedef std::pair<std::string, std::string> TreeElementAttribute;
typedef std::vector<TreeElementAttribute> TreeElementAttributeList;

struct TreeElement
{
	TreeElement *Parent;

	std::string Name;
	std::string Value;

	TreeElementAttributeList Attributes;

	std::vector<TreeElement*> Children;

	TreeElement(TreeElement *_Parent = NULL);
	~TreeElement(void);

	const std::string GetAttributeValue(const char *Name) const;
};

END_NAMESPACE