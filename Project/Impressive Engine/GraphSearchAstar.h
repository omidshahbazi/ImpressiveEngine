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

#include <vector>
#include <list>

#include "StdAfxOGRE.h"
#include "GraphSearchTimeSliced.h"
#include "PriorityQueue.h"

BEGIN_NAMESPACE
class GraphNode;
class GraphEdge;
class SparseGraph;
class PathEdge;

class GraphSearchAstar : public GraphSearchTimeSliced
{
public:
	GraphSearchAstar(const unsigned int SourceNodeIndex, const unsigned int TargetSourceIndex);

	~GraphSearchAstar(void);

	float Calculate(const unsigned int NodeIndex1, const unsigned int NodeIndex2);

	float GetCostToTarget(void) const;

	int CycleOnce();

	std::list<int> GetPathToTarget(void) const;

	std::list<PathEdge*> GetPathAsPathEdge(void) const;

private:
	typedef GraphNode Node;
	typedef GraphEdge Edge;
	typedef std::list<const Node*> Path;

	std::vector<float> m_GCost;
	std::vector<float> m_FCost;

	std::vector<const Edge*> m_ShortestPath;
	std::vector<const Edge*> m_Frontier;

	const unsigned int m_Source;
	const unsigned int m_Target;

	IndexedPriorityQLow<float>* m_PQ;

	bool m_bFound;

};


END_NAMESPACE