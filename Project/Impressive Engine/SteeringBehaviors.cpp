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
#include "SteeringBehaviors.h"
#include "SteeringBehaviorTypes.h"
#include "AIEngine.h"
#include "AIObject.h"
#include "AISoldierObject.h"
#include "AICommanderObject.h"
#include "AIObjectType.h"
#include "GameObject.h"
#include "GameMap.h"
#include "Ray.h"
#include "NavigationGraph.h"
#include "Mathematics.h"
#include "StdAfxBoost.h"
#include "Time.h"
#include "Log.h"


BEGIN_NAMESPACE
SteeringBehaviors::SteeringBehaviors(AIObject* Owner) : m_Owner(Owner),
														   m_Flags(0),
														   m_Friend(NULL),
														   m_WeightSeek(0.5f),
														   m_WeightFlee(1.0f),
														   m_WeightArrive(1.0f),
														   m_WeightSeparation(20.0f),
														   m_WeightAlignment(0.80f),
														   m_WeightCohesion(2.50f),
														   m_WeightWallAvoidance(10.0f),
														   m_WeightObstacleAvoidance(10.0f),
														   m_WeightWander(50.0f),
														   m_WeightOffsetPursuit(1.0f),
														   m_WanderRadius(WANDER_RADIUS),
														   m_WanderJitter(WANDER_JITTER),
														   m_WanderDistance(WANDER_DISTANCE),
														   m_Deceleration(DT_NORMAL),
														   m_DecelerationRate(m_Owner->GetSpeed() * 2.5f),
														   m_DistanceToOthers(00.0f),
														   m_WallDetectionFeelerLength(25.0f),
														   m_ObstacleDetectionLength(0.0f),
														   m_Offset(Ogre::Vector3::ZERO),
														   m_bIsStucked(false)
{
	std::srand(Time::GetInstance().GetRealTime());

	const float theta = std::rand() * (Ogre::Math::PI / 2);

	m_WanderTarget = Ogre::Vector3(m_WanderRadius * Ogre::Math::Sin(theta), 0, m_WanderRadius * Ogre::Math::Cos(theta));
}


SteeringBehaviors::~SteeringBehaviors(void)
{
}


bool SteeringBehaviors::AccumulateForce(Ogre::Vector3 &SteeringForce, const Ogre::Vector3& ForceToAdd)
{
	const float MagnitudeSoFar = SteeringForce.length();

	const float MagnitudeRemaining = m_Owner->GetForce() - MagnitudeSoFar;

	if(MagnitudeRemaining <= 0.0) return false;

	float MagnitudeToAdd = ForceToAdd.length();

	if(MagnitudeToAdd < MagnitudeRemaining)
	{
		m_SteeringForce += ForceToAdd;
	}
	else
	{
		MagnitudeToAdd = MagnitudeRemaining;

		m_SteeringForce += ForceToAdd.normalisedCopy() * MagnitudeToAdd;
	}

	return true;
}


Ogre::Vector3 SteeringBehaviors::Calculate(void)
{
	m_SteeringForce = Ogre::Vector3::ZERO;

	//if(On(SBT_SEPARATION))
	//{
	//	AIEngine::GetInstance().TagAIObjectsFor(m_Owner, m_DistanceToOthers);
	//}

	m_SteeringForce = CalculatePrioritized();

	return m_SteeringForce;
}


Ogre::Vector3 SteeringBehaviors::CalculatePrioritized(void)
{
	m_bIsStucked = false;

	Ogre::Vector3 force = Ogre::Vector3::ZERO;

	if(On(SBT_OBSTACLE_AVOIDANCE))
	{
		force = ObstacleAvoidance(GameMap::GetInstance().GetObstacles()) * m_WeightObstacleAvoidance;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	if(On(SBT_WALL_AVOIDANCE))
	{
		force = WallAvoidance(GameMap::GetInstance().GetObstacles()) * m_WeightWallAvoidance;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}


	if(On(SBT_SEPARATION))
	{
		force = Separation(AIEngine::GetInstance().GetAIObjects()) * m_WeightSeparation;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	if(On(SBT_ALIGNMENT))
	{
		force = Alignment(AIEngine::GetInstance().GetAIObjects()) * m_WeightAlignment;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	if(On(SBT_COHESION))
	{
		force = Cohesion(AIEngine::GetInstance().GetAIObjects()) * m_WeightCohesion;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	if(On(SBT_OFFSET_PURSUIT))
	{
		force = OffsetPursuit(((AISoldierObject*)m_Owner)->GetCommander(), ((AISoldierObject*)m_Owner)->GetOffsetToCommander()) * m_WeightOffsetPursuit;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	if(On(SBT_SEEK) && !m_bIsStucked)
	{
		force = Seek(m_Target) * m_WeightSeek;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	if(On(SBT_ARRIVE) && m_bIsStucked)
	{
		force = Arrive(m_Target, m_Deceleration) * m_WeightArrive;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	if(On(SBT_WANDER))
	{
		force = Wander() * m_WeightWander;

		if(!AccumulateForce(m_SteeringForce, force)) return m_SteeringForce;
	}

	return m_SteeringForce;
}


const bool SteeringBehaviors::On(SteeringBehaviorType bt) const
{
	return (m_Flags & bt) == bt;
}


void SteeringBehaviors::SeekOn(void)
{
	m_Flags |= SBT_SEEK;
}


void SteeringBehaviors::FleeOn(void)
{
	m_Flags |= SBT_FLEE;
}


void SteeringBehaviors::ArriveOn(void)
{
	m_Flags |= SBT_ARRIVE;
}


void SteeringBehaviors::SeparationOn(void)
{
	m_Flags |= SBT_SEPARATION;
}


void SteeringBehaviors::AlignmentOn(void)
{
	m_Flags |= SBT_ALIGNMENT;
}


void SteeringBehaviors::CohesionOn(void)
{
	m_Flags |= SBT_COHESION;
}


void SteeringBehaviors::WallAvoidanceOn(void)
{
	m_Flags |= SBT_WALL_AVOIDANCE;
}


void SteeringBehaviors::ObstacleAvoidanceOn(void)
{
	m_Flags |= SBT_OBSTACLE_AVOIDANCE;
}

void SteeringBehaviors::WanderOn(void)
{
	m_Flags |= SBT_WANDER;
}


void SteeringBehaviors::OffsetPursuitOn(void)
{
	m_Flags |= SBT_OFFSET_PURSUIT;
}


void SteeringBehaviors::SeekOff(void)
{
	if(On(SBT_SEEK))
	{
		m_Flags ^= SBT_SEEK;
	}
}


void SteeringBehaviors::FleeOff(void)
{
	if(On(SBT_FLEE))
	{
		m_Flags ^= SBT_FLEE;
	}
}


void SteeringBehaviors::ArriveOff(void)
{
	if(On(SBT_ARRIVE))
	{
		m_Flags ^= SBT_ARRIVE;
	}
}


void SteeringBehaviors::SeparationOff(void)
{
	if(On(SBT_SEPARATION))
	{
		m_Flags ^= SBT_SEPARATION;
	}
}


void SteeringBehaviors::AlignmentOff(void)
{
	if(On(SBT_ALIGNMENT))
	{
		m_Flags ^= SBT_ALIGNMENT;
	}
}


void SteeringBehaviors::CohesionOff(void)
{
	if(On(SBT_COHESION))
	{
		m_Flags ^= SBT_COHESION;
	}
}


void SteeringBehaviors::WallAvoidanceOff(void)
{
	if(On(SBT_WALL_AVOIDANCE))
	{
		m_Flags ^= SBT_WALL_AVOIDANCE;
	}
}


void SteeringBehaviors::ObstacleAvoidanceOff(void)
{
	if(On(SBT_OBSTACLE_AVOIDANCE))
	{
		m_Flags ^= SBT_OBSTACLE_AVOIDANCE;
	}
}

void SteeringBehaviors::WanderOff(void)
{
	if(On(SBT_WANDER))
	{
		m_Flags ^= SBT_WANDER;
	}
}


void SteeringBehaviors::OffsetPursuitOff(void)
{
	if(On(SBT_OFFSET_PURSUIT))
	{
		m_Flags ^= SBT_OFFSET_PURSUIT;
	}
}


const bool SteeringBehaviors::IsSeekOn(void) const
{
	return On(SBT_SEEK);
}


const bool SteeringBehaviors::IsFleeOn(void) const
{
	return On(SBT_FLEE);
}


const bool SteeringBehaviors::IsArriveOn(void) const
{
	return On(SBT_ARRIVE);
}


const bool SteeringBehaviors::IsSeparationOn(void) const
{
	return On(SBT_SEPARATION);
}


const bool SteeringBehaviors::IsAlignmentOn(void) const
{
	return On(SBT_ALIGNMENT);
}


const bool SteeringBehaviors::IsCohesionOn(void) const
{
	return On(SBT_COHESION);
}


const bool SteeringBehaviors::IsWallAvoidanceOn(void) const
{
	return On(SBT_WALL_AVOIDANCE);
}


const bool SteeringBehaviors::IsObstacleAvoidanceOn(void) const
{
	return On(SBT_OBSTACLE_AVOIDANCE);
}


const bool SteeringBehaviors::IsWanderOn(void) const
{
	return On(SBT_WANDER);
}


const bool SteeringBehaviors::IsOffsetPursuitOn(void) const
{
	return On(SBT_OFFSET_PURSUIT);
}


Ogre::Vector3 SteeringBehaviors::Seek(const Ogre::Vector3 &Target)
{
	Ogre::Vector3 DesiredVelocity = (Target - m_Owner->GetGameObject()->GetPosition()).normalisedCopy() * m_Owner->GetSpeed();

	return DesiredVelocity - m_Owner->GetVelocity();
}


Ogre::Vector3 SteeringBehaviors::Flee(const Ogre::Vector3 &Target)
{
	Ogre::Vector3 DesiredVelocity = (m_Owner->GetGameObject()->GetPosition() - Target).normalisedCopy() * m_Owner->GetSpeed();

	return DesiredVelocity - m_Owner->GetVelocity();
}


Ogre::Vector3 SteeringBehaviors::Arrive(const Ogre::Vector3 &Target, const DecelerationType Deceleration)
{
	Ogre::Vector3 ToTarget = Target - m_Owner->GetGameObject()->GetPosition();

	float Distance = ToTarget.length();

	float Speed = Distance / ((float)Deceleration * m_DecelerationRate);

	Speed = Mathematics::Min(Speed, m_Owner->GetSpeed());

	if (Speed <= 0.01f) Speed = 0.0f;

	Ogre::Vector3 DesiredVelocity = Distance > 0 ? ToTarget * Speed / Distance : ToTarget * Speed;

	return DesiredVelocity - m_Owner->GetVelocity();
}


Ogre::Vector3 SteeringBehaviors::Separation(const std::map<const unsigned int, AIObject*>& AIObjects)
{
	Ogre::Vector3 SteeringForce = Ogre::Vector3::ZERO;

	std::map<const unsigned int, AIObject*>::const_iterator it = AIObjects.begin();

	for(it; it != AIObjects.end(); it++)
	{
		Ogre::Vector3 Dir = it->second->GetGameObject()->GetPosition() - m_Owner->GetGameObject()->GetPosition();

		float range = (m_Owner->GetBoundingRadius() + it->second->GetBoundingRadius());

		if((it->second != m_Owner) && (Dir.squaredLength() < range * range))
		{
			Ogre::Vector3 forward = m_Owner->GetGameObject()->GetForward();

			if(forward.dotProduct(Dir) > 0.9f)
			{
				m_bIsStucked = true;

				Ogre::Vector3 ToOther = -Dir;

				if(Dir.dotProduct(m_Owner->GetGameObject()->GetRight()) >= 0.0f)
				{
					SteeringForce = ToOther.normalisedCopy()/ToOther.length() + m_Owner->GetGameObject()->GetRight()/ToOther.length();
				}
				else
				{
					SteeringForce = ToOther.normalisedCopy()/ToOther.length() - m_Owner->GetGameObject()->GetRight()/ToOther.length();
				}
			}
		}
	}

	return SteeringForce;
}


Ogre::Vector3 SteeringBehaviors::Alignment(const std::map<const unsigned int, AIObject*>& AIObjects)
{
	Ogre::Vector3 AverageHeading = Ogre::Vector3::ZERO;

	unsigned int NeighborCount = 0;

	std::map<const unsigned int, AIObject*>::const_iterator it = AIObjects.begin();

	for(it; it != AIObjects.end(); it++)
	{
		if(it->second != m_Owner && it->second->IsTagged() && it->second->GetType() == AIOT_NPC)
		{
			AverageHeading = m_Owner->GetGameObject()->GetForward();

			++NeighborCount;
		}
	}

	if(NeighborCount > 0)
	{
		AverageHeading /= NeighborCount;

		AverageHeading -= m_Owner->GetGameObject()->GetForward();
	}

	return AverageHeading;
}


Ogre::Vector3 SteeringBehaviors::Cohesion(const std::map<const unsigned int, AIObject*>& AIObjects)
{
	Ogre::Vector3 SteeringForce = Ogre::Vector3::ZERO;
	Ogre::Vector3 CenterOfMass = Ogre::Vector3::ZERO;

	unsigned int NeighborCount = 0;

	std::map<const unsigned int, AIObject*>::const_iterator it = AIObjects.begin();

	for(it; it != AIObjects.end(); it++)
	{
		if(it->second != m_Owner && it->second->IsTagged() && it->second->GetType() == AIOT_NPC)
		{
			CenterOfMass = m_Owner->GetGameObject()->GetPosition();

			++NeighborCount;
		}
	}

	if(NeighborCount > 0)
	{
		CenterOfMass /= NeighborCount;

		SteeringForce = Seek(CenterOfMass);
	}

	return SteeringForce;
}


Ogre::Vector3 SteeringBehaviors::WallAvoidance(const std::list<GameObject*>& Obstacles)
{
	CreateFeelers();

	float DistToClosestObstacle = 100.0f;
	int ClosestWallIndex = -1;

	Ogre::Vector3 SteeringForce = Ogre::Vector3::ZERO;
	Ogre::Vector3 ClosestPoint = Ogre::Vector3::ZERO;
	Ogre::Vector3 OverShoot = Ogre::Vector3::ZERO;

	GameObject* Obstacle = NULL;

	for(unsigned int flr = 0; flr < m_Feelers.size(); flr++)
	{
		int index = -1;

		BOOST_FOREACH(const GameObject* current, GameMap::GetInstance().GetObstacles())
		{
			index++;

			Ogre::Vector3 dir = (m_Feelers[flr] - m_Owner->GetGameObject()->GetPosition());
			Ogre::Ray ray(m_Owner->GetGameObject()->GetPosition(), dir);
			std::pair<bool, float> intersection = ray.intersects(current->GetWorldAABB());

			if(intersection.first)
			{
				if(intersection.second < DistToClosestObstacle)
				{
					DistToClosestObstacle = intersection.second;
					ClosestWallIndex = index;
					ClosestPoint = ray.getPoint(intersection.second);
				}
			}
		}

		if(ClosestWallIndex >= 0)
		{
			OverShoot = m_Feelers[flr] - ClosestPoint;
			//SteeringForce = GameMap::GetInstance().GetObstacle(index)->GetWorldAABB(). * OverShoot.length();
		}
	}

	return SteeringForce;
}


Ogre::Vector3 SteeringBehaviors::ObstacleAvoidance(const std::list<GameObject*>& Obstacles)
{
	m_ObstacleDetectionLength = m_Owner->GetBoundingRadius() + m_Owner->GetSpeed() * m_Owner->GetBoundingRadius();

	GameObject* ClosestIntersectingObstacle = NULL;
	float ClosestIntersectingObstacleBR = 0.0f;

	float DistToClosestObstacle = 50.0f;

	Ogre::Vector3 LocalPosOfClosestObstacle;

	BOOST_FOREACH(GameObject* current, GameMap::GetInstance().GetObstacles())
	{
		const Ogre::Vector3 dir = (current->GetPosition() - m_Owner->GetGameObject()->GetPosition());

		if(dir.squaredLength() < m_ObstacleDetectionLength * m_ObstacleDetectionLength)
		{
			Ogre::Vector3 LocalPos = m_Owner->GetGameObject()->ConvertWorldToLocalPosition(current->GetPosition());

			const Ogre::Vector3 forward = m_Owner->GetGameObject()->GetForward();
			float dot = dir.dotProduct(forward);

			if(LocalPos.x >= 0)
			{
				const Ogre::AxisAlignedBox Box = current->GetWorldAABB();
				const float ObstacleBoundingRadius = Mathematics::Max((Box.getMaximum().x - Box.getMinimum().x),
													(Box.getMaximum().z - Box.getMinimum().z)) / 2;

				float ExpandedRadius = ObstacleBoundingRadius + m_Owner->GetBoundingRadius();

				if (fabs(LocalPos.z) < ExpandedRadius)
				{

					float cX = LocalPos.x;
					float cz = LocalPos.z;
          
					float SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cz*cz);

					float ip = cX - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cX + SqrtPart;
					}

					if (ip < DistToClosestObstacle)
					{
						DistToClosestObstacle = ip;

						ClosestIntersectingObstacle = current;

						ClosestIntersectingObstacleBR = ObstacleBoundingRadius;

						LocalPosOfClosestObstacle = LocalPos;
					}         
				}
			}
		}
	}

	Ogre::Vector3 SteeringForce = Ogre::Vector3::ZERO;

	if (ClosestIntersectingObstacle)
	{
		float multiplier = 0.001f + (m_ObstacleDetectionLength - LocalPosOfClosestObstacle.x) / m_ObstacleDetectionLength;

		SteeringForce.z = (ClosestIntersectingObstacleBR - LocalPosOfClosestObstacle.z)  * multiplier;   

		const float BrakingWeight = 0.001f;

		SteeringForce.x = (ClosestIntersectingObstacleBR - LocalPosOfClosestObstacle.x) * BrakingWeight;

		SteeringForce = m_Owner->GetGameObject()->ConvertLocalToWorldPosition(SteeringForce);
	}

	return 	SteeringForce;
}


Ogre::Vector3 SteeringBehaviors::Wander()
{
	float JitterThisTimeSlice = m_WanderJitter * Time::GetInstance().GetDeltaTime();

	m_WanderTarget += Ogre::Vector3(  (std::rand()%50 - rand()%50) * JitterThisTimeSlice, 
									  0, (std::rand()%50 - rand()%50) * JitterThisTimeSlice);

	m_WanderTarget.normalise();

	m_WanderTarget *= m_WanderRadius;

	Ogre::Vector3 targetLocal = m_WanderTarget + Ogre::Vector3(m_WanderDistance, 0, m_WanderDistance);

	Ogre::Vector3 Target = m_Owner->GetGameObject()->ConvertLocalToWorldPosition(targetLocal);

	return Target - m_Owner->GetGameObject()->GetPosition();
}


Ogre::Vector3 SteeringBehaviors::OffsetPursuit(AIObject* Leader, const Ogre::Vector3& Offset)
{
	Ogre::Vector3 offset = Ogre::Vector3::ZERO;

	if(Leader->GetVelocity() == Ogre::Vector3::ZERO)
	{
		offset = Offset;
	}
	else
	{
		offset = ((AICommanderObject*)Leader)->CalculateOffsetToSoldier((AISoldierObject*)m_Owner);
	}

	if(m_Owner->IsAtPosition(offset)) return Ogre::Vector3::ZERO;

	Ogre::Vector3 ToOffset = offset - m_Owner->GetGameObject()->GetPosition();

	float SquaredSpeed = (m_Owner->GetSpeed() + Leader->GetSpeed()) * (m_Owner->GetSpeed() + Leader->GetSpeed());
	float LookAheadTime = offset.squaredLength() / SquaredSpeed;

	return Arrive(offset + Leader->GetVelocity() * LookAheadTime, DT_NORMAL);
}


void SteeringBehaviors::SetOffset(const Ogre::Vector3 Offset)
{
	m_Offset = Offset;
}


void SteeringBehaviors::SetTarget(const Ogre::Vector3 &Target)
{
	m_Target = Target;
}


const Ogre::Vector3& SteeringBehaviors::GetForce(void) const
{
	return m_SteeringForce;
}


void SteeringBehaviors::CreateFeelers(void)
{
	m_Feelers[0] = m_Owner->GetGameObject()->GetPosition() + m_Owner->GetGameObject()->GetForward() * m_WallDetectionFeelerLength; 

	m_Feelers[1] =  m_Owner->GetGameObject()->GetPosition() + (m_WallDetectionFeelerLength/2) * (m_Owner->GetGameObject()->GetForward() + m_Owner->GetGameObject()->GetRight());

	m_Feelers[2] = m_Owner->GetGameObject()->GetPosition() + (m_WallDetectionFeelerLength/2) * (m_Owner->GetGameObject()->GetForward() + m_Owner->GetGameObject()->GetRight().reflect(m_Owner->GetGameObject()->GetRight()));
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(SteeringBehaviors)
	BEGIN_DEFINE_TYPE(SteeringBehaviors, "Steering")
		DEFINE_FUNCTION("SeekOn", &SteeringBehaviors::SeekOn)
		DEFINE_FUNCTION("SeekOff", &SteeringBehaviors::SeekOff)
		DEFINE_FUNCTION("ArriveOn", &SteeringBehaviors::ArriveOn)
		DEFINE_FUNCTION("ArriveOff", &SteeringBehaviors::ArriveOff)
		DEFINE_FUNCTION("SeparationOn", &SteeringBehaviors::SeparationOn)
		DEFINE_FUNCTION("SeparationOff", &SteeringBehaviors::SeparationOff)
		DEFINE_FUNCTION("AlignmentOn", &SteeringBehaviors::AlignmentOn)
		DEFINE_FUNCTION("AlignmentOff", &SteeringBehaviors::AlignmentOff)
		DEFINE_FUNCTION("CohesionOn", &SteeringBehaviors::CohesionOn)
		DEFINE_FUNCTION("CohesionOff", &SteeringBehaviors::CohesionOff)
		DEFINE_FUNCTION("ObstacleAvoidanceOn", &SteeringBehaviors::ObstacleAvoidanceOn)
		DEFINE_FUNCTION("ObstacleAvoidanceOff", &SteeringBehaviors::ObstacleAvoidanceOff)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE