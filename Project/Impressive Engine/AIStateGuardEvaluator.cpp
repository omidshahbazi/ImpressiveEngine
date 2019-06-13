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
#include "AIStateGuardEvaluator.h"
#include "AIObject.h"
#include "AIBrain.h"
#include "AIObjectType.h"
#include "AIEnemyObject.h"


BEGIN_NAMESPACE
AIStateGuardEvaluator::AIStateGuardEvaluator(float Bias) : AIStateEvaluator(Bias)
{
}


float AIStateGuardEvaluator::CalculateDesirability(AIObject* aiObject)
{
	if(((AIEnemyObject*)aiObject)->GetWaypoints().size() <=0 || !aiObject->IsStuckOnWaypoints())
	{
		return 0;
	}

	float Desirability = 0.08f;

	Desirability *= m_CharacterBias;

	return Desirability;
}


void AIStateGuardEvaluator::SetGoal(AIObject* aiObject)
{
	aiObject->GetBrain()->AddStateGuard(((AIEnemyObject*)aiObject)->GetWaypoints());
}

END_NAMESPACE