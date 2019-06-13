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
#include "StdAfxOGRE.h"
#include "GraphSearchAstar.h"

BEGIN_NAMESPACE
class AIObject;
class GraphNode;
class GraphEdge;
class PathEdge;
class SparseGraph;
class MessageDispatcher;

class PathPlanner
{
public:
	typedef std::list<PathEdge*>		Path;

public:
	PathPlanner(AIObject* Owner);

	~PathPlanner(void);

	Path GetPath(void);

	Path GetPathReversed(void);

	int CycleOnce(void);

	bool RequestPathTo(const Ogre::Vector3 TargetPos);

	double GetCostToNode(const unsigned int NodeIdx) const;

	Ogre::Vector3 &GetDestination(void) const;

	void SetDestination(Ogre::Vector3 Destination);

	Ogre::Vector3 &GetNodePosition(int NodeIdx) const;

private:
	const unsigned int GetClosestNodeToPosition(const Ogre::Vector3 &Posaition) const;

	void SmoothPathEdgesQuick(Path& path);

	void SmoothPathEdgesPercise(Path& path);

	void GetReadyForNewSearch(void);

	void CalculatePath(void);

private:
	enum {no_closest_node_found};

	typedef GraphNode					Node;
	typedef GraphEdge					Edge;
	typedef SparseGraph				Graph;
	typedef MessageDispatcher			Dispatcher;

	Path m_Path;

	AIObject* m_Owner;

	GraphSearchAstar* m_CurrentSearch;

	Ogre::Vector3 m_DestinationPos;

	bool m_bSearchRequestSent;

};
END_NAMESPACE