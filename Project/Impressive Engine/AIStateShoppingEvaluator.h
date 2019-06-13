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
#include "Common.h"

BEGIN_NAMESPACE
class AIStateShoppingEvaluator : public AIStateEvaluator
{
public:
	AIStateShoppingEvaluator(float Bias);

	~AIStateShoppingEvaluator(void);

	float CalculateDesirability(AIObject* aiObject);

	void SetGoal(AIObject* aiObject);
};


END_NAMESPACE