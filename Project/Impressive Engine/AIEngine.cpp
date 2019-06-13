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
#include "AIEngine.h"
#include "AINPCObject.h"
#include "AISellerObject.h"
#include "AISoldierObject.h"
#include "AICommanderObject.h"
#include "AIObjectType.h"
#include "PathManager.h"
#include "MainWindow.h"
#include "GameObject.h"
#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"
#include "PathManagerThreadWorker.h"
#include "ThreadingManager.h"
#include "Log.h"


BEGIN_NAMESPACE
AIEngine *Singleton<AIEngine>::m_Instance = NULL;


AIEngine::AIEngine(void) : m_PathManagerThreadWorker(NULL), m_ObjectCounter(0)
{
	m_PathManagerThreadWorker = new PathManagerThreadWorker();
}


AIEngine::~AIEngine(void)
{
	ResetToInitializeState();
}


void AIEngine::ResetToInitializeState(void)
{
	PathManager::GetInstance().Drop();
	RemoveAIObjects();
}


void AIEngine::Update(void)
{
#ifdef EDIT_MODE
	if(MainWindow::GetPointer()->IsInEditMode())
	{
		return;
	}
#endif

	if(GameMap::GetInstance().GetNavGraph()->GetSparseGraph()->IsEmpty())
	{
		return;
	}

	//ThreadingManager::GetInstance().CreateThread(m_PathManagerThreadWorker);

	PathManager::GetInstance().UpdateSearches();

	for(ObjectMap::iterator obj = m_ObjectMap.begin(); obj != m_ObjectMap.end(); obj++)
	{
		obj->second->Update();
	}

}


AIEnemyObject* AIEngine::CreateEnemyObject(GameObject* gameObject)
{
	AIObject* obj = GetAIObject(m_ObjectCounter);

	if(obj) return (AIEnemyObject*)obj;

	obj = new AIEnemyObject(m_ObjectCounter, gameObject);

	m_ObjectMap.insert(std::make_pair(m_ObjectCounter, obj));

	m_ObjectCounter++;

	return (AIEnemyObject*)obj;
}


AINPCObject* AIEngine::CreateNPCObject(GameObject* gameObject, bool RandomInitPosition)
{
	AIObject* obj = GetAIObject(m_ObjectCounter);

	if(obj) return (AINPCObject*)obj;

	obj = new AINPCObject(m_ObjectCounter, gameObject, RandomInitPosition);

	m_ObjectMap.insert(std::make_pair(m_ObjectCounter, obj));

	m_ObjectCounter++;

	return (AINPCObject*)obj;
}


AISellerObject* AIEngine::CreateSellerObject(GameObject* gameObject)
{
	AIObject* obj = GetAIObject(m_ObjectCounter);

	if(obj) return (AISellerObject*)obj;

	obj = new AISellerObject(m_ObjectCounter, gameObject);

	m_ObjectMap.insert(std::make_pair(m_ObjectCounter, obj));

	m_ObjectCounter++;

	return (AISellerObject*)obj;
}


AISoldierObject* AIEngine::CreateSoldierObject(GameObject* gameObject)
{
	AIObject* obj = GetAIObject(gameObject->GetID());

	if(obj) return (AISoldierObject*)obj;

	obj = new AISoldierObject(gameObject->GetID(), gameObject);

	m_ObjectMap.insert(std::make_pair(gameObject->GetID(), obj));

	m_ObjectCounter++;

	return (AISoldierObject*)obj;
}


AICommanderObject* AIEngine::CreateCommanderObject(GameObject* gameObject)
{
	AIObject* obj = GetAIObject(m_ObjectCounter);

	if(obj) return (AICommanderObject*)obj;

	obj = new AICommanderObject(m_ObjectCounter, gameObject);

	m_ObjectMap.insert(std::make_pair(m_ObjectCounter, obj));

	m_ObjectCounter++;

	return (AICommanderObject*)obj;
}


AIEnemyObject* AIEngine::GetEnemyObject(const unsigned int& ID)
{
	return (AIEnemyObject*)GetAIObject(ID);
}


AIObject* AIEngine::GetAIObject(const unsigned int& ID)
{
	if(m_ObjectMap.count(ID) > 0) return m_ObjectMap.find(ID)->second;

	return NULL;
}


const AIEngine::ObjectMap& AIEngine::GetAIObjects(void) const
{
	return m_ObjectMap;
}


void AIEngine::TagAIObjectsFor(AIObject* Object, float& Distance)
{
	std::map<const unsigned int, AIObject*>::const_iterator it = m_ObjectMap.begin();

	for(it; it != m_ObjectMap.end(); it++)
	{
		it->second->UnTag();

		Ogre::Vector3 toOther = it->second->GetGameObject()->GetPosition() - Object->GetGameObject()->GetPosition();

		float range = Distance + it->second->GetBoundingRadius();

		if((it->second != Object) && (toOther.squaredLength() < range * range))
		{
			it->second->Tag();
		}
	}
}


void AIEngine::RemoveAIObjects(void)
{
	for(ObjectMap::iterator obj = m_ObjectMap.begin(); obj != m_ObjectMap.end(); obj++)
	{
		obj->second->Drop();
	}

	m_ObjectMap.clear();
	m_ObjectCounter = 0;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(AIEngine)
	BEGIN_DEFINE_TYPE(AIEngine, "AIEngine")
		DEFINE_FUNCTION("CreateEnemyObject", (AIEnemyObject*(AIEngine::*)(GameObject*))&AIEngine::CreateEnemyObject)
		//DEFINE_FUNCTION("CreateNPCObject", (AINPCObject*(AIEngine::*)(GameObject*))&AIEngine::CreateNPCObject)
		DEFINE_FUNCTION("CreateNPCObject", (AINPCObject*(AIEngine::*)(GameObject*, bool))&AIEngine::CreateNPCObject)
		DEFINE_FUNCTION("CreateSellerObject", (AISellerObject*(AIEngine::*)(GameObject*))&AIEngine::CreateSellerObject)
		DEFINE_FUNCTION("CreateSoldierObject", (AISellerObject*(AIEngine::*)(GameObject*))&AIEngine::CreateSoldierObject)
		DEFINE_FUNCTION("CreateCommanderObject", (AICommanderObject*(AIEngine::*)(GameObject*))&AIEngine::CreateCommanderObject)
		DEFINE_FUNCTION("GetEnemyObject", (AIEnemyObject*(AIEngine::*)(const unsigned int))&AIEngine::GetEnemyObject)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE