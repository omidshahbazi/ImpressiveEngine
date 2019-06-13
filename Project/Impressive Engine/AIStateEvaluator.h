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

#include "Common.h"

BEGIN_NAMESPACE
class AIObject;

class AIStateEvaluator
{
public:
	AIStateEvaluator(float CharacterBias) : m_CharacterBias(CharacterBias) {}

	virtual ~AIStateEvaluator(void) {}

	virtual float CalculateDesirability(AIObject* aiObject) = 0;

	virtual void SetGoal(AIObject* aiObject) = 0;

protected:
	float m_CharacterBias;

};


END_NAMESPACE