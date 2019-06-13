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
#include "AIStateIdleEvaluator.h"
#include "AIObject.h"
#include "AIBrain.h"


BEGIN_NAMESPACE
AIStateIdleEvaluator::AIStateIdleEvaluator(float Bias) : AIStateEvaluator(Bias)
{
}


float AIStateIdleEvaluator::CalculateDesirability(AIObject* aiObject)
{
	float Desirability = 0.05f;

	Desirability *= m_CharacterBias;

	return Desirability;
}


void AIStateIdleEvaluator::SetGoal(AIObject* aiObject)
{
	aiObject->GetBrain()->AddStateIdle();
}

END_NAMESPACE