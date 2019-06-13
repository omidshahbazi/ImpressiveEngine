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

#include "Entity.h"


BEGIN_NAMESPACE
Entity *Singleton<Entity>::m_Instance = NULL;


//Entity::Entity(void)
//{
//}
//
//Entity::~Entity(void)
//{
//}

void Entity::Initialize(wxWindow *Parent)
{
	m_Control = new EntityControl(Parent);
}


void Entity::Refresh(void)
{
	m_Control->ReloadContent();
}
END_NAMESPACE

#endif