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

#include <vector>
#include "StdAfxOGRE.h"
#include "AIDecisionMaker.h"

BEGIN_NAMESPACE
class AIObject;
class AIStateEvaluator;

class AIBrain : public AIDecisionMaker<AIObject>
{
public:

	AIBrain(AIObject* Owner);

	~AIBrain(void);

	void Arbitrate(void);

	const bool IsNotPresent(const unsigned GoalType) const;

	int Process(void);

	void Activate(void);

	void Terminate(void);

	const bool IsInState(AIStateType Type);

	void AddStateEvaluator(AIStateEvaluator* Evaluator);

	void AddStateIdle(void);

	void AddStateExplore(void);

	void AddStateGuard(std::vector<const Ogre::Vector3> Waypoints);

	void AddStateSeller(void);

	void AddStateShopping(void);

	void AddStateCommander(std::vector<const Ogre::Vector3> Waypoints);

	void AddStateMoveToPosition(Ogre::Vector3 Pos);

private:

	typedef std::vector<AIStateEvaluator*> StateEvaluators;

	StateEvaluators m_StateEvaluators;
};


END_NAMESPACE