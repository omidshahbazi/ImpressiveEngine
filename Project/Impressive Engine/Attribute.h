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
#include "StdAfxOGRE.h"

BEGIN_NAMESPACE
const char *builtInAttributeTypeName[] = {
	"INT",
	"FLOAT",
	"STRING",
	"BOOL",
	"COLOR",
	"VECTOR2",
	"VECTOR3",
	"VECTOR4",
	"QUATERNION"
};

enum ATTRIBUTE_TYPE
{
	AT_INT = 0,
	AT_FLOAT,
	AT_STRING,
	AT_BOOL,
	AT_COLOR,
	AT_VECTOR2,
	AT_VECTOR3,
	AT_VECTOR4,
	AT_QUATERNION,
	AT_COUNT
};

class Attribute
{
public:
	Attribute(const wxString &Name) :
		m_Name(Name)
	{
	}


	//~Attribute(void)
	//{
	//}

	virtual Ogre::String GetValueString(void) const = 0;

	virtual ATTRIBUTE_TYPE GetType(void) const = 0;

	const char* GetTypeString(void) const
	{
		return builtInAttributeTypeName[GetType()];
	}

	const wxString &GetName(void) const
	{
		return m_Name;
	}

	wxString m_Name;
};


class AttributeInteger : public Attribute
{
public:
	AttributeInteger(const wxString &Name, int Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_INT;
	}

	void SetValue(int Value)
	{
		m_Value = Value;
	}

	int GetValue(void)
	{
		return m_Value;
	}

private:
	int m_Value;
};


class AttributeFloat : public Attribute
{
public:
	AttributeFloat(const wxString &Name, float Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_FLOAT;
	}

	void SetValue(float Value)
	{
		m_Value = Value;
	}

	float GetValue(void)
	{
		return m_Value;
	}

private:
	float m_Value;
};


class AttributeString : public Attribute
{
public:
	AttributeString(const wxString &Name, const wxString &Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return m_Value;
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_STRING;
	}

	void SetValue(const char *Value)
	{
		m_Value = Value;
	}

	const Ogre::String &GetValue(void) const
	{
		return m_Value;
	}

private:
	Ogre::String m_Value;
};


class AttributeBoolean : public Attribute
{
public:
	AttributeBoolean(const wxString &Name, bool Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_BOOL;
	}

	void SetValue(bool Value)
	{
		m_Value = Value;
	}

	bool GetValue(void)
	{
		return m_Value;
	}

private:
	bool m_Value;
};


class AttributeColor : public Attribute
{
public:
	AttributeColor(const wxString &Name, const Ogre::ColourValue &Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_COLOR;
	}

	void SetValue(const Ogre::ColourValue &Value)
	{
		m_Value = Value;
	}

	const Ogre::ColourValue &GetValue(void)
	{
		return m_Value;
	}

private:
	Ogre::ColourValue m_Value;
};


class AttributeVector2 : public Attribute
{
public:
	AttributeVector2(const wxString &Name, const Ogre::Vector2 &Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_VECTOR2;
	}

	void SetValue(const Ogre::Vector2 &Value)
	{
		m_Value = Value;
	}

	const Ogre::Vector2 &GetValue(void)
	{
		return m_Value;
	}

private:
	Ogre::Vector2 m_Value;
};


class AttributeVector3 : public Attribute
{
public:
	AttributeVector3(const wxString &Name, const Ogre::Vector3 &Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_VECTOR3;
	}

	void SetValue(const Ogre::Vector3 &Value)
	{
		m_Value = Value;
	}

	const Ogre::Vector3 &GetValue(void)
	{
		return m_Value;
	}

private:
	Ogre::Vector3 m_Value;
};


class AttributeVector4 : public Attribute
{
public:
	AttributeVector4(const wxString &Name, const Ogre::Vector4 &Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_VECTOR4;
	}

	void SetValue(const Ogre::Vector4 &Value)
	{
		m_Value = Value;
	}

	const Ogre::Vector4 &GetValue(void)
	{
		return m_Value;
	}

private:
	Ogre::Vector4 m_Value;
};


class AttributeQuaternion : public Attribute
{
public:
	AttributeQuaternion(const wxString &Name, const Ogre::Quaternion &Value) :
		Attribute(Name),
		m_Value(Value)
	{
	}

	Ogre::String GetValueString(void) const
	{
		return Ogre::StringConverter::toString(m_Value);
	}

	ATTRIBUTE_TYPE GetType(void) const
	{
		return AT_QUATERNION;
	}

	void SetValue(const Ogre::Quaternion &Value)
	{
		m_Value = Value;
	}

	const Ogre::Quaternion &GetValue(void)
	{
		return m_Value;
	}

private:
	Ogre::Quaternion m_Value;
};
END_NAMESPACE