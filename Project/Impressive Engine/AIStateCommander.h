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
#include "AIDecisionMaker.h"
#include "AICommanderObject.h"

BEGIN_NAMESPACE
class AIObject;
class PathEdge;

enum WaypointType;

class AIStateCommander : public AIDecisionMaker<AIObject>
{
public:
	typedef std::vector<const Ogre::Vector3> WaypointVector;

public:
	AIStateCommander(AIObject* Owner, WaypointVector Waypoints);

	void Activate(void);

	int Process(void);

	void Terminate(void);

	bool HandleMessage(const Message& Msg);

private:
	void UpdateCounter(void);

private:
	WaypointVector m_Waypoints;

	typedef std::list<PathEdge*> PathEdgeList;
	typedef std::vector<PathEdgeList> PathEdgeListVector;

	PathEdgeListVector m_PathEdges;
	PathEdgeListVector m_PathEdgesReversed;

	unsigned int m_PathEdgesSize;

	unsigned int m_WaypointIndex;
	bool m_bReachedEnd;

	WaypointType m_WType;

	bool m_bIsIdle;

};


END_NAMESPACE