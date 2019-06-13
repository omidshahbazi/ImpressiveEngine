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
#include "AIStateSeller.h"
#include "AISellerObject.h"
#include "AITimer.h"


BEGIN_NAMESPACE
AIStateSeller::AIStateSeller(AIObject* pOwner) : AIDecisionMaker<AIObject>(pOwner, AIST_SELLER),
													   m_Timer(NULL)
{
	m_Timer = new AITimer(((AISellerObject*)m_Owner)->GetAnnounceInterval());
}


AIStateSeller::~AIStateSeller(void)
{
}


void AIStateSeller::Activate(void)
{
	m_Status = ST_ACTIVE;

}


int AIStateSeller::Process(void)
{
	ActivateIfInactive();

	if(m_Timer->IsReady())
	{
		((AISellerObject*)m_Owner)->Announce();
	}

	m_Status = ProcessSubGoals();

	ReactivateIfFailed();

	if (m_Status == ST_COMPLETED)
	{
		m_Status = ST_INACTIVE;
	}

	return m_Status;
}


void AIStateSeller::Terminate(void)
{
}


bool AIStateSeller::HandleMessage(const Message& Msg)
{
	return ForwardMessageToFrontMostSubGoal(Msg);
}

END_NAMESPACE