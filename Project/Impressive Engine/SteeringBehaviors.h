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

#include <map>
#include <vector>
#include <list>
#include "StdAfxOGRE.h"
#include "ScriptRegister.h"


BEGIN_NAMESPACE
const float WANDER_RADIUS =					80.0f;
const float WANDER_DISTANCE =				2.0f;
const float WANDER_JITTER =					20.0f;
const float WAYPOINT_SEEK_DISTANCE =		20.0f;

class AIObject;
class GameObject;

enum SteeringBehaviorType;

enum DecelerationType
{
	DT_SLOW,
	DT_NORMAL,
	DT_FAST
};

class SteeringBehaviors
{
public:
	SteeringBehaviors(AIObject* Owner);

	~SteeringBehaviors(void);

	Ogre::Vector3 Calculate(void);

	void SetTarget(const Ogre::Vector3 &Target);

	const Ogre::Vector3 GetTarget(void) const;

	void SetFriend(AIObject* AIObject);

	void SetOffset(const Ogre::Vector3 Offset);

	void SeekOn(void);
	void FleeOn(void);
	void ArriveOn(void);
	void SeparationOn(void);
	void AlignmentOn(void);
	void CohesionOn(void);
	void WallAvoidanceOn(void);
	void WanderOn(void);
	void OffsetPursuitOn(void);
	void ObstacleAvoidanceOn(void);

	void SeekOff(void);
	void FleeOff(void);
	void ArriveOff(void);
	void SeparationOff(void);
	void AlignmentOff(void);
	void CohesionOff(void);
	void WallAvoidanceOff(void);
	void WanderOff(void);
	void OffsetPursuitOff(void);
	void ObstacleAvoidanceOff(void);

	const bool IsSeekOn(void) const;
	const bool IsFleeOn(void) const;
	const bool IsArriveOn(void) const;
	const bool IsSeparationOn(void) const;
	const bool IsAlignmentOn(void) const;
	const bool IsCohesionOn(void) const;
	const bool IsWallAvoidanceOn(void) const;
	const bool IsWanderOn(void) const;
	const bool IsOffsetPursuitOn(void) const;
	const bool IsObstacleAvoidanceOn(void) const;

	const Ogre::Vector3& GetForce(void) const;

	DECLARE_SCRIPT_REGISTATION()

private:

	bool AccumulateForce(Ogre::Vector3 &SteeringForce, const Ogre::Vector3& ForceToAdd);

	const bool On(SteeringBehaviorType bt) const;

	Ogre::Vector3 Seek(const Ogre::Vector3 &Target);
	Ogre::Vector3 Flee(const Ogre::Vector3 &Target);
	Ogre::Vector3 Arrive(const Ogre::Vector3 &Target, const DecelerationType Deceleration);
	Ogre::Vector3 Separation(const std::map<const unsigned int, AIObject*>& AIObjects);
	Ogre::Vector3 Alignment(const std::map<const unsigned int, AIObject*>& AIObjects);
	Ogre::Vector3 Cohesion(const std::map<const unsigned int, AIObject*>& AIObjects);
	Ogre::Vector3 WallAvoidance(const std::list<GameObject*>& Obstacles);
	Ogre::Vector3 Wander();
	Ogre::Vector3 OffsetPursuit(AIObject* Follower, const Ogre::Vector3& Offset);
	Ogre::Vector3 ObstacleAvoidance(const std::list<GameObject*>& Obstacles);

	Ogre::Vector3 CalculatePrioritized(void);

	void CreateFeelers(void);

private:
	AIObject* m_Owner;

	AIObject* m_Friend;

	Ogre::Vector3	m_SteeringForce;

	Ogre::Vector3 m_Target;

	std::vector<Ogre::Vector3> m_Feelers;

	unsigned int m_Flags;

	float m_WeightSeek;
	float m_WeightFlee;
	float m_WeightArrive;
	float m_WeightSeparation;
	float m_WeightAlignment;
	float m_WeightCohesion;
	float m_WeightWallAvoidance;
	float m_WeightWander;
	float m_WeightOffsetPursuit;
	float m_WeightObstacleAvoidance;

	DecelerationType m_Deceleration;
	const float m_DecelerationRate;

	float m_DistanceToOthers;

	float m_WallDetectionFeelerLength;

	float m_ObstacleDetectionLength;

	Ogre::Vector3 m_WanderTarget;
	float m_WanderJitter;
	float m_WanderRadius;
	float m_WanderDistance;

	Ogre::Vector3 m_Offset;

	bool m_bIsStucked;

};


END_NAMESPACE