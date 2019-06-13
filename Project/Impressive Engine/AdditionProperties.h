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

#pragma once

#include "StdAfxwxWidgets.h"
//#include "Common.h"
#include "StdAfxOGRE.h"

typedef Ogre::Vector2 Vector2;
typedef Ogre::Vector3 Vector3;
typedef Ogre::Quaternion Quaternion;

//BEGIN_NAMESPACE


//
// Vector2
//

WX_PG_DECLARE_VARIANT_DATA(Vector2VariantData, Vector2, wxPG_NO_DECL)
//WX_PG_DECLARE_VARIANT_DATA(Vector2)
	

class Vector2Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(Vector2Property)

public:
	Vector2Property(const wxString &Label = wxPG_LABEL, const wxString &Name = wxPG_LABEL, const Vector2 &Value = Vector2());

	virtual ~Vector2Property();

	WX_PG_DECLARE_PARENTAL_METHODS()
};


//
// Vector3
//

WX_PG_DECLARE_VARIANT_DATA(Vector3VariantData, Vector3, wxPG_NO_DECL)
//WX_PG_DECLARE_VARIANT_DATA(Vector3)
	

class Vector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(Vector3Property)

public:
	Vector3Property(const wxString &Label = wxPG_LABEL, const wxString &Name = wxPG_LABEL, const Vector3 &Value = Vector3());

	virtual ~Vector3Property();
	
	WX_PG_DECLARE_PARENTAL_METHODS()
};
	

//
// Quaternion
//
WX_PG_DECLARE_VARIANT_DATA(QuaternionVariantData, Quaternion, wxPG_NO_DECL)
//WX_PG_DECLARE_VARIANT_DATA(Quaternion)


class QuaternionProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(QuaternionProperty)

public:
	QuaternionProperty(const wxString &Label = wxPG_LABEL, const wxString &Name = wxPG_LABEL, const Quaternion &Value = Quaternion());

	virtual ~QuaternionProperty();

	WX_PG_DECLARE_PARENTAL_METHODS()
};

//END_NAMESPACE
#endif