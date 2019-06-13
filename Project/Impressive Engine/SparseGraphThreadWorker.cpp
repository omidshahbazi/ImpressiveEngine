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
#include "SparseGraphThreadWorker.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"


BEGIN_NAMESPACE
SparseGraphThreadWorker::SparseGraphThreadWorker(FunctionType FType) : m_FunctionType(FType),
																	   m_NavGraphFileName("")
{
}


void SparseGraphThreadWorker::Do(void)
{
	if(m_FunctionType == FT_LOADGRAPH)
	{
		GameMap::GetInstance().GetNavGraph()->LoadGraph(GetNavGraphFileName());
	}
	else if (m_FunctionType == FT_CLEARGRAPH)
	{
		GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->Clear();
	}
}


void SparseGraphThreadWorker::SetNavGraphFileName(const Ogre::String& FileName)
{
	m_NavGraphFileName = FileName;
}


const Ogre::String& SparseGraphThreadWorker::GetNavGraphFileName(void) const
{
	return m_NavGraphFileName;
}
END_NAMESPACE