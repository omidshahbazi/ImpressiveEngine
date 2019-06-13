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
#include "AIStateSeekToPosition.h"
#include "AIObject.h"
#include "SteeringBehaviors.h"
#include "Time.h"
#include "Log.h"


BEGIN_NAMESPACE
AIStateSeekToPosition::AIStateSeekToPosition(AIObject* Owner,
											 Ogre::Vector3 Target) : AIState(Owner, AIST_SEETTOPOSITION),
																	 m_Position(Target),
																	 m_TimeToReachPos(0)
{
}


AIStateSeekToPosition::~AIStateSeekToPosition(void)
{
}


void AIStateSeekToPosition::Activate(void)
{
	m_Status = ST_ACTIVE;

	m_StartTime = Time::GetInstance().GetRealTime();

	m_TimeToReachPos = m_Owner->CalculateTimeToReachPosition(m_Position);

	const unsigned int MarginOfError = 200;

	m_TimeToReachPos += MarginOfError;

	m_Owner->GetSteering()->SetTarget(m_Position);

	m_Owner->GetSteering()->SeekOn();
}


int AIStateSeekToPosition::Process(void)
{
	ActivateIfInactive();

	if(IsStuck())
	{
		m_Status = ST_FAILED;
	}
	else
	{
		if(m_Owner->IsAtPosition(m_Position))
		{
			m_Status = ST_COMPLETED;
		}
	}

	return m_Status;;
}


void AIStateSeekToPosition::Terminate(void)
{
	m_Owner->GetSteering()->SeekOff();

	m_Status = ST_COMPLETED;
}


const bool AIStateSeekToPosition::IsStuck(void)
{
	unsigned int TimeTaken = Time::GetInstance().GetRealTime() - m_StartTime;

	if(TimeTaken > m_TimeToReachPos)
	{
		Log::GetInstance().AddInfo("AI Object with ID " + Ogre::StringConverter::toString(m_Owner->GetID()) + " has stucked.");

		return true;
	}

	return false;
}

END_NAMESPACE