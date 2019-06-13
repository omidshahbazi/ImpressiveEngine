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
#include "PathManager.h"
#include "PathPlanner.h"
#include "AIEngine.h"
#include "Log.h"


BEGIN_NAMESPACE
PathManager* Singleton<PathManager>::m_Instance = NULL;


PathManager::PathManager(void) : m_SearchCyclesPerUpdate(1000),
								 m_SearchRequests(NULL)
{
}


PathManager::~PathManager(void)
{
}


void PathManager::Drop(void)
{
	std::list<PathPlanner*>::iterator curPath = m_SearchRequests.begin();

	while(curPath != m_SearchRequests.end())
	{
		m_SearchRequests.remove(*curPath);
	}

	m_SearchRequests.clear();
}


void PathManager::UpdateSearches(void)
{
	int CyclesRemaning = m_SearchCyclesPerUpdate;

	std::list<PathPlanner*>::iterator curPath = m_SearchRequests.begin();

	while(CyclesRemaning-- && !m_SearchRequests.empty())
	{
		int result = (*curPath)->CycleOnce();

		if(result == TARGET_FOUND || result == TARGET_NOT_FOUND)
		{
			if(m_SearchRequests.empty())
			{
				int i = 0;
			}
			curPath = m_SearchRequests.erase(curPath);
		}
		else
		{
			if(m_SearchRequests.empty())
			{
				int i = 0;
			}
			++curPath;
		}

		if(curPath == m_SearchRequests.end())
		{
			curPath = m_SearchRequests.begin();
		}
	}
}


void PathManager::Register(PathPlanner* pPathPlanner)
{
	if(std::find(m_SearchRequests.begin(), m_SearchRequests.end(), pPathPlanner) == m_SearchRequests.end())
	{
		m_SearchRequests.push_back(pPathPlanner);
	}
}


void PathManager::UnRegister(PathPlanner* pPathPlanner)
{
	if(std::find(m_SearchRequests.begin(), m_SearchRequests.end(), pPathPlanner) == m_SearchRequests.end())
	{
		int i = 0;
	}
	m_SearchRequests.remove(pPathPlanner);
}


const bool PathManager::Contains(PathPlanner* pPathPlanner)
{
	if(!(std::find(m_SearchRequests.begin(), m_SearchRequests.end(), pPathPlanner) == m_SearchRequests.end()))
	{
		return true;
	}

	return false;
}


const unsigned int PathManager::ActiveSearchSize() const
{
	return m_SearchRequests.size();
}

END_NAMESPACE