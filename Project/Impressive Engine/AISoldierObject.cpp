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
#include "AISoldierObject.h"
#include "AICommanderObject.h"
#include "SteeringBehaviors.h"
#include "AIObjectType.h"
#include "GameObject.h"


BEGIN_NAMESPACE
AISoldierObject::AISoldierObject(const unsigned int ID, GameObject* gameObject) : AIEnemyObject(ID, gameObject),
																				  m_Commander(NULL)
{
	m_Type = AIOT_SOLDIER;

	m_Steering->OffsetPursuitOn();
	m_Steering->SeparationOff();
}


AISoldierObject::~AISoldierObject(void)
{
}


void AISoldierObject::SetCommander(AICommanderObject* Commander)
{
	m_Commander = Commander;
}


AICommanderObject* AISoldierObject::GetCommander(void)
{
	return m_Commander;
}


void AISoldierObject::SetOffsetToCommander(Ogre::Vector3 Offset)
{
	m_OffsetToCommander = Offset;
}


const Ogre::Vector3& AISoldierObject::GetOffsetToCommander(void) const
{
	return m_OffsetToCommander;
}

void AISoldierObject::SetPositionType(SoldierPositionType Type)
{
	m_PositionType = Type;
}


const SoldierPositionType AISoldierObject::GetPositionType(void) const
{
	return m_PositionType;
}


void AISoldierObject::GoToPosition(void)
{
	m_GameObject->SetPosition(m_OffsetToCommander);
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AISoldierObject)
	BEGIN_DEFINE_TYPE_BASE(AIEnemyObject, AISoldierObject, "AISoldierObject")
		DEFINE_FUNCTION("SetCommander", (void(AISoldierObject::*)(AICommanderObject*))&AISoldierObject::SetCommander)
		DEFINE_FUNCTION("GetCommander", &AISoldierObject::GetCommander)
		DEFINE_FUNCTION("GetOffsetToCommander", &AISoldierObject::GetOffsetToCommander)
		DEFINE_FUNCTION("GoToPosition", &AISoldierObject::GoToPosition)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE