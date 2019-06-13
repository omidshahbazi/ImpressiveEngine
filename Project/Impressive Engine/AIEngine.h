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

#include "Singleton.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class GameObject;
class AIObject;
class AIEnemyObject;
class AINPCObject;
class AISellerObject;
class AISoldierObject;
class AICommanderObject;
class PathManagerThreadWorker;

class AIEngine : public Singleton<AIEngine>
{
public:
	typedef std::map<const unsigned int, AIObject*> ObjectMap;

public:
	AIEngine(void);

	~AIEngine(void);

	void ResetToInitializeState(void);

	void Update(void);

	AIEnemyObject* CreateEnemyObject(GameObject* gameObject);

	AINPCObject* CreateNPCObject(GameObject* gameObject, bool RandomInitPosition = false);

	AISellerObject* CreateSellerObject(GameObject* gameObject);

	AISoldierObject* CreateSoldierObject(GameObject* gameObject);

	AICommanderObject* CreateCommanderObject(GameObject* gameObject);

	AIEnemyObject* GetEnemyObject(const unsigned int& ID);

	AIObject* GetAIObject(const unsigned int& ID);

	const ObjectMap& GetAIObjects(void) const;

	void TagAIObjectsFor(AIObject* Object, float& Distance);

	DECLARE_SCRIPT_REGISTATION()

private:
	void RemoveAIObjects(void);

private:

	PathManagerThreadWorker* m_PathManagerThreadWorker;

	unsigned int m_ObjectCounter;

	typedef std::vector<AISellerObject*> SellerVector;

	ObjectMap m_ObjectMap;
	SellerVector m_Sellers;

};


END_NAMESPACE