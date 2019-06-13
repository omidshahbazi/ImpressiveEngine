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
#include "AIStateTraverseEdge.h"
#include "AIObject.h"
#include "PathEdge.h"
#include "SteeringBehaviors.h"
#include "Time.h"


BEGIN_NAMESPACE
AIStateTraverseEdge::AIStateTraverseEdge(AIObject* Owner,
										 PathEdge* Edge,
										 const bool IsLastEdge) : AIState<AIObject>(Owner, AIST_TRAVERSEEDGE),
																  m_Edge(Edge),
																  m_bLastEdgeInPath(IsLastEdge)

{
}


AIStateTraverseEdge::~AIStateTraverseEdge(void)
{
}


void AIStateTraverseEdge::Activate(void)
{
	m_Status = ST_ACTIVE;

	m_TimeStartToTraverse = Time::GetInstance().GetRealTime();

	m_TimeExpectedtoReachDestination = m_Owner->CalculateTimeToReachPosition(m_Edge->GetDestination());

	const unsigned int MarginOfError = 200;

	m_TimeExpectedtoReachDestination += MarginOfError;

	m_Owner->LookAt(m_Edge->GetDestination());

	m_Owner->GetSteering()->SetTarget(m_Edge->GetDestination());

	if(!m_bLastEdgeInPath)
	{
		m_Owner->GetSteering()->SeekOn();
	}
	else
	{
		m_Owner->GetSteering()->ArriveOn();
	}
}


int AIStateTraverseEdge::Process(void)
{
	ActivateIfInactive();

	if(IsStuck())
	{
		m_Status = ST_FAILED;
	}
	else
	{
		if(m_Owner->IsAtPosition(m_Edge->GetDestination()))
		{
			m_Status = ST_COMPLETED;
		}
	}

	return m_Status;
}


void AIStateTraverseEdge::Terminate(void)
{
	m_Owner->GetSteering()->SeekOff();
	m_Owner->GetSteering()->ArriveOff();
}


bool AIStateTraverseEdge::IsStuck(void)
{
	const unsigned int TimeTaken = Time::GetInstance().GetRealTime() - m_TimeStartToTraverse;

	if(TimeTaken > m_TimeExpectedtoReachDestination)
	{
		return true;
	}

	return false;
}

END_NAMESPACE