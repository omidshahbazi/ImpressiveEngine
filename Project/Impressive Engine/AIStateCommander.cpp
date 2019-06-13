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
#include "AIStateCommander.h"
#include "AIObject.h"
#include "WaypointType.h"
#include "AIStateMoveToPosition.h"
#include "AIStateIdle.h"
#include "PathPlanner.h"
#include "AIStatePathFollow.h"
#include "GameObject.h"


BEGIN_NAMESPACE
AIStateCommander::AIStateCommander(AIObject* Owner,
						   WaypointVector Waypoints) : AIDecisionMaker<AIObject>(Owner, AIST_COMMANDER),
													  m_Waypoints(Waypoints),
													  m_WType(((AICommanderObject*)m_Owner)->GetWaypointType()),
													  m_PathEdgesSize(m_Waypoints.size()),
													  m_WaypointIndex(0),
													  m_bReachedEnd(false),
													  m_bIsIdle(false)
{
	if(m_WType == WT_OPENWAY) m_PathEdgesSize--;
}


void AIStateCommander::Activate(void)
{
	m_Status = ST_ACTIVE;

	const bool bIsAtPosition = m_Owner->IsAtPosition(m_Waypoints[m_WaypointIndex]);

	if(!bIsAtPosition)
	{
		if(!((AICommanderObject*)m_Owner)->AreSoldiresAtPositions())
		{
			m_bIsIdle = true;

			AddState(new AIStateIdle(m_Owner, 1));
		}
		else
		{
			m_bIsIdle = false;

			((AICommanderObject*)m_Owner)->SoldiersLookAt(m_Waypoints[m_WaypointIndex]);

			AddState(new AIStateMoveToPosition(m_Owner, m_Waypoints[m_WaypointIndex]));
		}
	}
	else
	{
		m_bIsIdle = true;

		UpdateCounter();

		AddState(new AIStateIdle(m_Owner, 1500));
	}
}


int AIStateCommander::Process(void)
{
	ActivateIfInactive();

	m_Status = ProcessSubGoals();

	ReactivateIfFailed();

	if (m_Status == ST_COMPLETED)
	{
		RemoveAllStates();

		m_Status = ST_INACTIVE;

		if(m_bIsIdle)
		{
			m_Owner->RotateSmoothTo(m_Waypoints[m_WaypointIndex], 0.05f);

			if(m_Owner->IsFacing(m_Waypoints[m_WaypointIndex]))
			{
				((AICommanderObject*)m_Owner)->CalculateOffsetToSoldiers();
			}

			((AICommanderObject*)m_Owner)->SoldiersRotateSmooth(0.05f);
		}
	}

	return m_Status;
}


void AIStateCommander::Terminate(void)
{
}


bool AIStateCommander::HandleMessage(const Message& Msg)
{
	const bool bHandled = ForwardMessageToFrontMostSubGoal(Msg);

	if(!bHandled)
	{
		if(Msg.m_Message == MSGT_PATHREADY)
		{
			RemoveAllStates();

			AddState(new AIStatePathFollow(m_Owner, m_Owner->GetPathPlanner()->GetPath()));

			return true;
		}
		else if (Msg.m_Message == MSGT_NOPATHAVAILABLE)
		{
			m_Status = ST_FAILED;

			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}


void AIStateCommander::UpdateCounter(void)
{
	if(!m_bReachedEnd && m_WaypointIndex >= m_Waypoints.size() - 1)
	{
		m_bReachedEnd = true;
	}
	else if (m_bReachedEnd && m_WaypointIndex <= 0)
	{
		m_bReachedEnd = false;
	}
	
	if(m_WType == WT_CLOSEDWAY)
	{
		m_WaypointIndex++;

		if(m_bReachedEnd)
		{
			m_WaypointIndex = 0;
		}
	}
	else
	{
		if(!m_bReachedEnd)
		{
			m_WaypointIndex++;
		}
		else
		{
			m_WaypointIndex--;
		}
	}

}
END_NAMESPACE