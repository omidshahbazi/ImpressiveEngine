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
#include "AIStateWallFollow.h"
#include "AIStateSeekToPosition.h"
#include "AIObject.h"
#include "GameObject.h"


BEGIN_NAMESPACE
enum WallSideType
{
	WST_RIGHT,
	WST_LEFT
};


AIStateWallFollow::AIStateWallFollow(AIObject* Owner,
									 GameObject* Wall,
									 WallSideType WSType) : AIDecisionMaker<AIObject>(Owner, AIST_WALLFOLLOW),
															m_Wall(Wall),
															m_WallSide(WSType),
															m_Destination(Ogre::Vector3::ZERO),
															m_bRayCasted(false)
{

}


void AIStateWallFollow::Activate(void)
{
	m_Status = ST_ACTIVE;

	RemoveAllStates();

	Ogre::Vector3 dir = Ogre::Vector3::ZERO;

	Ogre::Vector3 right = m_Owner->GetGameObject()->GetRight();

	if(m_WallSide == WST_LEFT)
	{
		dir = m_Owner->GetGameObject()->GetRight() * (m_Owner->GetBoundingRadius() + 10);
	}
	else
	{
		dir = m_Owner->GetGameObject()->GetRight() * (m_Owner->GetBoundingRadius() + 10);
	}

	Ogre::Ray Ray(m_Owner->GetGameObject()->GetPosition(), dir);

	std::pair<bool, float> intersection = Ray.intersects(m_Wall->GetWorldAABB());

	m_bRayCasted = intersection.first;

	if(m_bRayCasted)
	{
		AddState(new AIStateSeekToPosition(m_Owner, m_Owner->GetGameObject()->GetForward() * m_Owner->GetSpeed()));
	}
}


int AIStateWallFollow::Process(void)
{
	ActivateIfInactive();

	m_Status = ProcessSubGoals();

	if(m_bRayCasted)
	{
		m_Status = ST_INACTIVE;
	}

	ReactivateIfFailed();

	return m_Status;
}


void AIStateWallFollow::Terminate(void)
{
}


bool AIStateWallFollow::HandleMessage(const Message& Msg)
{
	bool bHandled = ForwardMessageToFrontMostSubGoal(Msg);
	return bHandled;
}

END_NAMESPACE