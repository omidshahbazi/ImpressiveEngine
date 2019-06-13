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

#include "AIState.h"

BEGIN_NAMESPACE
class AIObject;
class PathEdge;

class AIStateTraverseEdge : public AIState<AIObject>
{
public:
	AIStateTraverseEdge(AIObject* Owner, PathEdge* Edge, const bool IsLastEdge);

	~AIStateTraverseEdge(void);

	void Activate(void);

	int Process(void);

	void Terminate(void);

private:
	bool IsStuck(void);

private:
	PathEdge* m_Edge;

	bool m_bLastEdgeInPath;

	unsigned int m_TimeExpectedtoReachDestination;

	unsigned int m_TimeStartToTraverse;

};


END_NAMESPACE