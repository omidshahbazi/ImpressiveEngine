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
#include "AIStateCommanderEvaluator.h"
#include "AIObjectType.h"
#include "AICommanderObject.h"
#include "AIBrain.h"


BEGIN_NAMESPACE
AIStateCommanderEvaluator::AIStateCommanderEvaluator(float Bias) : AIStateEvaluator(Bias)
{
}


float AIStateCommanderEvaluator::CalculateDesirability(AIObject* aiObject)
{
	m_Owner = (AICommanderObject*)aiObject;

	if(m_Owner->GetWaypoints().size() <= 0 || m_Owner->GetType() != AIOT_COMMANDER)
	{
		return 0.0f;
	}

	float Desirability = 1.0f;

	Desirability *= m_CharacterBias;

	return Desirability;
}


void AIStateCommanderEvaluator::SetGoal(AIObject* aiObject)
{
	aiObject->GetBrain()->AddStateCommander(m_Owner->GetWaypoints());
}

END_NAMESPACE