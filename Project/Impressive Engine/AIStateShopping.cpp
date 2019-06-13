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
#include "AIStateShopping.h"
#include "AISellerObject.h"
#include "AINPCObject.h"
#include "PathPlanner.h"
#include "GameObject.h"
#include "AIStateMoveToPosition.h"
#include "AIStateIdle.h"


BEGIN_NAMESPACE
AIStateShopping::AIStateShopping(AIObject* pOwner,
								 AISellerObject* pSeller,
								 const Ogre::Vector3& OffsetToSeller) : AIDecisionMaker<AIObject>(pOwner, AIST_SHOPPING),
																		m_Seller(pSeller),
																		m_Destination(OffsetToSeller),
																		m_bStartShopping(false)

{
}


AIStateShopping::~AIStateShopping(void)
{
}


void AIStateShopping::Activate(void)
{
	m_Status = ST_ACTIVE;

	RemoveAllStates();

	const bool bIsAtPosition = m_Owner->IsAtPosition(m_Destination);

	if (!bIsAtPosition)
	{
		AddState(new AIStateMoveToPosition(m_Owner, m_Destination));
	}
	else
	{
		if(!m_bStartShopping)
		{
			RemoveAllStates();

			m_Owner->LookAt(m_Seller->GetGameObject()->GetPosition());

			m_bStartShopping = true;

			AddState(new AIStateIdle(m_Owner, ((AINPCObject*)m_Owner)->STOPT_TIME_AT_SELLER));
		}
	}
}


int AIStateShopping::Process(void)
{
	ActivateIfInactive();

	m_Status = ProcessSubGoals();

	if(m_Status == ST_COMPLETED && !m_bStartShopping)
	{
		m_Status = ST_INACTIVE;
	}

	if(m_Status == ST_COMPLETED && m_bStartShopping)
	{
		RemoveAllStates();

		((AINPCObject*)m_Owner)->LeaveShopping();
	}

	ReactivateIfFailed();

	return m_Status;
}


void AIStateShopping::Terminate(void)
{
}


bool AIStateShopping::HandleMessage(const Message& Msg)
{
	return ForwardMessageToFrontMostSubGoal(Msg);
	
}

END_NAMESPACE