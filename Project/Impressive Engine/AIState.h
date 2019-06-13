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

#include "StdAfxOGRE.h"
#include "Message.h"
#include "AIStateTypes.h"

BEGIN_NAMESPACE
template <class AIObject_Type>
class AIState
{
public:

	enum StatusTypes
	{
		ST_ACTIVE,
		ST_INACTIVE,
		ST_COMPLETED,
		ST_FAILED
	};

public:

	AIState(AIObject_Type* Owner, AIStateType Type) : m_Owner(Owner),
													  m_Type(Type),
													  m_Status(ST_INACTIVE)
	{
	}

	~AIState(void) {}

	virtual void Activate() = 0;

	virtual int Process() = 0;

	virtual void Terminate() = 0;

	virtual bool HandleMessage(const Message &Msg)
	{
		return false;
	}

	virtual void AddState(AIState<AIObject_Type>* state) {};

	const bool IsCompleted(void) const
	{
		return m_Status == ST_COMPLETED;
	}

	const bool IsActive(void) const
	{
		return m_Status == ST_ACTIVE;
	}

	const bool IsInactive(void) const
	{
		return m_Status == ST_INACTIVE;
	}

	const bool IsFailed(void) const
	{
		return m_Status == ST_FAILED;
	}

	const AIStateType GetType() const
	{
		return m_Type;
	}

	const AIStateType GetParentType() const
	{
		return m_ParentType;
	}

protected:

	AIStateType m_Type;

	AIObject_Type* m_Owner;

	unsigned int m_Status;

	void ActivateIfInactive(void);

	void ReactivateIfFailed(void);
};


template <class AIObject_Type>
void AIState<AIObject_Type>::ActivateIfInactive(void)
{
	if(m_Status == ST_INACTIVE)
	{
		Activate();
	}
}


template <class AIObject_Type>
void AIState<AIObject_Type>::ReactivateIfFailed(void)
{
	if(m_Status == ST_FAILED)
	{
		m_Status = ST_INACTIVE;
	}
}

END_NAMESPACE