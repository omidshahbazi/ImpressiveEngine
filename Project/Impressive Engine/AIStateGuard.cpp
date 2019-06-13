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
#include "AIStateGuard.h"
#include "AIStateSeekToPosition.h"
#include "AIStatePathFollow.h"
#include "AIStateMoveToPosition.h"
#include "AIStateIdle.h"
#include "AIStateWallFollow.h"
#include "SteeringBehaviors.h"
#include "AIObject.h"
#include "GameMap.h"
#include "GameObject.h"
#include "StdAfxBoost.h"
#include "PathPlanner.h"
#include "PathEdge.h"
#include "Mathematics.h"
#include "Log.h"


BEGIN_NAMESPACE
enum WaypointType
{
	WT_OPENWAY,
	WT_CLOSEDWAY
};


enum WallSideType
{
	WST_RIGHT,
	WST_LEFT
};


AIStateGuard::AIStateGuard(AIObject* Owner,
						   WaypointVector Waypoints) : AIDecisionMaker<AIObject>(Owner, AIST_GUARD),
													  m_Waypoints(Waypoints),
													  m_WType(((AIEnemyObject*)m_Owner)->GetWaypointType()),
													  m_WaypointIndex(0),
													  m_bReachedEnd(false),
													  m_bObstacleDetected(false)
{
}


void AIStateGuard::Activate(void)
{
	m_Status = ST_ACTIVE;

	if(m_bObstacleDetected) return;

	const bool bIsAtPosition = m_Owner->IsAtPosition(m_Waypoints[m_WaypointIndex]);

	if(!bIsAtPosition)
	{
		AddState(new AIStateMoveToPosition(m_Owner, m_Waypoints[m_WaypointIndex]));
	}
	else
	{
		UpdateCounter();

		AddState(new AIStateIdle(m_Owner, 1500));
	}
}


int AIStateGuard::Process(void)
{
	ActivateIfInactive();

	CheckForObstacles();

		m_Status = ProcessSubGoals();

	ReactivateIfFailed();

	if (m_Status == ST_COMPLETED || m_bObstacleDetected)
	{
		m_Status = ST_INACTIVE;
	}

	return m_Status;
}


void AIStateGuard::Terminate(void)
{
}


bool AIStateGuard::HandleMessage(const Message& Msg)
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


void AIStateGuard::UpdateCounter(void)
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


float AIStateGuard::GetRandomPositionInRadius(void)
{
	float RandomDist = 0.0f;

	while(Ogre::Math::Abs(RandomDist) < 1)
	{
		RandomDist = Mathematics::Rand32((int)-m_Owner->GetBoundingRadius() + 10, (int)m_Owner->GetBoundingRadius() - 10);
	}

	return RandomDist;

}


void AIStateGuard::CheckForObstacles(void)
{
	if(!m_bObstacleDetected)
	{
		BOOST_FOREACH (GameObject* current, GameMap::GetInstance().GetObstacles())
		{
			Ogre::Vector3 OwnerPosition = m_Owner->GetGameObject()->GetPosition();

			float RandomRadius = GetRandomPositionInRadius();

			Ogre::Vector3 RandomLocation = OwnerPosition + m_Owner->GetGameObject()->GetRight() * RandomRadius;
			Ogre::Ray FirstRay(RandomLocation, m_Owner->GetGameObject()->GetForward());

			std::pair<bool, float> FirstIntersection = FirstRay.intersects(current->GetWorldAABB());

			if(FirstIntersection.first && FirstIntersection.second <= m_Owner->GetBoundingRadius() * m_Owner->GetSpeed())
			{
				Ogre::Vector3 SecondDirToObstacle = current->GetPosition() - OwnerPosition;;
				Ogre::Ray SecondRay(OwnerPosition, SecondDirToObstacle.normalisedCopy());

				std::pair<bool, float> SecondIntersection = SecondRay.intersects(current->GetWorldAABB());
				if(SecondIntersection.first)
				{
					m_Owner->GetSteering()->SeekOff();
					m_Owner->GetSteering()->ArriveOff();
					m_bObstacleDetected = true;
					RemoveAllStates();

					Ogre::Vector3 FirstPoint = FirstRay.getPoint(FirstIntersection.second);
					Ogre::Vector3 SecondPoint = SecondRay.getPoint(SecondIntersection.second);

					Ogre::Vector3 direction = SecondPoint - FirstPoint;
					direction.normalise();

					Ogre::Quaternion orientation = m_Owner->GetGameObject()->GetForward().getRotationTo(direction);

					m_Owner->GetGameObject()->_GetSceneNode()->rotate(orientation);

					Ogre::Vector3 cross = SecondDirToObstacle.crossProduct(FirstPoint - OwnerPosition);

					if(cross.y >= 0)
					{
						Log::GetInstance().AddInfo("Right");
						AddState(new AIStateWallFollow(m_Owner, current, WST_LEFT));
					}
					else
					{
						Log::GetInstance().AddInfo("Left");
						AddState(new AIStateWallFollow(m_Owner, current, WST_RIGHT));
					}

					m_Status = ST_FAILED;
				}
			}
		}
	}
}

END_NAMESPACE