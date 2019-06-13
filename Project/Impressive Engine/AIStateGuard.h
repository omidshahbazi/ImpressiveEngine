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
#include "AIEnemyObject.h"

BEGIN_NAMESPACE
class AIObject;
class PathEdge;

enum WaypointType;

class AIStateGuard : public AIDecisionMaker<AIObject>
{
public:
	typedef std::vector<const Ogre::Vector3> WaypointVector;

public:
	AIStateGuard(AIObject* Owner, WaypointVector Waypoints);

	void Activate(void);

	int Process(void);

	void Terminate(void);

	bool HandleMessage(const Message& Msg);

private:
	float GetRandomPositionInRadius(void);

	void CheckForObstacles(void);

private:
	WaypointVector m_Waypoints;

	unsigned int m_PathEdgesSize;

	unsigned int m_WaypointIndex;
	bool m_bReachedEnd;
	bool m_bObstacleDetected;

	WaypointType m_WType;

	void UpdateCounter(void);

};


END_NAMESPACE