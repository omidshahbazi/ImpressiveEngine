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
#include "AIBrain.h"
#include "AISellerObject.h"
#include "AINPCObject.h"
#include "AIEnemyObject.h"
#include "AIStateTypes.h"
#include "AIStateIdle.h"
#include "AIStateGuard.h"
#include "AIStateExplore.h"
#include "AIStateCommander.h"
#include "AIStateSeller.h"
#include "AIStateShopping.h"
#include "AIStateMoveToPosition.h"
#include "AIStateIdleEvaluator.h"


BEGIN_NAMESPACE
AIBrain::AIBrain(AIObject* Owner) : AIDecisionMaker<AIObject>(Owner, AIST_BRAIN)
{
	m_StateEvaluators.push_back(new AIStateIdleEvaluator(1));
}


AIBrain::~AIBrain(void)
{
	for(StateEvaluators::iterator it = m_StateEvaluators.begin(); it != m_StateEvaluators.end(); it++)
	{
		delete *it;
	}
}


void AIBrain::Arbitrate(void)
{
	float Best = 0.0f;
	AIStateEvaluator* MostDesirable = 0;

	for(StateEvaluators::iterator it = m_StateEvaluators.begin(); it != m_StateEvaluators.end(); it++)
	{
		const float Desirabilty = (*it)->CalculateDesirability(m_Owner);

		if(Desirabilty >= Best)
		{
			Best = Desirabilty;
			MostDesirable = *it;
		}
	}

	if(MostDesirable)
	{
		MostDesirable->SetGoal(m_Owner);
	}
}


int AIBrain::Process(void)
{
	ActivateIfInactive();

	int SubGoalStatus = ProcessSubGoals();

	if(SubGoalStatus == ST_COMPLETED || SubGoalStatus == ST_FAILED)
	{
		if(!m_Owner->IsPossessed())
		{
			return ST_INACTIVE;
		}
	}

	return SubGoalStatus;
}

void AIBrain::Activate(void)
{
	if(!m_Owner->IsPossessed())
	{
		Arbitrate();
	}

	m_Status = ST_ACTIVE;
}

void AIBrain::Terminate(void)
{
}


const bool AIBrain::IsInState(AIStateType Type)
{
	if(!m_States.empty())
	{
		return m_States.front()->GetType() == Type;
	}

	return false;
}


void AIBrain::AddStateEvaluator(AIStateEvaluator* Evaluator)
{
	m_StateEvaluators.push_back(Evaluator);
}


void AIBrain::AddStateIdle(void)
{
	if(!IsInState(AIST_IDLE))
	{
		RemoveAllStates();
		AddState(new AIStateIdle(m_Owner, 10000));
	}
}


void AIBrain::AddStateExplore(void)
{
	if(!IsInState(AIST_EXPLORE))
	{
		RemoveAllStates();
		AddState(new AIStateExplore(m_Owner));
	}
}

void AIBrain::AddStateGuard(std::vector<const Ogre::Vector3> Waypoints)
{
	if(!IsInState(AIST_GUARD))
	{
		RemoveAllStates();
		AddState(new AIStateGuard(m_Owner, Waypoints));
	}
}


void AIBrain::AddStateCommander(std::vector<const Ogre::Vector3> Waypoints)
{
	if(!IsInState(AIST_COMMANDER))
	{
		RemoveAllStates();
		AddState(new AIStateCommander(m_Owner, Waypoints));
	}
}


void AIBrain::AddStateSeller(void)
{
	if(!IsInState(AIST_SELLER))
	{
		RemoveAllStates();
		AddState(new AIStateSeller(m_Owner));
	}
}


void AIBrain::AddStateShopping(void)
{
	if(!IsInState(AIST_SHOPPING))
	{
		RemoveAllStates();
		AddState(new AIStateShopping(m_Owner, ((AINPCObject*)m_Owner)->GetSeller(), ((AINPCObject*)m_Owner)->GetOffsetToSeller()));
	}
}


void AIBrain::AddStateMoveToPosition(Ogre::Vector3 Pos)
{
	if(!IsInState(AIST_MOVETOPOSITION))
	{
		RemoveAllStates();
		AddState(new AIStateMoveToPosition(m_Owner, Pos));
	}
}

END_NAMESPACE