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

#include "GameObject.h"
#include <map>

BEGIN_NAMESPACE
class Scene;

class GameObjectFactory
{
public:
	GameObjectFactory(Scene *Scene);
	//~GameObjectFactory(void);

	const char *GetStringOfType(const GameObject::GameObjectType &Type);

	GameObject *CreateGameObject(const std::string &TypeName, const std::string &Name, GameObject *Parent);

private:
	Scene *m_Scene;
	
	typedef std::pair<GameObject::GameObjectType, const char*> ObjectTypePair;
	typedef std::map<GameObject::GameObjectType, const char*> ObjectTypeMap;

	ObjectTypeMap m_ObjectTypes;
};


END_NAMESPACE