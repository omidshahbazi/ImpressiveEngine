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
#include "AIStateIdle.h"
#include "AIObject.h"
#include "SteeringBehaviors.h"
#include "AITimer.h"


BEGIN_NAMESPACE
AIStateIdle::AIStateIdle(AIObject* Owner, unsigned int Duration) : AIState<AIObject>(Owner,
																							 AIST_IDLE),
																   m_StandbyDuration(Duration),
																   m_Timer(NULL)
{
	m_Timer = new AITimer(m_StandbyDuration);
}


void AIStateIdle::Activate(void)
{
	m_Status = ST_ACTIVE;

	m_Owner->GetSteering()->SeekOff();
	m_Owner->GetSteering()->ArriveOff();
}


int AIStateIdle::Process(void)
{
	ActivateIfInactive();

	if(m_Timer->IsReady())
	{
		m_Status = ST_COMPLETED;
	}

	ReactivateIfFailed();

	return m_Status;
}


void AIStateIdle::Terminate(void)
{
}
END_NAMESPACE