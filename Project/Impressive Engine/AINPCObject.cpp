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
#include "AINPCObject.h"
#include "AISellerObject.h"
#include "GameObject.h"
#include "AIObjectType.h"
#include "AIBrain.h"
#include "SteeringBehaviors.h"
#include "AIStateExploreEvaluator.h"
#include "AIStateShoppingEvaluator.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"
#include "Mathematics.h"
#include "Time.h"


BEGIN_NAMESPACE
AINPCObject::AINPCObject(const unsigned int ID, GameObject* gameObject,
												bool& RandomInitPosition) : AIObject(ID, gameObject),
																		  m_SpawnPoint(Ogre::Vector3::ZERO),
																		  m_bGoShopping(false),
																		  m_LastTimeShoppingFinished(0),
																		  m_ShoppingInterval(0),
																		  m_Seller(NULL),
																		  m_OffsetToSeller(Ogre::Vector3::ZERO),
																		  STOPT_TIME_AT_SELLER(10000),
																		  MIN_SHOPPINT_TIME(10000),
																		  MAX_SHOPPINT_TIME(15000)
{
	m_Type = AIOT_NPC;

	m_ShoppingInterval = Mathematics::Rand32(MIN_SHOPPINT_TIME, MAX_SHOPPINT_TIME);

	if(RandomInitPosition && !GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->IsEmpty())
	{
		m_SpawnPoint = GameMap::GetInstance().GetNavGraph()->GetRandomLocation();

		m_GameObject->SetPosition(m_SpawnPoint);
	}

	m_Brain->AddStateEvaluator(new AIStateExploreEvaluator(1));
	m_Brain->AddStateEvaluator(new AIStateShoppingEvaluator(1));

	//m_Steering->AlignmentOn();
	//m_Steering->CohesionOn();

	//m_Steering->WanderOn();
	//m_Steering->ObstacleAvoidanceOn();
}


AINPCObject::~AINPCObject(void)
{
}


const bool &AINPCObject::IsGoingShopping(void) const
{
	return m_bGoShopping;
}


void AINPCObject::GoShopping(AISellerObject* pSeller)
{
	m_Seller = pSeller;
	m_bGoShopping = true;
}


void AINPCObject::LeaveShopping(void)
{
	m_Seller->FreeRoom(this);
	m_bGoShopping = false;
	GetSteering()->SeparationOn();
	m_LastTimeShoppingFinished = Time::GetInstance().GetRealTime();
}


const unsigned int &AINPCObject::GetLastTimeShoppingFinished(void) const
{
	return m_LastTimeShoppingFinished;
}


const unsigned int &AINPCObject::GetShoppingInterval(void) const
{
	return m_ShoppingInterval;
}


AISellerObject* AINPCObject::GetSeller(void)
{
	return m_Seller;
}


const Ogre::Vector3& AINPCObject::GetOffsetToSeller(void) const
{
	return m_OffsetToSeller;
}


void AINPCObject::SetOffsetToSeller(const Ogre::Vector3 Offset)
{
	m_OffsetToSeller = Offset;
}


void AINPCObject::ExploreFreely(void)
{
	m_bStickOnWaypoints = false;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AINPCObject)
	BEGIN_DEFINE_TYPE_BASE(AIObject, AINPCObject, "AINPCObject")
		DEFINE_FUNCTION("LastRoomPos", &AINPCObject::GetOffsetToSeller)
		DEFINE_FUNCTION("ExploreFreely", &AINPCObject::ExploreFreely)
		//DEFINE_FUNCTION("SetTarget", (void(AIEnemyObject::*)(AIEnemyObject*))&AIEnemyObject::SetTarget)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE