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
#pragma once

#include "AIState.h"

BEGIN_NAMESPACE
class AIObject;

class AIStateSeekToPosition : public AIState<AIObject>
{
public:
	AIStateSeekToPosition(AIObject* Owner, Ogre::Vector3 Target);

	~AIStateSeekToPosition(void);

	void Activate(void);

	int Process(void);

	void Terminate(void);

private:
	const bool IsStuck(void);

private:
	Ogre::Vector3 m_Position;

	unsigned int m_TimeToReachPos;

	float m_StartTime;

};


END_NAMESPACE