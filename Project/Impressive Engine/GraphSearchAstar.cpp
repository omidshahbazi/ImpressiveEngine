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
#include "GraphSearchAstar.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"
#include "PathEdge.h"
#include "Log.h"

BEGIN_NAMESPACE
GraphSearchAstar::GraphSearchAstar(const unsigned int SourceNodeIndex, const unsigned int TargetSourceIndex) :
									m_Source(SourceNodeIndex), m_Target(TargetSourceIndex),
									m_ShortestPath(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNodeSize()),
									m_Frontier(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNodeSize()),
									m_GCost(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNodeSize(), 0.0), 
									m_FCost(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNodeSize(), 0.0)
{
	m_PQ = new IndexedPriorityQLow<float>(m_FCost, GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNodeSize());

	m_PQ->insert(m_Source);
}


GraphSearchAstar::~GraphSearchAstar(void)
{
	delete m_PQ;
}


inline float GraphSearchAstar::Calculate(const unsigned int NodeIndex1, const unsigned int NodeIndex2)
{
	return GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(NodeIndex1).GetPosition().squaredDistance(
													GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(NodeIndex2).GetPosition());
}


float GraphSearchAstar::GetCostToTarget(void) const
{
	return m_GCost[m_Target];
}


int GraphSearchAstar::CycleOnce()
{
	if (m_PQ->empty())
	{
		return TARGET_NOT_FOUND;
	}

	unsigned int NextClosestNode = m_PQ->Pop();

	m_ShortestPath[NextClosestNode] = m_Frontier[NextClosestNode];

	if (NextClosestNode == m_Target)
	{
		return TARGET_FOUND;
	}

	SparseGraph::EdgeIterator EdgeItr(GameMap::GetInstance().GetNavGraph()->GetSparseGraph(), NextClosestNode);

	for (const Edge* pE = EdgeItr.begin(); !EdgeItr.end(); pE = EdgeItr.next())
	{
		float HCost = Calculate(m_Target, pE->GetEndPoint());

		float GCost = m_GCost[NextClosestNode] + pE->GetDistance();

		if (m_Frontier[pE->GetEndPoint()] == NULL)
		{
			m_FCost[pE->GetEndPoint()] = GCost + HCost;
			m_GCost[pE->GetEndPoint()] = GCost;

			m_PQ->insert(pE->GetEndPoint());

			m_Frontier[pE->GetEndPoint()] = pE;
		}
		else if ((GCost < m_GCost[pE->GetEndPoint()]) && (m_ShortestPath[pE->GetEndPoint()] == NULL))
		{
			m_FCost[pE->GetEndPoint()] = GCost + HCost;
			m_GCost[pE->GetEndPoint()] = GCost;

			m_PQ->ChangePriority(pE->GetEndPoint());

			m_Frontier[pE->GetEndPoint()] = pE;
		}
	}

	return SEARCH_INCOMPELETE;
}


std::list<int> GraphSearchAstar::GetPathToTarget(void) const
{
	std::list<int> path;

	if (m_Target < 0)  return path;    

	int nd = m_Target;

	path.push_front(nd);
    
	while ((nd != m_Source) && (m_ShortestPath[nd] != 0))
	{
		nd = m_ShortestPath[nd]->GetStartPoint();

		path.push_front(nd);
	}

	return path;
}


std::list<PathEdge*> GraphSearchAstar::GetPathAsPathEdge(void) const
{
	std::list<PathEdge*> path;

	if (m_Target < 0)  return path;    

	int nd = m_Target;
    
	while ((nd != m_Source) && (m_ShortestPath[nd] != 0))
	{
		path.push_front(new PathEdge(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(m_ShortestPath[nd]->GetStartPoint()).GetPosition(),
									GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNode(m_ShortestPath[nd]->GetEndPoint()).GetPosition(),
									m_ShortestPath[nd]->GetFlags()));

		nd = m_ShortestPath[nd]->GetStartPoint();
	}

	return path;
}


END_NAMESPACE