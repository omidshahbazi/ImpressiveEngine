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
#include "PathPlanner.h"
#include "AIObject.h"
#include "GameObject.h"
#include "PathEdge.h"
#include "PathManager.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Log.h"


BEGIN_NAMESPACE
PathPlanner::PathPlanner(AIObject* Owner) : m_Owner(Owner),
											m_CurrentSearch(NULL),
											m_bSearchRequestSent(false),
											m_Path(NULL),
											m_DestinationPos(Ogre::Vector3::ZERO)
{
}

PathPlanner::~PathPlanner()
{
	GetReadyForNewSearch();
}


PathPlanner::Path PathPlanner::GetPath(void)
{
	if(m_Path.empty())
	{
		int i = 0;
	}
	return m_Path;
}


PathPlanner::Path PathPlanner::GetPathReversed(void)
{

	Path path;

	if (!m_Path.empty())
	{
		for(Path::iterator it = m_Path.begin(); it != m_Path.end(); it++)
		{
			path.push_front(new PathEdge((*it)->GetDestination(), (*it)->GetSource(), 0));
		}
	}

	return path;
}


int PathPlanner::CycleOnce(void)
{
	int result = m_CurrentSearch->CycleOnce();

	if(result == TARGET_NOT_FOUND)
	{
		MessageDispatcher::GetInstance().DispatchTheMessage(SEND_MESSAGE_IMMEDIATELY,
										SENDER_ID_IRRELEVANT,
										m_Owner->GetID(),
										MSGT_NOPATHAVAILABLE);
	}
	else if (result == TARGET_FOUND)
	{
		CalculatePath();

		MessageDispatcher::GetInstance().DispatchTheMessage(SEND_MESSAGE_IMMEDIATELY,
										SENDER_ID_IRRELEVANT,
										m_Owner->GetID(),
										MSGT_PATHREADY);
	}

	return result;
}


bool PathPlanner::RequestPathTo(const Ogre::Vector3 TargetPos)
{
	GetReadyForNewSearch();

	m_bSearchRequestSent = false;
	m_DestinationPos = TargetPos;

	if(m_Owner->CanWalkTo(m_DestinationPos))
	{
		//CalculatePath();
		//MessageDispatcher::GetInstance().DispatchTheMessage(SEND_MESSAGE_IMMEDIATELY,
		//								SENDER_ID_IRRELEVANT,
		//								m_Owner->GetID(),
		//								MSGT_PATHREADY);

		return true;
	}

	const unsigned int ClosestNodeToObject = GetClosestNodeToPosition(m_Owner->GetGameObject()->GetPosition());

	if(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(ClosestNodeToObject).Blocked())
	{
		return false;
	}

	const unsigned int ClosestNodeToTarget = GetClosestNodeToPosition(m_DestinationPos);

	if(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(ClosestNodeToTarget).Blocked())
	{
		return false;
	}

	m_CurrentSearch = new GraphSearchAstar(ClosestNodeToObject, ClosestNodeToTarget);

	m_bSearchRequestSent = true;

	PathManager::GetInstance().Register(this);

	return true;
}


const unsigned int PathPlanner::GetClosestNodeToPosition(const Ogre::Vector3 &Position) const
{
	return GameMap::GetInstance().GetNavGraph()->GetNodeIndexInTerrainSpace(Position);
}


void PathPlanner::SmoothPathEdgesQuick(Path& path)
{
	Path::iterator e1(path.begin()), e2(path.begin());

	++e2;

	while(e2 != path.end())
	{
		if(m_Owner->CanWalkBetween((*e1)->GetSource(), (*e2)->GetDestination()))
		{
			(*e1)->SetDestination((*e2)->GetDestination());
			e2 = path.erase(e2);
		}
		else
		{
			e1 = e2;
			++e2;
		}
	}

}


void PathPlanner::GetReadyForNewSearch(void)
{
	PathManager::GetInstance().UnRegister(this);

	delete m_CurrentSearch;
	m_CurrentSearch = NULL;
}


void PathPlanner::CalculatePath(void)
{
	m_Path.clear();

	if(m_bSearchRequestSent)
	{
		m_Path = m_CurrentSearch->GetPathAsPathEdge();

		const unsigned int closest = GetClosestNodeToPosition(m_Owner->GetGameObject()->GetPosition());

		m_Path.push_front(new PathEdge(m_Owner->GetGameObject()->GetPosition(),
									   GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(closest).GetPosition(), 0));

		m_Path.push_back(new PathEdge(m_Path.back()->GetDestination(), m_DestinationPos, 0));

		SmoothPathEdgesQuick(m_Path);
	}
	else
	{
		m_Path.push_front(new PathEdge(m_Owner->GetGameObject()->GetPosition(), m_DestinationPos, 0));
	}
}

END_NAMESPACE