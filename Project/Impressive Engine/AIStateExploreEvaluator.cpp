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
#include "AIStateExploreEvaluator.h"
#include "AIObject.h"
#include "AIBrain.h"


BEGIN_NAMESPACE
AIStateExploreEvaluator::AIStateExploreEvaluator(float Bias) : AIStateEvaluator(Bias)
{
}


float AIStateExploreEvaluator::CalculateDesirability(AIObject* aiObject)
{
	if(aiObject->IsStuckOnWaypoints()) return 0.0f;

	float Desirability = 0.07f;

	Desirability *= m_CharacterBias;

	return Desirability;
}


void AIStateExploreEvaluator::SetGoal(AIObject* aiObject)
{
	aiObject->GetBrain()->AddStateExplore();
}

END_NAMESPACE