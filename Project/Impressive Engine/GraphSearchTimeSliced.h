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
#include <list>

BEGIN_NAMESPACE
class PathEdge;

enum {TARGET_FOUND, TARGET_NOT_FOUND, SEARCH_INCOMPELETE};

class GraphSearchTimeSliced
{
public:
	//GraphSearchTimeSliced(void);

	//~GraphSearchTimeSliced(void);

	virtual int CycleOnce() = 0;

	virtual float GetCostToTarget(void) const = 0;

	virtual std::list<int> GetPathToTarget(void) const = 0;

	virtual std::list<PathEdge*> GetPathAsPathEdge(void) const = 0;
};
END_NAMESPACE