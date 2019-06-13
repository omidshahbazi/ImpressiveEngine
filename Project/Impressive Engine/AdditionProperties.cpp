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

#include "AdditionProperties.h"

//BEGIN_NAMESPACE

//
// Vector2
//

WX_PG_IMPLEMENT_VARIANT_DATA(Vector2VariantData, Vector2)
//WX_PG_IMPLEMENT_VARIANT_DATA(Vector2)
WX_PG_IMPLEMENT_PROPERTY_CLASS(Vector2Property, wxPGProperty, Vector2, const Vector2&, TextCtrl)


Vector2Property::Vector2Property(const wxString& Label, const wxString& Name, const Vector2& Value) :
	wxPGProperty(Label, Name)
{
	//SetValue(Vector2ToVariant(Value));
	m_value << Value;

	AddPrivateChild(new wxFloatProperty("X", wxPG_LABEL, Value.x));
	AddPrivateChild(new wxFloatProperty("Y", wxPG_LABEL, Value.y));
}


Vector2Property::~Vector2Property()
{
}


void Vector2Property::RefreshChildren()
{
    if (!GetCount()) 
		return;
	
	Vector2 &vector = Vector2FromVariant(m_value);
	Item(0)->SetValue(vector.x);
	Item(1)->SetValue(vector.y);
}


void Vector2Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    Vector2 &vector = Vector2FromVariant(thisValue);

    switch (childIndex)
    {
        case 0: vector.x = childValue.GetDouble(); break;
        case 1: vector.y = childValue.GetDouble(); break;
    }
}


//
// Vector3
//

WX_PG_IMPLEMENT_VARIANT_DATA(Vector3VariantData, Vector3)
//WX_PG_IMPLEMENT_VARIANT_DATA(Vector3)
WX_PG_IMPLEMENT_PROPERTY_CLASS(Vector3Property, wxPGProperty, Vector3, const Vector3&, TextCtrl)


Vector3Property::Vector3Property(const wxString& Label, const wxString& Name, const Vector3& Value) :
	wxPGProperty(Label, Name)
{
	//SetValue(Vector3ToVariant(Value));
	m_value << Value;

	AddPrivateChild(new wxFloatProperty("X", wxPG_LABEL, Value.x));
	AddPrivateChild(new wxFloatProperty("Y", wxPG_LABEL, Value.y));
	AddPrivateChild(new wxFloatProperty("Z", wxPG_LABEL, Value.z));
}


Vector3Property::~Vector3Property()
{
}


void Vector3Property::RefreshChildren()
{
    if (!GetCount()) 
		return;
	
	Vector3 &vector = Vector3FromVariant(m_value);
	Item(0)->SetValue(vector.x);
	Item(1)->SetValue(vector.y);
	Item(2)->SetValue(vector.z);
}


void Vector3Property::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    Vector3 &vector = Vector3FromVariant(thisValue);

    switch (childIndex)
    {
        case 0: vector.x = childValue.GetDouble(); break;
        case 1: vector.y = childValue.GetDouble(); break;
        case 2: vector.z = childValue.GetDouble(); break;
    }
}


//
// Quaternion
//

WX_PG_IMPLEMENT_VARIANT_DATA(QuaternionVariantData, Quaternion)
//WX_PG_IMPLEMENT_VARIANT_DATA(Quaternion)
WX_PG_IMPLEMENT_PROPERTY_CLASS(QuaternionProperty, wxPGProperty, Quaternion, const Quaternion&, TextCtrl)


QuaternionProperty::QuaternionProperty(const wxString& Label, const wxString& Name, const Quaternion& Value) :
	wxPGProperty(Label, Name)
{
	//SetValue(QuaternionToVariant(Value));
	m_value << Value;

	AddPrivateChild(new wxFloatProperty("W", wxPG_LABEL, Value.w));
	AddPrivateChild(new wxFloatProperty("X", wxPG_LABEL, Value.x));
	AddPrivateChild(new wxFloatProperty("Y", wxPG_LABEL, Value.y));
	AddPrivateChild(new wxFloatProperty("Z", wxPG_LABEL, Value.z));
}


QuaternionProperty::~QuaternionProperty()
{
}


void QuaternionProperty::RefreshChildren()
{
    if (!GetCount()) 
		return;
	
	Quaternion &quat = QuaternionFromVariant(m_value);
	Item(0)->SetValue(quat.w);
	Item(1)->SetValue(quat.x);
	Item(2)->SetValue(quat.y);
	Item(3)->SetValue(quat.z);
}


void QuaternionProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
    Quaternion &quat = QuaternionFromVariant(thisValue);

    switch (childIndex)
    {
        case 0: quat.w = childValue.GetDouble(); break;
        case 1: quat.x = childValue.GetDouble(); break;
        case 2: quat.y = childValue.GetDouble(); break;
        case 3: quat.z = childValue.GetDouble(); break;
    }
}


//END_NAMESPACE
#endif