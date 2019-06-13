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
#include "GameObjectFactory.h"
#include "Scene.h"
#include "MeshGameObject.h"
#include "CameraGameObject.h"
#include "LightGameObject.h"
#include "TerrainGameObject.h"
#include "OceanGameObject.h"
#include "ParticleGameObject.h"
#include "StdAfxBoost.h"


BEGIN_NAMESPACE
GameObjectFactory::GameObjectFactory(Scene *Scene) :
	m_Scene(Scene)
{
	m_ObjectTypes[GameObject::GOT_EMPTY] = "Empty";
	m_ObjectTypes[GameObject::GOT_MESH] = "Mesh";
	m_ObjectTypes[GameObject::GOT_CAMERA] = "Camera";
	m_ObjectTypes[GameObject::GOT_LIGHT] = "Light";
	m_ObjectTypes[GameObject::GOT_TERRAIN] = "Terrain";
	m_ObjectTypes[GameObject::GOT_OCEAN] = "Ocean";
	m_ObjectTypes[GameObject::GOT_PARTICLE] = "Particle";
}


//GameObjectFactory::~GameObjectFactory(void)
//{
//}


const char *GameObjectFactory::GetStringOfType(const GameObject::GameObjectType &Type)
{
	return m_ObjectTypes[Type];
}


GameObject *GameObjectFactory::CreateGameObject(const std::string &TypeName, const std::string &Name, GameObject *Parent)
{
	GameObject::GameObjectType type;
	bool typeFound = false;

	BOOST_FOREACH(const ObjectTypePair &value, m_ObjectTypes)
		if (value.second == TypeName)
		{
			type = value.first;
			typeFound = true;
			break;
		}
		
	GameObject *obj = NULL;

	if (typeFound)
		switch (type)
		{
		case GameObject::GOT_EMPTY:
			{
				obj = m_Scene->CreateEmptyGameObject(Name);
			} break;
		
		case GameObject::GOT_MESH:
			{
				obj = m_Scene->CreateMeshGameObject(Name, "", Parent);
			} break;

		case GameObject::GOT_CAMERA:
			{
				obj = m_Scene->CreateCameraGameObject(Name, Parent);
			} break;

		case GameObject::GOT_LIGHT:
			{
				obj = m_Scene->CreateLightGameObject(Name, Parent);
			} break;

		case GameObject::GOT_TERRAIN:
			{
				obj = m_Scene->CreateTerrainGameObject(Name, Parent);
			} break;

		case GameObject::GOT_OCEAN:
			{
				obj = m_Scene->CreateOceanGameObject(Name, Parent);
			} break;

		case GameObject::GOT_PARTICLE:
			{
				obj = m_Scene->CreateParticleGameObject(Name, Parent);
			} break;
		}

	return obj;
}
END_NAMESPACE