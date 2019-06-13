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
#include "StdAfxOGRE.h"
#include "Common.h"

BEGIN_NAMESPACE
class GameObject;


class DoBase
{
public:
	DoBase(wxString Description = "");
	
	virtual void Do(void) = 0;
	virtual void Undo(void) = 0;

	const wxString &GetDescription(void)
	{
		return m_Description;
	}

private:
	wxString m_Description;
};


class RemoveGameObjectDo : public DoBase
{
public:
	RemoveGameObjectDo(GameObject *Object, bool Reverse = false);
	
	void Do(void);
	void Undo(void);

private:
	void DoBase(bool Undo);

private:
	GameObject *m_Object;
	bool m_Reverse;
};


//class ChangePositionDo : public DoBase
//{
//public:
//	ChangePositionDo(GameObject *Object, Ogre::Vector3 Position);
//	
//	void Do(void);
//	void Undo(void);
//
//private:
//	GameObject *m_Object;
//	Ogre::Vector3 m_Position;
//};
//
//
//class ChangeScaleDo : public DoBase
//{
//public:
//	ChangeScaleDo(GameObject *Object, Ogre::Vector3 Scale);
//	
//	void Do(void);
//	void Undo(void);
//
//private:
//	GameObject *m_Object;
//	Ogre::Vector3 m_Scale;
//};
//
//
//class ChangeOrientationDo : public DoBase
//{
//public:
//	ChangeOrientationDo(GameObject *Object, Ogre::Quaternion Orientation);
//	
//	void Do(void);
//	void Undo(void);
//
//private:
//	GameObject *m_Object;
//	Ogre::Quaternion m_Orientation;
//};

END_NAMESPACE

#endif