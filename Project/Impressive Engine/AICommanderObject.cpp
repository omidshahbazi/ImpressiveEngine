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
#include "AICommanderObject.h"
#include "GameObject.h"
#include "AIBrain.h"
#include "AISoldierObject.h"
#include "SparseGraph.h"
#include "AIStateCommanderEvaluator.h"
#include "GraphNode.h"
#include "AIObjectType.h"
#include "SteeringBehaviors.h"


BEGIN_NAMESPACE
AICommanderObject::AICommanderObject(const unsigned int ID, GameObject* gameObject) : AIEnemyObject(ID, gameObject),
																					  m_FireTeamType(FTT_THREESOME)
{
	m_Type = AIOT_COMMANDER;

	m_Soldiers.clear();

	m_Brain->AddStateEvaluator(new AIStateCommanderEvaluator(1));
}


AICommanderObject::~AICommanderObject(void)
{
}


void AICommanderObject::AddSoldier(const unsigned ID)
{
}


void AICommanderObject::AddSoldier(const Ogre::String Name)
{
}


void AICommanderObject::AddSoldier(AISoldierObject* Soldier)
{
	if (m_FireTeamType == FTT_THREESOME && m_Soldiers.size() == 2) return;

	if (m_FireTeamType == FTT_FOURSOME && m_Soldiers.size() == 3) return;

	Soldier->SetCommander(this);
	if(m_Soldiers.size() == 0)
	{
		Soldier->SetPositionType(SPT_RIGHT);
	}
	else
	{
		Soldier->SetPositionType(SPT_LEFT);
	}
	Ogre::Vector3 Offset = CalculateOffsetToSoldier(Soldier);
	Soldier->SetOffsetToCommander(Offset);
	m_Soldiers.push_back(Soldier);
}


AISoldierObject* AICommanderObject::GetSoldier(const unsigned int Index)
{
	if(m_Soldiers.empty() || Index > m_Soldiers.size()) return NULL;

	return m_Soldiers[Index];
}


const FireTeamType& AICommanderObject::GetFireTeamType(void) const
{
	return m_FireTeamType;
}


Ogre::Vector3 AICommanderObject::CalculateOffsetToSoldier(AISoldierObject* Soldier)
{
	if(m_FireTeamType == FTT_THREESOME)
	{
		if(Soldier->GetPositionType() == SPT_RIGHT)
		{
			return m_GameObject->GetPosition() + (m_GameObject->GetForward() * -m_BoundingRadius * 1.5f) + (m_GameObject->GetRight() * Soldier->GetBoundingRadius() * 1.5f);
		}
		else if (Soldier->GetPositionType() == SPT_LEFT)
		{
			return m_GameObject->GetPosition() + (m_GameObject->GetForward() * -m_BoundingRadius * 1.5f) + (m_GameObject->GetRight() * -Soldier->GetBoundingRadius() * 1.5f);
		}
	}

	return Ogre::Vector3::ZERO;
}


void AICommanderObject::CalculateOffsetToSoldiers(void)
{
	for(unsigned int i = 0; i < m_Soldiers.size(); i++)
	{
		if(m_FireTeamType == FTT_THREESOME)
		{
			if(m_Soldiers[i]->GetPositionType() == SPT_RIGHT)
			{
				m_Soldiers[i]->SetOffsetToCommander(m_GameObject->GetPosition() + (m_GameObject->GetForward() * -m_BoundingRadius * 1.5f) + (m_GameObject->GetRight() * m_Soldiers[i]->GetBoundingRadius() * 1.5f));
			}
			else if (m_Soldiers[i]->GetPositionType() == SPT_LEFT)
			{
				m_Soldiers[i]->SetOffsetToCommander(m_GameObject->GetPosition() + (m_GameObject->GetForward() * -m_BoundingRadius * 1.5f) + (m_GameObject->GetRight() * -m_Soldiers[i]->GetBoundingRadius() * 1.5f));
			}
		}
	}
}


const bool AICommanderObject::AreSoldiresAtPositions(void) const
{
	for(unsigned int i = 0; i < m_Soldiers.size(); i++)
	{
		if(!m_Soldiers[i]->IsAtPosition(m_Soldiers[i]->GetOffsetToCommander()))
		{
			return false;
		}
	}

	return true;
}



void AICommanderObject::SoldiersPursuitOn(void)
{
	for(unsigned int i = 0; i < m_Soldiers.size(); i++)
	{
		m_Soldiers[i]->GetSteering()->OffsetPursuitOn();
	}
}


void AICommanderObject::SoldiersPursuitOff(void)
{
	for(unsigned int i = 0; i < m_Soldiers.size(); i++)
	{
		m_Soldiers[i]->GetSteering()->OffsetPursuitOff();
	}
}


void AICommanderObject::SoldiersLookAt(Ogre::Vector3& Target)
{
	for(unsigned int i = 0; i < m_Soldiers.size(); i++)
	{
		if(m_FireTeamType == FTT_THREESOME)
		{
			if(m_Soldiers[i]->GetPositionType() == SPT_RIGHT)
			{
				m_Soldiers[i]->LookAt(Target + (m_GameObject->GetRight() * m_Soldiers[i]->GetBoundingRadius() * 1.5f));
			}
			else if (m_Soldiers[i]->GetPositionType() == SPT_LEFT)
			{
				m_Soldiers[i]->LookAt(Target + (m_GameObject->GetRight() * -m_Soldiers[i]->GetBoundingRadius() * 1.5f));
			}
		}
	}
}


void AICommanderObject::SoldiersRotateSmooth(const float& SmoothTime)
{
	for(unsigned int i = 0; i < m_Soldiers.size(); i++)
	{
		m_Soldiers[i]->RotateSmoothTo(m_GameObject->GetPosition(), SmoothTime);
	}
}


const bool AICommanderObject::SoldierExists(AISoldierObject* Soldier)
{
	for(unsigned int i = 0; i < m_Soldiers.size(); i++)
	{
		if(m_Soldiers[i]->GetID() == Soldier->GetID())
		{
			return true;
		}
	}

	return false;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AICommanderObject)
	BEGIN_DEFINE_TYPE_BASE(AIEnemyObject, AICommanderObject, "AICommanderObject")
		DEFINE_FUNCTION("AddSoldier", (void(AICommanderObject::*)(AISoldierObject*))&AICommanderObject::AddSoldier)
		DEFINE_FUNCTION("GetSoldier", (AISoldierObject*(AICommanderObject::*)(const unsigned int))&AICommanderObject::GetSoldier)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE