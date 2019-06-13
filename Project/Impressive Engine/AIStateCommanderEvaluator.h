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

#include "AIStateEvaluator.h"

BEGIN_NAMESPACE
class AICommanderObject;

class AIStateCommanderEvaluator : public AIStateEvaluator
{
public:
	AIStateCommanderEvaluator(float Bias);

	float CalculateDesirability(AIObject* aiObject);

	void SetGoal(AIObject* aiObject);

private:
	AICommanderObject* m_Owner;

};


END_NAMESPACE