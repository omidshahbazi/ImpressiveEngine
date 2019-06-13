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
#include "TreeElement.h"
#include "Utility.h"
#include "StdAfxBoost.h"


BEGIN_NAMESPACE
TreeElement::TreeElement(TreeElement *_Parent) :
	Parent(_Parent)
{
	if (Parent)
		Parent->Children.push_back(this);
}

	
TreeElement::~TreeElement(void)
{
	Name = "";
	Value = "";

	Attributes.clear();

	BEGIN_VECTOR_ITERATOR(Children, TreeElement*)
		delete current;
	END_VECTOR_ITERATOR(NULL)

	Children.clear();
}


const std::string TreeElement::GetAttributeValue(const char *Name) const
{
	BOOST_FOREACH(const TreeElementAttribute &att, Attributes)
		if (att.first == Name)
			return att.second;

	return "";
}

END_NAMESPACE