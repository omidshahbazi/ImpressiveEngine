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

#include "DoClasses.h"
#include "ScenesHolder.h"
#include "GameObject.h"


BEGIN_NAMESPACE
DoBase::DoBase(wxString Description) :
	m_Description(Description)
{
}


//
// RemoveGameObjectDo
//


RemoveGameObjectDo::RemoveGameObjectDo(GameObject *Object, bool Reverse) :
	m_Object(Object),
	m_Reverse(Reverse)
{
}


void RemoveGameObjectDo::Do(void)
{
	DoBase(m_Reverse);
}


void RemoveGameObjectDo::Undo(void)
{
	DoBase(!m_Reverse);
}


void RemoveGameObjectDo::DoBase(bool Undo)
{
	if (Undo)
	{
		//m_Object->_GetSceneNode()->setParent(m_SceneManager->getRootSceneNode());

		m_Object->SetVisible(true);

		m_Object->SetAsRemoved(false);

		m_Object->GetParent()->AddChild(m_Object);

		ScenesHolder::GetInstance().GetCurrentScene()->RefreshSceneControlContent();
	}
	else
	{
		//m_Object->_GetSceneNode()->setParent(NULL);

		m_Object->SetVisible(false);

		m_Object->ShowBoundingBox(false);

		m_Object->SetAsRemoved(true);
		
		m_Object->GetParent()->RemoveChild(m_Object);

		ScenesHolder::GetInstance().GetCurrentScene()->RefreshSceneControlContent();
	}
}


//
// ChangePositionDo
//


//ChangePositionDo::ChangePositionDo(GameObject *Object, Ogre::Vector3 Position) :
//	m_Object(Object),
//	m_Position(Position)
//{
//}
//
//
//void ChangePositionDo::Do(void)
//{
//	Ogre::Vector3 temp = m_Object->GetPosition();
//
//	m_Object->SetPosition(m_Position);
//
//	m_Position = temp;
//}
//
//
//void ChangePositionDo::Undo(void)
//{
//	Do();
//}


END_NAMESPACE

#endif