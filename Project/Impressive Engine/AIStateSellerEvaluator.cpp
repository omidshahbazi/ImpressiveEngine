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
#include "AIStateSellerEvaluator.h"
#include "AIObject.h"
#include "AIObjectType.h"
#include "AIBrain.h"


BEGIN_NAMESPACE
AIStateSellerEvaluator::AIStateSellerEvaluator(float Bias) : AIStateEvaluator(Bias)
{
}


AIStateSellerEvaluator::~AIStateSellerEvaluator(void)
{
}


float AIStateSellerEvaluator::CalculateDesirability(AIObject* aiObject)
{
	if(aiObject->GetType() != AIOT_SELLER)
	{
		return 0.0f;
	}
	else
	{
		return 1.0f;
	}
}


void AIStateSellerEvaluator::SetGoal(AIObject* aiObject)
{
	aiObject->GetBrain()->AddStateSeller();
}

END_NAMESPACE