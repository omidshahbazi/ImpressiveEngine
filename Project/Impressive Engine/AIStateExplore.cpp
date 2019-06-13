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
#include "AIStateExplore.h"
#include "AINPCObject.h"
#include "AISellerObject.h"
#include "AIObjectType.h"
#include "GameObject.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "PathPlanner.h"
#include "AIStateMoveToPosition.h"


BEGIN_NAMESPACE
AIStateExplore::AIStateExplore(AIObject* pOwner) : AIDecisionMaker<AIObject>(pOwner, AIST_EXPLORE),
												   m_bDestinationAvailable(false)
{
}


void AIStateExplore::Activate(void)
{
	m_Status = ST_ACTIVE;

	RemoveAllStates();

	if (!m_bDestinationAvailable)
	{
		m_Destination = GameMap::GetInstance().GetNavGraph()->GetRandomLocation();

		m_bDestinationAvailable = true;

		AddState(new AIStateMoveToPosition(m_Owner, m_Destination));
	}
}


int AIStateExplore::Process(void)
{
	ActivateIfInactive();

	m_Status = ProcessSubGoals();

	ReactivateIfFailed();

	return m_Status;
}


void AIStateExplore::Terminate(void)
{
}


bool AIStateExplore::HandleMessage(const Message& Msg)
{
	bool bHandled = ForwardMessageToFrontMostSubGoal(Msg);

	if(!bHandled)
	{
		if (Msg.m_Message == MSGT_COMETOBUY)
		{
			if(m_Owner->GetType() == AIOT_NPC)
			{
				AISellerObject* Seller = (AISellerObject*)Msg.m_ExtraInfo;

				if(!((AINPCObject*)m_Owner)->IsGoingShopping() && Seller->RoomAvailable())
				{
					((AINPCObject*)m_Owner)->GoShopping(Seller);
				}

				return true;
			}
		}
		else
		{
			return false;
		}
	}

	return bHandled;
}

END_NAMESPACE