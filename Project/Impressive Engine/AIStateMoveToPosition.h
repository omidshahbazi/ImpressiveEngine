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

#include "AIDecisionMaker.h"

BEGIN_NAMESPACE
class AIObject;

class AIStateMoveToPosition : public AIDecisionMaker<AIObject>
{
public:
	AIStateMoveToPosition(AIObject* Owner, Ogre::Vector3 Pos);

	~AIStateMoveToPosition(void);

	void Activate(void);

	int Process(void);

	void Terminate(void);

	bool HandleMessage(const Message& Msg);

private:
	Ogre::Vector3 m_Destination;

};


END_NAMESPACE