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
#include "AIStatePathFollow.h"
#include "AIObject.h"
#include "PathEdge.h"
#include "AIStateTraverseEdge.h"


BEGIN_NAMESPACE
AIStatePathFollow::AIStatePathFollow(AIObject* Owner,
									 std::list<PathEdge*> Path) : AIDecisionMaker<AIObject>(Owner,
																				  AIST_PATHFOLLOW),
																  m_Path(Path)
{
}


AIStatePathFollow::~AIStatePathFollow(void)
{
}


void AIStatePathFollow::Activate(void)
{
	m_Status = ST_ACTIVE;

	PathEdge* edge = m_Path.front();

	m_Path.pop_front();

	if(edge->GetBehavior() == 0)
	{
		AddState(new AIStateTraverseEdge(m_Owner, edge, m_Path.empty()));
	}
}


int AIStatePathFollow::Process(void)
{
	ActivateIfInactive();

	m_Status = ProcessSubGoals();

	if(m_Status == ST_COMPLETED && !m_Path.empty())
	{
		Activate();
	}

	return m_Status;
}


void AIStatePathFollow::Terminate(void)
{
}

END_NAMESPACE