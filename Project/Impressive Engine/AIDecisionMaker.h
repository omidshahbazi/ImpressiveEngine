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

#include <list>
#include "AIState.h"

BEGIN_NAMESPACE
template <class AIObject_Type>
class AIDecisionMaker : public AIState<AIObject_Type>
{
public:

	AIDecisionMaker(AIObject_Type* Owner,
					AIStateType Type) : AIState<AIObject_Type>(Owner, Type)
	{
	}

	~AIDecisionMaker(void)
	{
		RemoveAllStates();
	}

	virtual void Activate(void) = 0;

	virtual int Process(void) = 0;

	virtual void Terminate(void) = 0;

	virtual bool HandleMessage(const Message& Msg)
	{
		return ForwardMessageToFrontMostSubGoal(Msg);
	}

	void AddState(AIState<AIObject_Type>* state);

	void RemoveAllStates(void);

protected:
	typedef std::list<AIState<AIObject_Type>*> StateList;

protected:

	StateList m_States;

	unsigned int ProcessSubGoals(void);

	bool ForwardMessageToFrontMostSubGoal(const Message &Msg);

};


template <class AIObject_Type>
void AIDecisionMaker<AIObject_Type>::RemoveAllStates(void)
{
	for(StateList::iterator it = m_States.begin(); it != m_States.end(); it++)
	{
		(*it)->Terminate();
		delete *it;
	}

	m_States.clear();
}


template <class AIObject_Type>
void AIDecisionMaker<AIObject_Type>::AddState(AIState<AIObject_Type>* state)
{
	m_States.push_front(state);
}


template <class AIObject_Type>
bool AIDecisionMaker<AIObject_Type>::ForwardMessageToFrontMostSubGoal(const Message& Msg)
{
	if(!m_States.empty())
	{
		return m_States.front()->HandleMessage(Msg);
	}

	return false;
}


template <class AIObject_Type>
unsigned int AIDecisionMaker<AIObject_Type>::ProcessSubGoals(void)
{
	while(!m_States.empty() && (m_States.front()->IsCompleted() || m_States.front()->IsFailed()))
	{
		m_States.front()->Terminate();

		delete m_States.front();

		m_States.pop_front();
	}

	if(!m_States.empty())
	{
		unsigned int StatusOfSubGoal = m_States.front()->Process();

		if(StatusOfSubGoal == ST_COMPLETED && m_States.size() > 1)
		{
			return ST_ACTIVE;
		}

		return StatusOfSubGoal;
	}
	else
	{
		return ST_COMPLETED;
	}
}
END_NAMESPACE