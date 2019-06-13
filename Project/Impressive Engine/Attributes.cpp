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
#include "Attributes.h"
#include "TreeParser.h"
#include "Attribute.h"
#include "Scene.h"
#include "Utility.h"


BEGIN_NAMESPACE
//Attributes::Attributes(void)
//{
//}


Attributes::~Attributes(void)
{
	for (unsigned int i = 0; i < m_Attributes.size(); i++)
	{
		delete m_Attributes[i];
	}

	m_Attributes.clear();
}


void Attributes::Read(const TreeElement *Element)
{
	BOOST_FOREACH(const TreeElement *elem, Element->Children)
	{
		const std::string name = elem->GetAttributeValue("Name");
		const std::string value = elem->GetAttributeValue("Value");

		if (elem->Name == builtInAttributeTypeName[AT_INT])
		{
			AddInteger(name, Ogre::StringConverter::parseInt(value));
		}
		else if (elem->Name == builtInAttributeTypeName[AT_FLOAT])
		{
			AddFloat(name, Ogre::StringConverter::parseReal(value));
		}
		else if (elem->Name == builtInAttributeTypeName[AT_STRING])
		{
			AddString(name, value);
		}
		else if (elem->Name == builtInAttributeTypeName[AT_BOOL])
		{
			AddBoolean(name, Ogre::StringConverter::parseBool(value));
		}
		else if (elem->Name == builtInAttributeTypeName[AT_COLOR])
		{
			AddColor(name, Ogre::StringConverter::parseColourValue(value));
		}
		else if (elem->Name == builtInAttributeTypeName[AT_VECTOR2])
		{
			AddVector2(name, Ogre::StringConverter::parseVector2(value));
		}
		else if (elem->Name == builtInAttributeTypeName[AT_VECTOR3])
		{
			AddVector3(name, Ogre::StringConverter::parseVector3(value));
		}
		else if (elem->Name == builtInAttributeTypeName[AT_VECTOR4])
		{
			AddVector4(name, Ogre::StringConverter::parseVector4(value));
		}
		else if (elem->Name == builtInAttributeTypeName[AT_QUATERNION])
		{
			AddQuaternion(name, Ogre::StringConverter::parseQuaternion(value));
		}
	}
}


void Attributes::Write(TreeElement *Element)
{
	TreeElement *attributes = new TreeElement(Element);
	attributes->Name = Scene::TAG_ATTRIBUTES;

	TreeElement *elem = NULL;

	BOOST_FOREACH(const Attribute *attr, m_Attributes)
	{
		elem = new TreeElement(attributes);
		elem->Name = attr->GetTypeString();

		elem->Attributes.push_back(TreeElementAttribute("Name", STRING_WX_TO_OGRE(attr->GetName())));
		elem->Attributes.push_back(TreeElementAttribute("Value", attr->GetValueString()));
	}
}


void Attributes::AddInteger(const std::string &Name, int Value)
{
	m_Attributes.push_back(new AttributeInteger(Name, Value));
}


void Attributes::AddFloat(const std::string &Name, float Value)
{
	m_Attributes.push_back(new AttributeFloat(Name, Value));
}


void Attributes::AddString(const std::string &Name, const std::string &Value)
{
	m_Attributes.push_back(new AttributeString(Name, Value));
}


void Attributes::AddBoolean(const std::string &Name, bool Value)
{
	m_Attributes.push_back(new AttributeBoolean(Name, Value));
}


void Attributes::AddColor(const std::string &Name, const Ogre::ColourValue &Value)
{
	m_Attributes.push_back(new AttributeColor(Name, Value));
}


void Attributes::AddVector2(const std::string &Name, const Ogre::Vector2 &Value)
{
	m_Attributes.push_back(new AttributeVector2(Name, Value));
}


void Attributes::AddVector3(const std::string &Name, const Ogre::Vector3 &Value)
{
	m_Attributes.push_back(new AttributeVector3(Name, Value));
}


void Attributes::AddVector4(const std::string &Name, const Ogre::Vector4 &Value)
{
	m_Attributes.push_back(new AttributeVector4(Name, Value));
}


void Attributes::AddQuaternion(const std::string &Name, const Ogre::Quaternion &Value)
{
	m_Attributes.push_back(new AttributeQuaternion(Name, Value));
}


void Attributes::SetInteger(const char *Name, int Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeInteger*)at)->SetValue(Value);
}


void Attributes::SetFloat(const char *Name, float Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeFloat*)at)->SetValue(Value);
}


void Attributes::SetString(const char *Name, const char *Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeString*)at)->SetValue(Value);
}


void Attributes::SetBoolean(const char *Name, bool Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeBoolean*)at)->SetValue(Value);
}


void Attributes::SetColor(const char *Name, const Ogre::ColourValue &Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeColor*)at)->SetValue(Value);
}


void Attributes::SetVector2(const char *Name, const Ogre::Vector2 &Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeVector2*)at)->SetValue(Value);
}


void Attributes::SetVector3(const char *Name, const Ogre::Vector3 &Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeVector3*)at)->SetValue(Value);
}


void Attributes::SetVector4(const char *Name, const Ogre::Vector4 &Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeVector4*)at)->SetValue(Value);
}


void Attributes::SetQuaternion(const char *Name, const Ogre::Quaternion &Value)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeQuaternion*)at)->SetValue(Value);
}


int Attributes::GetInteger(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeInteger*)at)->GetValue();

	return 0;
}


float Attributes::GetFloat(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeFloat*)at)->GetValue();

	return 0.f;
}


std::string Attributes::GetString(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeString*)at)->GetValue();

	return "";
}


bool Attributes::GetBoolean(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeBoolean*)at)->GetValue();

	return false;
}


Ogre::ColourValue Attributes::GetColor(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeColor*)at)->GetValue();

	return Ogre::ColourValue::ZERO;
}


Ogre::Vector2 Attributes::GetVector2(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeVector2*)at)->GetValue();

	return Ogre::Vector2::ZERO;
}


Ogre::Vector3 Attributes::GetVector3(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeVector3*)at)->GetValue();

	return Ogre::Vector3::ZERO;
}


Ogre::Vector4 Attributes::GetVector4(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeVector4*)at)->GetValue();

	return Ogre::Vector4::ZERO;
}


Ogre::Quaternion Attributes::GetQuaternion(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
		return ((AttributeQuaternion*)at)->GetValue();

	return Ogre::Quaternion::ZERO;
}


void Attributes::Remove(const char *Name)
{
	Attribute *at = GetAttribute(Name);

	if (at)
	{
		std::vector<Attribute*>::iterator it = m_Attributes.begin();

		while (it != m_Attributes.end())
		{
			if (at == *it)
			{
				m_Attributes.erase(it);
				break;
			}

			it++;
		}
	}
}


Attribute *Attributes::GetAttribute(const char *Name)
{
	for (unsigned int i = 0; i < m_Attributes.size(); i++)
		if (m_Attributes[i]->GetName() == wxString(Name))
			return m_Attributes[i];

	return NULL;
}
END_NAMESPACE