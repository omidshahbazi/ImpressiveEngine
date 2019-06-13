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
class AITimer;

class AIStateIdle : public AIState<AIObject>
{
public:
	AIStateIdle(AIObject* Owner, unsigned int Duration);

	void Activate(void);

	int Process(void);

	void Terminate(void);

private:
	unsigned int m_StandbyDuration;

	AITimer* m_Timer;

};

END_NAMESPACE
