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
#include "AIObject.h"
#include "MovingObject.h"
#include "AIBrain.h"
#include "GameObject.h"
#include "AIObjectType.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "PathPlanner.h"
#include "PathManager.h"
#include "SteeringBehaviors.h"
#include "Message.h"
#include "Mathematics.h"
#include "ScenesHolder.h"
#include "AIStateGuardEvaluator.h"
#include "Scene.h"
#include "WaypointType.h"
#include "AITimer.h"
#include "Log.h"
#include "Time.h"


BEGIN_NAMESPACE
AIObject::AIObject(const unsigned int ID, GameObject* gameObject) : MovingObject(ID, gameObject),
																	m_PathPlanner(NULL),
																	m_Steering(NULL),
																	m_Brain(NULL),
																	m_ArbitrationTimer(NULL),
																	m_WType(WT_OPENWAY),
																	m_bStickOnWaypoints(true)
{
	m_Waypoints.clear();

	m_Brain = new AIBrain(this);

	m_PathPlanner = new PathPlanner(this);

	m_Steering = new SteeringBehaviors(this);

	m_ArbitrationTimer = new AITimer(1000);

	m_Steering->SeparationOn();

	//Ogre::Vector3 p1 = m_GameObject->GetPosition() + m_GameObject->GetForward() * 100; 

	//Ogre::Vector3 dir = (p1 - m_GameObject->GetPosition());
	//dir.normalise();
	//Ogre::Ray ray(m_GameObject->GetPosition(), dir);

	//std::pair<bool, float> intersection = ray.intersects(ScenesHolder::GetInstance().GetCurrentScene()->GetGameObject("Wall")->GetWorldAABB());

	//Log::GetInstance().AddInfo("Intersected: " + Ogre::StringConverter::toString(intersection.first) +
	//				" Length: " + Ogre::StringConverter::toString(intersection.second));

	m_Brain->AddStateEvaluator(new AIStateGuardEvaluator(1));
}


AIObject::~AIObject(void)
{
	Drop();
}


void AIObject::Drop(void)
{
	delete m_PathPlanner;

	delete m_Steering;

	delete m_Brain;
}


void AIObject::Update()
{
	m_Brain->Process();

	UpdateMovement();

	if(m_ArbitrationTimer->IsReady())
	{
		m_Brain->Arbitrate();
	}
}


void AIObject::UpdateMovement()
{
	Ogre::Vector3 force = m_Steering->Calculate();

	if(m_Steering->GetForce() == Ogre::Vector3::ZERO)
	{
		const float BreakingRate = 0.8f;

		m_Velocity *= BreakingRate;
	}

	m_Velocity += force;

	Mathematics::TruncateVector3(m_Velocity, m_Speed);

	m_GameObject->SetPosition(m_GameObject->GetPosition() + m_Velocity * Time::GetInstance().GetDeltaTime() / 1000);
}


bool AIObject::HandleMessage(const Message& msg)
{
	return m_Brain->HandleMessage(msg);
}


const bool AIObject::CanWalkTo(Ogre::Vector3 Pos)
{
	return CanWalkBetween(m_GameObject->GetPosition(), Pos);
}


const bool AIObject::CanWalkBetween(Ogre::Vector3 PosA, Ogre::Vector3 PosB)
{
	return !GameMap::GetInstance().IsObstractedBetween(PosA, PosB, m_BoundingRadius);
}


AIBrain* AIObject::GetBrain(void)
{
	return m_Brain;
}


PathPlanner* AIObject::GetPathPlanner(void)
{
	return m_PathPlanner;
}


SteeringBehaviors* AIObject::GetSteering(void)
{
	return m_Steering;
}


unsigned int AIObject::CalculateTimeToReachPosition(const Ogre::Vector3 &Position)
{
	return (m_GameObject->GetPosition().distance(Position) / (GetSpeed() * 60)) * 1000;
}


const bool AIObject::IsAtPosition(const Ogre::Vector3 &Position) const
{
	const float tolerance = 10.0;
	
	//Log::GetInstance().AddInfo("Distance: " + Ogre::StringConverter::toString(m_Position.squaredDistance(Position)) + 
	//	" Tolerance: " + Ogre::StringConverter::toString(tolerance * tolerance));
	return m_GameObject->GetPosition().squaredDistance(Position) < tolerance * tolerance;
}


void AIObject::LookAt(const Ogre::Vector3& Target)
{
	Ogre::Vector3 direction = (Target - m_GameObject->GetPosition());

	direction.y = 0.0f;

	direction.normalise();

	Ogre::Vector3 Forward = m_GameObject->GetForward();

	if(Forward.dotProduct(direction) >= 0.99f) return;

	Forward.y = 0.0f;

	Ogre::Quaternion quat = Forward.getRotationTo(direction);

	m_GameObject->_GetSceneNode()->rotate(quat);
}


Ogre::Quaternion AIObject::LookRotation(const Ogre::Vector3& forward)
{

	Ogre::Vector3 direction = forward;

	direction.y = 0.0f;

	direction.normalise();

	Ogre::Vector3 Forward = m_GameObject->GetForward();

	Forward.y = 0.0f;

	return Forward.getRotationTo(direction);
}


void AIObject::RotateSmoothTo(const Ogre::Vector3& Target, const float& SmoothTime)
{
	const Ogre::Vector3 dir = Target - m_GameObject->GetPosition();

	const Ogre::Quaternion& From = m_GameObject->GetOrientation();
	Ogre::Quaternion To = LookRotation(dir);

	To = To * From;

	Ogre::Quaternion delta = Ogre::Quaternion::Slerp(SmoothTime, From, To, true);

	m_GameObject->SetOrientation(delta);
}


const bool AIObject::IsFacing(const Ogre::Vector3& Target)
{
	return m_GameObject->GetForward().dotProduct((Target - m_GameObject->GetPosition()).normalisedCopy()) >= 0.99f;
}


void AIObject::AddWaypoint(const Ogre::Vector3 Waypoint)
{
	m_Waypoints.push_back(Waypoint);
}


void AIObject::AddWaypoint(const int X, const int Y, const int Z)
{
	m_Waypoints.push_back(Ogre::Vector3(X, Y, Z));
}


void AIObject::GoToFirstWaypoint(void)
{
	if(m_Waypoints.empty()) return;

	GetGameObject()->SetPosition(m_Waypoints[0]);
}


const std::vector<const Ogre::Vector3> &AIObject::GetWaypoints(void) const
{
	return m_Waypoints;
}


void AIObject::SetWaypointType(const WaypointType WType)
{
	m_WType = WType;
}


const WaypointType& AIObject::GetWaypointType(void) const
{
	return m_WType;
}


void AIObject::StickOnWaypoints(void)
{
	m_bStickOnWaypoints = true;
}


const bool AIObject::IsStuckOnWaypoints(void) const
{
	return m_bStickOnWaypoints;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AIObject)
	BEGIN_DEFINE_TYPE_BASE(MovingObject, AIObject, "AIObject")
		DEFINE_FUNCTION("LookAt", (void(AIObject::*)(const Ogre::Vector3&))&AIObject::LookAt)
		DEFINE_FUNCTION("GetSteering", &AIObject::GetSteering)
		DEFINE_FUNCTION("AddWaypoint", (void(AIObject::*)(const Ogre::Vector3))&AIObject::AddWaypoint)
		DEFINE_FUNCTION("AddWaypoint", (void(AIObject::*)(const int, const int, const int))&AIObject::AddWaypoint)
		DEFINE_FUNCTION("SetWaypointType", (void(AIObject::*)(const WaypointType))&AIObject::SetWaypointType)
		DEFINE_FUNCTION("GoToFirstWaypoint", &AIObject::GoToFirstWaypoint)
		DEFINE_FUNCTION("StickOnWaypoints", &AIObject::StickOnWaypoints)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE