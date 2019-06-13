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
#include "AIEnemyObject.h"
#include "GameObject.h"
#include "AIBrain.h"
#include "AIStateGuardEvaluator.h"
#include "AIObjectType.h"


BEGIN_NAMESPACE
AIEnemyObject::AIEnemyObject(const unsigned int ID, GameObject* gameObject) : AIObject(ID, gameObject)
{

}


AIEnemyObject::~AIEnemyObject(void)
{
}


void AIEnemyObject::PursueTarget()
{
	//if(!m_Target) return;

	////double dist = m_Target->GetPosition().distance(m_Position);

	////double T = dist / obj->GetTarget()->GetSpeed();

	////Ogre::Vector3 targetPosition = m_Target->GetPosition() + (m_Target->GetVelocity()/* * T*/);

	//if((Time::GetInstance().GetRealTime() - m_StartPursuitTime) > 500)
	//{
	//	counter++;

	//	m_WorkerThread.interrupt();
	//	m_NewPath = NULL;
	//	delete m_NewPath;
	//	//unsigned int targetCurrentNode = GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->GetNodeIndexInTerrainSpace(m_Target->GetPosition());
	//	//if(targetCurrentNode != m_TargetPreviousNode)
	//	//{
	//	RequestPathTo(m_Target);

	//	m_StartPursuitTime = Time::GetInstance().GetRealTime();
	//	//}
	//}

	//FollowPath();
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AIEnemyObject)
	BEGIN_DEFINE_TYPE_BASE(AIObject, AIEnemyObject, "AIEnemyObject")
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE