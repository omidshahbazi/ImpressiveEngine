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
#include "AIStateMoveToPosition.h"
#include "AIStateSeekToPosition.h"
#include "AIStatePathFollow.h"
#include "AIObject.h"
#include "PathPlanner.h"
#include "PathEdge.h"
#include "Log.h"


BEGIN_NAMESPACE
AIStateMoveToPosition::AIStateMoveToPosition(AIObject* Owner,
											 Ogre::Vector3 Pos) : AIDecisionMaker<AIObject>(Owner,
																							AIST_MOVETOPOSITION),
																  m_Destination(Pos)
{
}


AIStateMoveToPosition::~AIStateMoveToPosition(void)
{
}


void AIStateMoveToPosition::Activate(void)
{
	m_Status = ST_ACTIVE;

	RemoveAllStates();

	if(m_Owner->GetPathPlanner()->RequestPathTo(m_Destination))
	{
		m_Owner->LookAt(m_Destination);

		AddState(new AIStateSeekToPosition(m_Owner, m_Destination));
	}
}


int AIStateMoveToPosition::Process(void)
{
	ActivateIfInactive();

	m_Status = ProcessSubGoals();

	ReactivateIfFailed();

	return m_Status;
}


void AIStateMoveToPosition::Terminate(void)
{
}


bool AIStateMoveToPosition::HandleMessage(const Message& Msg)
{
	const bool bHandled = ForwardMessageToFrontMostSubGoal(Msg);

	if(!bHandled)
	{
		if (Msg.m_Message == MSGT_PATHREADY)
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


END_NAMESPACE