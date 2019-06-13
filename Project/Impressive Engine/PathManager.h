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
#include "Singleton.h"

BEGIN_NAMESPACE
class PathPlanner;

class PathManager : public Singleton<PathManager>
{
public:
	PathManager(void);

	~PathManager(void);

	void Drop(void);

	void UpdateSearches(void);

	void Register(PathPlanner* pPathPlanner);

	void UnRegister(PathPlanner* pPathPlanner);

	const bool Contains(PathPlanner* pPathPlanner);

	const unsigned int ActiveSearchSize() const;

private:
	std::list<PathPlanner*> m_SearchRequests;

	unsigned int m_SearchCyclesPerUpdate;

};


END_NAMESPACE