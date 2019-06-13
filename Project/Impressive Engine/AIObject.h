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

#include "MovingObject.h"

BEGIN_NAMESPACE
struct Message;

class AIBrain;
class PathPlanner;
class SteeringBehaviors;
class AITimer;

enum WaypointType;

class AIObject : public MovingObject
{
public:
	AIObject(const unsigned int ID, GameObject* gameObject);

	~AIObject(void);

	virtual void Drop(void);

	void Update();

	void UpdateMovement();

	bool HandleMessage(const Message& msg);

	const bool CanWalkTo(Ogre::Vector3 Pos);

	const bool CanWalkBetween(Ogre::Vector3 PosA, Ogre::Vector3 PosB);

	AIBrain* GetBrain(void);

	PathPlanner* GetPathPlanner(void);

	SteeringBehaviors* GetSteering(void);

	unsigned int CalculateTimeToReachPosition(const Ogre::Vector3 &Position);

	const bool IsAtPosition(const Ogre::Vector3 &Position) const;

	void LookAt(const Ogre::Vector3& Target);

	Ogre::Quaternion LookRotation(const Ogre::Vector3& forward);

	void RotateSmoothTo(const Ogre::Vector3& Target, const float& SmoothTime);

	const bool IsFacing(const Ogre::Vector3& Target);

	void AddWaypoint(const Ogre::Vector3 Waypoint);
	void AddWaypoint(const int X, const int Y, const int Z);

	void GoToFirstWaypoint(void);

	const std::vector<const Ogre::Vector3> &GetWaypoints(void) const;

	void SetWaypointType(const WaypointType WType);

	const WaypointType& GetWaypointType(void) const;

	void StickOnWaypoints(void);

	const bool IsStuckOnWaypoints(void) const;

	DECLARE_SCRIPT_REGISTATION()

protected:
	AIBrain* m_Brain;

	PathPlanner* m_PathPlanner;

	SteeringBehaviors* m_Steering;

	AITimer* m_ArbitrationTimer;

	WaypointType m_WType;

	std::vector<const Ogre::Vector3> m_Waypoints;

	bool m_bStickOnWaypoints;
};


END_NAMESPACE