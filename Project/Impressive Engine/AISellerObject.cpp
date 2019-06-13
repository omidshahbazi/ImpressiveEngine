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
#include "AISellerObject.h"
#include "AIObjectType.h"
#include "AINPCObject.h"
#include "GameObject.h"
#include "AIBrain.h"
#include "AIEngine.h"
#include "AIStateSellerEvaluator.h"
#include "CustomerStand.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time.h"


BEGIN_NAMESPACE
AISellerObject::AISellerObject(const unsigned int ID, GameObject* gameObject) : AIObject(ID, gameObject),
																				m_LastTimeAnnounced(0.0),
																				m_AnnounceInterval(10000),
																				m_CustomerStands(NULL),
																				OFFSET_TO_CUSTOMER(300)
{
	m_Type = AIOT_SELLER;

	m_Brain->AddStateEvaluator(new AIStateSellerEvaluator(1));

	//Ogre::Vector3 p1 = m_GameObject->GetPosition() + m_GameObject->GetForward() * m_BoundingRadius * 32;
	//Ogre::Vector3 p2 = m_GameObject->GetPosition() + m_BoundingRadius * 16 * (m_GameObject->GetForward() + m_GameObject->GetRight());
	//Ogre::Vector3 p3 = m_GameObject->GetPosition() + m_BoundingRadius * 16 * (m_GameObject->GetForward() + m_GameObject->GetRight().reflect(m_GameObject->GetRight()));

	//const unsigned short MAX_LIMIT = GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetWorldSize() / 2;

	//GraphNode nd;

	//if(p1.x >= -MAX_LIMIT && p1.x <= MAX_LIMIT && p1.z >= -MAX_LIMIT && p1.z <= MAX_LIMIT)
	//{
	//	nd = GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(p1);
	//	if(!nd.Blocked())
	//	{
	//		m_CustomerStands.push_back(new CustomerStand(nd.GetPosition())); 
	//	}
	//}

	//if(p2.x >= -MAX_LIMIT && p2.x <= MAX_LIMIT && p2.z >= -MAX_LIMIT && p2.z <= MAX_LIMIT)
	//{
	//	nd = GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(p2);
	//	if(!nd.Blocked())
	//	{
	//		m_CustomerStands.push_back(new CustomerStand(nd.GetPosition())); 
	//	}
	//}

	//if(p3.x >= -MAX_LIMIT && p3.x <= MAX_LIMIT && p3.z >= -MAX_LIMIT && p3.z <= MAX_LIMIT)
	//{
	//	nd = GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(p3);
	//	if(!nd.Blocked())
	//	{
	//		m_CustomerStands.push_back(new CustomerStand(nd.GetPosition()));
	//	}
	//}
}


AISellerObject::~AISellerObject(void)
{

}


void AISellerObject::AddRoom(const Ogre::Vector3 Position)
{
	const unsigned short MAX_LIMIT = GameMap::GetInstance().GetNavGraph()->GetWorldSize() / 2;
	GraphNode nd;

	if(Position.x >= -MAX_LIMIT && Position.x <= MAX_LIMIT && Position.z >= -MAX_LIMIT && Position.z <= MAX_LIMIT)
	{
		nd = GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(Position);
		if(!nd.Blocked())
		{
			m_CustomerStands.push_back(new CustomerStand(nd.GetPosition())); 
		}
	}
}


const CustomerStand* AISellerObject::GetRoom(const unsigned int Index)
{
	if(m_CustomerStands.empty() || m_CustomerStands.size() < Index) return NULL;

	return m_CustomerStands[Index];
}


void AISellerObject::Announce(void)
{
	if(Time::GetInstance().GetRealTime() - m_LastTimeAnnounced <= m_AnnounceInterval || !RoomAvailable())
	{
		return;
	}


	std::map<const  unsigned int, AIObject*>::const_iterator it = AIEngine::GetInstance().GetAIObjects().begin();

	while(it != AIEngine::GetInstance().GetAIObjects().end())
	{
		if(it->second->GetType() == AIOT_NPC)
		{
			float Distance = (it->second->GetGameObject()->GetPosition() - GetGameObject()->GetPosition()).squaredLength();

			if(Distance <= OFFSET_TO_CUSTOMER * OFFSET_TO_CUSTOMER)
			{
				MessageDispatcher::GetInstance().DispatchTheMessage(SEND_MESSAGE_IMMEDIATELY,
																	SENDER_ID_SALESMAN,
																	it->second->GetID(),
																	MSGT_COMETOBUY,
																	this);
			}
		}

		++it;
	}

	m_LastTimeAnnounced = Time::GetInstance().GetRealTime();
}


const unsigned int AISellerObject::GetAnnounceInterval(void) const
{
	return m_AnnounceInterval;
}


const bool AISellerObject::RoomAvailable(void) const
{
	CustomerStandVector::const_iterator room = m_CustomerStands.begin();

	for(room; room != m_CustomerStands.end(); room++)
	{
		if(!(*room)->IsTaken()) return true;
	}

	return false;
}


const bool AISellerObject::Reserveed(AINPCObject* NPC) const
{
	if(m_CustomerStands.empty()) return false;

	CustomerStandVector::const_iterator room = m_CustomerStands.begin();

	for(room; room != m_CustomerStands.end(); room++)
	{
		if((*room)->GetCustomerID() == NPC->GetID()) return true;
	}

	return false;
}


void AISellerObject::ReserveRoom(AINPCObject* NPC, Ogre::Vector3& RoomPos)
{
	CustomerStandVector::const_iterator room = m_CustomerStands.begin();

	for(room; room != m_CustomerStands.end(); room++)
	{
		if(!(*room)->IsTaken())
		{
			(*room)->Reserve(NPC->GetID());
			RoomPos = (*room)->GetPosition();
			return;
		}
	}
}


void AISellerObject::FreeRoom(AINPCObject* NPC)
{
	CustomerStandVector::const_iterator room = m_CustomerStands.begin();

	for(room; room != m_CustomerStands.end(); room++)
	{
		if((*room)->GetCustomerID() == NPC->GetID())
		{
			(*room)->Realease();
			return;
		}
	}
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AISellerObject)
	BEGIN_DEFINE_TYPE_BASE(AIObject, AISellerObject, "AISellerObject")
		DEFINE_FUNCTION("AddRoom", (void(AISellerObject::*)(const Ogre::Vector3))&AISellerObject::AddRoom)
		DEFINE_FUNCTION("GetRoom", (CustomerStand*(AISellerObject::*)(const unsigned int))&AISellerObject::GetRoom)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE