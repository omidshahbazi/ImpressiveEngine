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
#include "AIDecisionMaker.h"

BEGIN_NAMESPACE
class AIObject;
class PathEdge;

class AIStatePathFollow : public AIDecisionMaker<AIObject>
{
public:
	AIStatePathFollow(AIObject* Owner, std::list<PathEdge*> Path);

	~AIStatePathFollow(void);

	void Activate(void);

	int Process(void);

	void Terminate(void);

private:
	std::list<PathEdge*> m_Path;

};


END_NAMESPACE