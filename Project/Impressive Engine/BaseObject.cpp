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
#include "BaseObject.h"
#include "GameObject.h"
#include "AIObjectType.h"
#include "Mathematics.h"


BEGIN_NAMESPACE
BaseObject::BaseObject(const unsigned int ID, GameObject* gameObject) : m_ID(ID),
																			m_GameObject(gameObject),
																			m_Type(AIOT_NONE),
																			m_bTag(false),
																			m_BoundingRadius(0.0f),
																			m_bPossessed(false)
{
	const Ogre::AxisAlignedBox Box = m_GameObject->GetWorldAABB();

	m_BoundingRadius = Mathematics::Max((Box.getMaximum().x - Box.getMinimum().x),
										(Box.getMaximum().z - Box.getMinimum().z)) / 2;
}


const unsigned int &BaseObject::GetID(void) const
{
	return m_ID;
}


GameObject* BaseObject::GetGameObject(void)
{
	return m_GameObject;
}


void BaseObject::SetType(const AIObjectType Type)
{
	m_Type = Type;
}


const AIObjectType &BaseObject::GetType(void) const
{
	return m_Type;
}


const bool &BaseObject::IsPossessed(void) const
{
	return m_bPossessed;
}


void BaseObject::Tag(void)
{
	m_bTag = true;
}


const bool &BaseObject::IsTagged(void) const
{
	return m_bTag;
}

void BaseObject::UnTag(void)
{
	m_bTag = false;
}


const float &BaseObject::GetBoundingRadius() const
{
	return m_BoundingRadius;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(BaseObject)
	BEGIN_DEFINE_TYPE(BaseObject, "BaseObject")
		DEFINE_FUNCTION("SetType", (void(BaseObject::*)(const AIObjectType))&BaseObject::SetType)
		DEFINE_FUNCTION("GetBoundingRadius", &BaseObject::GetBoundingRadius)
		DEFINE_FUNCTION("GetGameObject", &BaseObject::GetGameObject)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE