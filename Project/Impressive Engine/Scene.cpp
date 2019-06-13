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
#include "Scene.h"
#include "Engine.h"
#include "ScenesHolder.h"
#include "IO.h"
#include "DeferredShadingSystem.h"
#include "GameObjectFactory.h"
#include "MeshGameObject.h"
#include "CameraGameObject.h"
#include "LightGameObject.h"
#include "TerrainGameObject.h"
#include "OceanGameObject.h"
#include "ParticleGameObject.h"
#include "Utility.h"
#include "Exception.h"
#include "Log.h"
#include "Time.h"
#include "ScriptRegister.h"
#include "TreeParser.h"
#include "ShadowManager.h"
//#include <OgreScriptCompiler.h>

#include "GameMap.h"
#include "NavigationGraph.h"
#include "SparseGraph.h"
#include "SparseGraphThreadWorker.h"
#include "TerrainGameObject.h"
#include "ThreadingManager.h"

#ifdef DEBUG
#include "Stats.h"
#endif

#include "AIEngine.h"


BEGIN_NAMESPACE
const char *Scene::TAG_SCENE("SCENE");
const char *Scene::TAG_LEVEL("LEVEL");
const char *Scene::TAG_ATTRIBUTES("ATTRIBUTES");
const char *Scene::TAG_CAMERA("CAMERA");
const char *Scene::TAG_GAMEOBJECT("GAMEOBJECT");

Scene::Scene(const Ogre::String &Name) :
	m_SceneManager(NULL),
	m_DeferredShadingSystem(NULL),
	m_SkyMode(SM_SKY_BOX),
	m_AtmosphereManager(NULL),
	m_SkyBoxMaterial(""),
	m_SkyDomeMaterial(""),
	m_CameraCounter(0),
	//m_FirstUpdate(false),
	m_Stats(NULL),
	m_Name(Name),
	m_EntityCounter(0),
	m_LightCounter(0),
	m_ParticleCounter(0),
	m_GameObjectCounter(0)
	//m_DSS(NULL)
{
	m_SceneManager = Engine::GetInstance().GetGraphic()->createSceneManager(Ogre::ST_GENERIC, m_Name);
	//m_SceneManager = Engine::GetInstance().GetGraphic()->createSceneManager("TerrainSceneManager");

	m_DeferredShadingSystem = new DeferredShadingSystem(this);

	m_GameObjectFactory = new GameObjectFactory(this);
	
	m_RootGameObject = new GameObject("RootGameObject", this);

	m_SceneManager->setAmbientLight(Ogre::ColourValue::Black);

	m_AtmosphereManager = new AtmosphereManager(this);

	m_ShadowManager = new ShadowManager(this);

#if !defined(EDIT_MODE) && defined(DEBUG)
	InitializeStats();
#endif
}


Scene::~Scene(void)
{
#ifdef DEBUG
	delete m_Stats;
#endif

	m_Name = "";
	
	delete m_AtmosphereManager;

	m_SkyBoxMaterial = "";
	m_SkyDomeMaterial = "";

	m_PostProcessEffects.clear();

	DestroyGameObject(m_RootGameObject);

	delete m_DeferredShadingSystem;

	Engine::GetInstance().GetGraphic()->destroySceneManager(m_SceneManager);
}
	

void Scene::Start(void)
{
	//if (m_FirstUpdate)
	//{
	CallFunction("Start");

	bool IsNavGraphLoaded = false;
		
	if(GetNavGraphFileName().size() > 0)
	{
		while(!IsNavGraphLoaded)
		{
			IsNavGraphLoaded = GameMap::GetInstance().GetNavGraph()->IsLoaded();
		}
	}

	m_RootGameObject->Start();
}

	
bool Scene::Update(void)
{
	m_DeferredShadingSystem->Update();

	// Handle remove object queue
	BEGIN_VECTOR_ITERATOR(m_GameObjectRemoveQueue, GameObject*)
		RemoveGameObject(current);
	END_VECTOR_ITERATOR(NULL)
	m_GameObjectRemoveQueue.clear();
		
	// Handle destroy object queue
	BEGIN_VECTOR_ITERATOR(m_GameObjectDestroyQueue, GameObject*)
		//current->GetParent()->RemoveChild(current, true);
		DestroyGameObject(current);
	END_VECTOR_ITERATOR(NULL)
	m_GameObjectDestroyQueue.clear();

	//if (m_FirstUpdate)
	//{
	//	CallFunction("Start");
	//	m_FirstUpdate = false;
	//}
	//else
		CallFunction("Update");
		
	//ThreadHandler *t = ThreadingManager::GetInstance().CreateThread(&u);
	m_RootGameObject->Update();

#if defined(DEBUG)/* && !defined(EDIT_MODE)*/
	if (m_Stats)
		m_Stats->Update();
#endif

	return true;
}


bool Scene::Deserialize(Attributes *Attributes)
{
	SetNavGraphFileName(Attributes->GetString("NavGraphFileName"));
	if(GetNavGraphFileName().size() > 0)
	{
		SparseGraphThreadWorker* worker = new SparseGraphThreadWorker(FT_LOADGRAPH);
		worker->SetNavGraphFileName(GetNavGraphFileName());
		ThreadingManager::GetInstance().CreateThread(worker);
	}

	Scriptable::Deserialize(Attributes);
					
//#ifdef EDIT_MODE
//	ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport()->setBackgroundColour(att->GetColor("ClearColor"));
//#else
	ViewPortsManager::GetInstance().GetViewPort()->setBackgroundColour(Attributes->GetColor("ClearColor"));
//#endif
	m_DeferredShadingSystem->SetRenderMode((DeferredShadingSystem::RenderMode)Attributes->GetInteger("RenderMode"));
	m_SceneManager->setAmbientLight(Attributes->GetColor("AmbientLight"));

	//m_SceneManager->setShowDebugShadows(Attributes->GetBoolean("ShowDebugShadows"));
	//m_SceneManager->setShadowTechnique((Ogre::ShadowTechnique)Attributes->GetInteger("ShadowTechnique"));
	//m_SceneManager->setShadowColour(Attributes->GetColor("ShadowColor"));
	//m_SceneManager->setShadowDirectionalLightExtrusionDistance(Attributes->GetFloat("ShadowDirectionalLightExtrusionDistance"));
	//m_SceneManager->setShadowFarDistance(Attributes->GetFloat("ShadowFarDistance"));
	//m_SceneManager->setShadowTextureSize(Attributes->GetInteger("ShadowTextureSize"));
	//m_SceneManager->setShadowDirLightTextureOffset(Attributes->GetFloat("ShadowDirectionalLightTextureOffset"));
	//m_SceneManager->setShadowTextureSelfShadow(Attributes->GetBoolean("ShadowTextureSelfShadow"));
	//m_SceneManager->setShadowCasterRenderBackFaces(Attributes->GetBoolean("ShadowCasterRenderBackFaces"));

	SetSkyMode((SkyMode)Attributes->GetInteger("SkyMode"));
	Ogre::String materialName = "";
	switch (m_SkyMode)
	{
	case SM_SKY_BOX:
		{
			materialName = Attributes->GetString("SkyBoxMaterialName");
			if (materialName.size())
			{
				//SetSkyBoxEnabled(Attributes->GetBoolean("SkyBoxEnabled"));
				SetSkyBoxMaterialName(materialName);
				SetSkyBoxDistance(Attributes->GetFloat("SkyBoxDistance"));
			}
		} break;

	case SM_SKY_DOME:
		{
			materialName = Attributes->GetString("SkyDomeMaterialName");
			if (materialName.size())
			{
				SetSkyDomeEnabled(Attributes->GetBoolean("SkyDomeEnabled"));
				SetSkyDomeMaterialName(materialName);
				SetSkyDomeCurvature(Attributes->GetFloat("SkyDomeCurvature"));
				SetSkyDomeTiling(Attributes->GetFloat("SkyDomeTiling"));
				SetSkyDomeDistance(Attributes->GetFloat("SkyDomeDistance"));
			}
		} break;

	case SM_ADVANCED_SKY:
		{
			m_AtmosphereManager->Deserialize(Attributes);
		} break;
	}

	SetSunPosition(Attributes->GetVector3("SunPosition"));
	SetSunStrength(Attributes->GetFloat("SunStrength"));
	SetSunArea(Attributes->GetFloat("SunArea"));
	SetSunColor(Attributes->GetColor("SunColor"));

	int i = 0;
	Ogre::String Value = "";
	while (true)
	{
		Value = Attributes->GetString((Ogre::String("PPE") + Ogre::StringConverter::toString(i)).c_str());

		if (Value.size() == 0)
			break;

		m_PostProcessEffects.push_back(Value);

		i++;
	}

	return true;
}

#ifdef DEBUG
void Scene::InitializeStats(void)
{
	m_Stats = new Stats(ViewPortsManager::GetInstance().GetViewPort(), m_SceneManager);
}
#endif

	
void Scene::SetInitializeState(void)
{
	Scriptable::SetInitializeState();

	m_RootGameObject->SetInitializeState();
}


void Scene::ResetToInitializeState(void)
{
	Scriptable::ResetToInitializeState();

	m_RootGameObject->ResetToInitializeState();

	AIEngine::GetInstance().ResetToInitializeState();
}


void Scene::ResetToInitializeStateScriptUsage(void)
{
	ResetToInitializeState();
}


void Scene::Resume(void)
{
#ifndef EDIT_MODE
	ScenesHolder::GetInstance().SetCurrentScene(this);
#endif
}


void Scene::Destroy(void)
{
#ifndef EDIT_MODE
	ScenesHolder::GetInstance().DestroyScene(this);
#endif
}


Ogre::Entity *Scene::AddEntity(const Ogre::String MeshName)
{
	Ogre::Entity* ent = NULL;

	try
	{
		ent = m_SceneManager->createEntity(GetNewEntityName(MeshName), MeshName);

		//m_Entites.push_back(ent);
	}
	catch (const Ogre::Exception &Error)
	{
		Exception::GetInstance().Throw(Exception::ER_FILE_NOT_FOUND, "Loading model \"" + MeshName + "\"", Error.getDescription());
	}

	return ent;
}


GameObject *Scene::CreateEmptyGameObject(const Ogre::String &Name, const Ogre::Vector3 &Translate, const Ogre::Quaternion &Rotate/*, Ogre::Entity *Entity*/)
{
	GameObject *obj = m_RootGameObject->CreateChild(Name);

	obj->SetPosition(Translate);
	obj->SetOrientation(Rotate);
	
	//AttachEntityToGameObject(obj, Entity);
	//obj->AttachObject(Entity);//m_SelectedNode->detachObject(

	return obj;
}


MeshGameObject *Scene::CreateMeshGameObject(const Ogre::String &Name, const Ogre::String &MeshName, GameObject *Parent)
{
	return new MeshGameObject(Name, this, MeshName, (Parent ? Parent : m_RootGameObject));
}


CameraGameObject *Scene::CreateCameraGameObject(const Ogre::String &Name, GameObject *Parent)
{
	return new CameraGameObject(Name, this, (Parent ? Parent : m_RootGameObject));
}


LightGameObject *Scene::CreateLightGameObject(const Ogre::String &Name, GameObject *Parent)
{
	return new LightGameObject(Name, this, (Parent ? Parent : m_RootGameObject));
}


TerrainGameObject *Scene::CreateTerrainGameObject(const Ogre::String &Name, GameObject *Parent)
{
	return new TerrainGameObject(Name, this, (Parent ? Parent : m_RootGameObject));
}


OceanGameObject *Scene::CreateOceanGameObject(const Ogre::String &Name, GameObject *Parent)
{
	return new OceanGameObject(Name, this, (Parent ? Parent : m_RootGameObject));
}


ParticleGameObject *Scene::CreateParticleGameObject(const Ogre::String &Name, GameObject *Parent)
{
	return new ParticleGameObject(Name, this, (Parent ? Parent : m_RootGameObject));
}


//GameObject *Scene::AddGameObject(const Ogre::String &Name, const Ogre::Vector3 &Translate, const Ogre::Quaternion &Rotate, Ogre::Entity *Entity)
//{
//	GameObject *node = new GameObject(Name);
//
//	node->SetPosition(Translate);
//	node->SetOrientation(Rotate);
//
//	AttachEntityToGameObject(node, Entity);
//
//	m_GameObjects.push_back(node);
//
//#ifdef EDIT_MODE
//	RefreshSceneControlContent();
//#endif
//
//	return node;
//}


//void Scene::AttachEntityToGameObject(GameObject *Object, Ogre::MovableObject *Entity)
//{
//	if (Entity)
//		Object->AttachObject(Entity);
//
////#ifdef EDIT_MODE
////	RefreshSceneControlContent();
////#endif
//}


void Scene::AddToGameObjectRemoveQueue(GameObject *Object)
{
	if (Object)
		m_GameObjectRemoveQueue.push_back(Object);
}


void Scene::RemoveGameObject(GameObject *Object)
{
	if (Object)
	{
		Object->SetVisible(false);

		Object->ShowBoundingBox(false);

		Object->SetAsRemoved(true);
	}
}


void Scene::RemoveGameObject(unsigned int ID/*const Ogre::String &Name*/)
{
	RemoveGameObject(GetGameObject(ID));
}


void Scene::AddToGameObjectDestroyQueue(GameObject *Object)
{
	if (Object)
		m_GameObjectDestroyQueue.push_back(Object);
}


void Scene::DestroyGameObject(GameObject *Object)
{
	//if (Object != m_RootGameObject)
	//	Object->GetParent()->RemoveChild(Object, true);

	BEGIN_LIST_ITERATOR(Object->GetChildren(), GameObject*)
		DestroyGameObject(current);
	END_LIST_ITERATOR(NULL)
	Object->GetChildren().clear();

	Object->_GetSceneNode()->removeAndDestroyAllChildren();

	//switch (Object->GetType())
	//{
	//case GameObject::GOT_MESH:
	//	delete Object;
	//	break;
	//case GameObject::GOT_CAMERA:
	//	delete (CameraGameObject*)Object;
	//	break;
	//case GameObject::GOT_LIGHT:
	//	delete (LightGameObject*)Object;
	//	break;
	//case GameObject::GOT_TERRAIN:
	//	delete (TerrainGameObject*)Object;
	//	break;
	//}
	delete Object;
}


void Scene::DestroyGameObject(unsigned int ID/*const Ogre::String &Name*/)
{
	RemoveGameObject(GetGameObject(ID));
}


//void Scene::AddGameObjectToList(GameObject *Object)
//{
//	m_GameObjects.push_back(Object);
//
//#ifdef EDIT_MODE
//	RefreshSceneControlContent();
//#endif
//}


//void Scene::RemoveGameObjectFromList(GameObject *Object)
//{
	//BEGIN_LIST_ITERATOR(m_GameObjects, GameObject*)
	//	if (current == Object)
	//	{
	//		m_GameObjects.remove(current);
	//	}
	//END_LIST_ITERATOR(NULL)
	
//	m_GameObjects.remove(Object);
//
//#ifdef EDIT_MODE
//	RefreshSceneControlContent();
//#endif
//}


GameObject *Scene::GetGameObject(Ogre::SceneNode *Node)
{
	if (!Node)
		return NULL;

	GameObject *obj;

	BEGIN_LIST_ITERATOR(GetChildren(), GameObject*)
		obj = GetGameObject(Node, current);
	
		if (obj)
			return obj;
	END_LIST_ITERATOR(NULL)

	return NULL;
}


GameObject *Scene::GetGameObject(Ogre::SceneNode *Node, GameObject *Parent)
{
	GameObject *obj = NULL;

	if (Parent->_GetSceneNode() == Node)
		return Parent;

	BEGIN_LIST_ITERATOR(Parent->GetChildren(), GameObject*)
		if (current->_GetSceneNode() == Node)
		{
			return current;
		}

		obj = GetGameObject(Node, current);

		if (obj)
			return obj;

	END_LIST_ITERATOR(NULL)

	return NULL;
}


GameObject *Scene::GetGameObject(unsigned int ID)
{
	GameObject *obj;

	BEGIN_LIST_ITERATOR(GetChildren(), GameObject*)
		obj = GetGameObject(ID, current);
	
		if (obj)
			return obj;
	END_LIST_ITERATOR(NULL)

	return 0;
}


GameObject *Scene::GetGameObject(unsigned int ID, GameObject *Parent)
{
	GameObject *obj = NULL;

	//Ogre::String name = Name;

	//Ogre::StringUtil::trim(name);

	//if (Ogre::StringUtil::match(Parent->GetName(), name))
	//{
	//	return Parent;
	//}

	if (Parent->GetID() == ID)
		return Parent;

	BEGIN_LIST_ITERATOR(Parent->GetChildren(), GameObject*)
		//if (Ogre::StringUtil::match(current->GetName(), name))
		//{
		//	return current;
		//}
		
		//if (current->GetID() == ID)
		//	return current;

		obj = GetGameObject(ID, current);

		if (obj)
			return obj;

	END_LIST_ITERATOR(NULL)

	return 0;
}


GameObject *Scene::GetGameObject(const Ogre::String &Name)
{
	GameObject *obj;

	BEGIN_LIST_ITERATOR(GetChildren(), GameObject*)
		obj = GetGameObject(Name, current);
	
		if (obj)
			return obj;
	END_LIST_ITERATOR(NULL)

	return 0;
}


GameObject *Scene::GetGameObject(const Ogre::String &Name, GameObject *Parent)
{
	GameObject *obj = NULL;

	if (Parent->GetName() == Name)
		return Parent;

	BEGIN_LIST_ITERATOR(Parent->GetChildren(), GameObject*)
		obj = GetGameObject(Name, current);

		if (obj)
			return obj;
	END_LIST_ITERATOR(NULL)

	return 0;
}


void Scene::GetGameObjectsByName(const Ogre::String &Name, std::list<GameObject*> &List)
{
	std::list<GameObject*> objs;
	GameObject* obj;

	BEGIN_LIST_ITERATOR(GetChildren(), GameObject*)
		obj = GetGameObject(Name, current);
	
		if (obj)
			List.push_back(obj);
	END_LIST_ITERATOR(NULL)
}


std::list<GameObject*> &Scene::GetChildren(void)
{
	return m_RootGameObject->GetChildren();
}


CameraGameObject *Scene::GetCamera(Ogre::Camera *Camera)
{
	std::list<CameraGameObject*> list = GetCameras();

	BEGIN_LIST_ITERATOR(list, CameraGameObject*)
		if (current->_GetCamera() == Camera)
			return current;
	END_LIST_ITERATOR(NULL)

	return NULL;
}


std::list<CameraGameObject*> Scene::GetCameras(void)
{
	std::list<CameraGameObject*> list;

	GetCamerasCore(list, m_RootGameObject);

	return list;
}


std::list<TerrainGameObject*> Scene::GetTerrains(void)
{
	std::list<TerrainGameObject*> list;

	GetTerrainsCore(list, m_RootGameObject);

	return list;
}


unsigned int Scene::GetNewGameObjectID(void)
{
	return (m_GameObjectCounter++);
}


Ogre::String Scene::GetNewEntityName(const Ogre::String &MeshName)
{
	return MeshName + "|" + Ogre::StringConverter::toString(m_EntityCounter++);
	//return Ogre::String("Mesh") + Ogre::StringConverter::toString(m_EntityCounter++);
}


Ogre::String Scene::GetNewCameraName(void)
{
	return "Camera|" + Ogre::StringConverter::toString(m_CameraCounter++);
}


Ogre::String Scene::GetNewLightName(void)
{
	return "Light|" + Ogre::StringConverter::toString(m_LightCounter++);
}


Ogre::String Scene::GetNewParticleName(void)
{
	return "Particle|" + Ogre::StringConverter::toString(m_ParticleCounter++);
}


Ogre::String Scene::GetNewSceneNodeName(unsigned int ObjectID)
{
	return Ogre::String("SceneNode") + Ogre::StringConverter::toString(ObjectID);
}


bool Scene::GetSkyBoxEnabled(void)
{
	return m_SceneManager->isSkyBoxEnabled();
}


void Scene::SetSkyBoxEnabled(bool Enabled)
{
	if (m_SkyBoxMaterial.size())
		m_SceneManager->setSkyBox(Enabled, m_SkyBoxMaterial, m_SceneManager->getSkyBoxGenParameters().skyBoxDistance, true, Ogre::Quaternion::IDENTITY, Engine::RESOURCES_CORE_MATERIAL);
	else
		m_SceneManager->setSkyBoxEnabled(Enabled);
}


const Ogre::String &Scene::GetSkyBoxMaterialName(void) const
{
	return m_SkyBoxMaterial;
}


void Scene::SetSkyBoxMaterialName(Ogre::String MaterialName)
{
	m_SkyBoxMaterial = MaterialName;

	if (m_SkyBoxMaterial.size())
	{
		m_SceneManager->setSkyBox(m_SceneManager->isSkyBoxEnabled(), m_SkyBoxMaterial, m_SceneManager->getSkyBoxGenParameters().skyBoxDistance, true, Ogre::Quaternion::IDENTITY, Engine::RESOURCES_CORE_MATERIAL);
	}
	else
	{
		m_SceneManager->setSkyBoxEnabled(false);
	}
}


float Scene::GetSkyBoxDistance(void)
{
	return m_SceneManager->getSkyBoxGenParameters().skyBoxDistance;
}


void Scene::SetSkyBoxDistance(float Value)
{
	if (m_SkyBoxMaterial.size())
	{
		m_SceneManager->setSkyBox(m_SceneManager->isSkyBoxEnabled(), m_SkyBoxMaterial, Value, true, Ogre::Quaternion::IDENTITY, Engine::RESOURCES_CORE_MATERIAL);
	}
}


bool Scene::GetSkyDomeEnabled(void)
{
	return m_SceneManager->isSkyDomeEnabled();
}


void Scene::SetSkyDomeEnabled(bool Enabled)
{
	if (m_SkyDomeMaterial.size())
		m_SceneManager->setSkyDome(Enabled, m_SkyDomeMaterial, 
			m_SceneManager->getSkyDomeGenParameters().skyDomeCurvature,
			m_SceneManager->getSkyDomeGenParameters().skyDomeTiling,
			m_SceneManager->getSkyDomeGenParameters().skyDomeDistance, 
			true, Ogre::Quaternion::IDENTITY, 16, 16, -1, Engine::RESOURCES_CORE_MATERIAL);
	else
		m_SceneManager->setSkyDomeEnabled(Enabled);
}


const Ogre::String &Scene::GetSkyDomeMaterialName(void) const
{
	return m_SkyDomeMaterial;
}


void Scene::SetSkyDomeMaterialName(Ogre::String MaterialName)
{
	m_SkyDomeMaterial = MaterialName;

	if (m_SkyDomeMaterial.size())
	{
		m_SceneManager->setSkyDome(m_SceneManager->isSkyDomeEnabled(), m_SkyDomeMaterial, 
			m_SceneManager->getSkyDomeGenParameters().skyDomeCurvature,
			m_SceneManager->getSkyDomeGenParameters().skyDomeTiling,
			m_SceneManager->getSkyDomeGenParameters().skyDomeDistance, 
			true, Ogre::Quaternion::IDENTITY, 16, 16, -1, Engine::RESOURCES_CORE_MATERIAL);
	}
	else
	{
		m_SceneManager->setSkyDomeEnabled(false);
	}
}


float Scene::GetSkyDomeCurvature(void)
{
	return m_SceneManager->getSkyDomeGenParameters().skyDomeCurvature;
}


void Scene::SetSkyDomeCurvature(float Value)
{
	if (m_SkyDomeMaterial.size())
	{
		m_SceneManager->setSkyDome(m_SceneManager->isSkyDomeEnabled(), m_SkyDomeMaterial, Value,
		m_SceneManager->getSkyDomeGenParameters().skyDomeTiling,
		m_SceneManager->getSkyDomeGenParameters().skyDomeDistance, 
			true, Ogre::Quaternion::IDENTITY, 16, 16, -1, Engine::RESOURCES_CORE_MATERIAL);
	}
}


float Scene::GetSkyDomeTiling(void)
{
	return m_SceneManager->getSkyDomeGenParameters().skyDomeTiling;
}


void Scene::SetSkyDomeTiling(float Value)
{
	if (m_SkyDomeMaterial.size())
	{
		m_SceneManager->setSkyDome(m_SceneManager->isSkyDomeEnabled(), m_SkyDomeMaterial, 
			m_SceneManager->getSkyDomeGenParameters().skyDomeCurvature,
			Value,
			m_SceneManager->getSkyDomeGenParameters().skyDomeDistance, 
			true, Ogre::Quaternion::IDENTITY, 16, 16, -1, Engine::RESOURCES_CORE_MATERIAL);
	}
}


float Scene::GetSkyDomeDistance(void)
{
	return m_SceneManager->getSkyDomeGenParameters().skyDomeDistance;
}


void Scene::SetSkyDomeDistance(float Value)
{
	if (m_SkyDomeMaterial.size())
	{
		m_SceneManager->setSkyDome(m_SceneManager->isSkyDomeEnabled(), m_SkyDomeMaterial, 
			m_SceneManager->getSkyDomeGenParameters().skyDomeCurvature,
			m_SceneManager->getSkyDomeGenParameters().skyDomeTiling,
			Value, 
			true, Ogre::Quaternion::IDENTITY, 16, 16, -1, Engine::RESOURCES_CORE_MATERIAL);
	}
}


void Scene::Clear(void)
{
	m_EntityCounter = NULL;
	m_CameraCounter = NULL;
	m_LightCounter = NULL;
	m_ParticleCounter = NULL;
	m_GameObjectCounter = NULL;

	m_GameObjectRemoveQueue.clear();
	m_GameObjectDestroyQueue.clear();

#ifndef EDIT_MODE
	ViewPortsManager::GetInstance().GetViewPort()->setCamera(NULL);
#endif

	BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
		//switch (current->GetType())
		//{
		//case GameObject::GOT_MESH:
		//	{
		//		delete (MeshGameObject*)current;
		//	} break;
		//	
		//case GameObject::GOT_CAMERA:
		//	{
		//		delete (CameraGameObject*)current;
		//	} break;
		//	
		//case GameObject::GOT_LIGHT:
		//	{
		//		delete (LightGameObject*)current;
		//	} break;
		//	
		//case GameObject::GOT_TERRAIN:
		//	{
		//		delete (TerrainGameObject*)current;
		//	} break;
		//	
		//case GameObject::GOT_OCEAN:
		//	{
		//		delete (OceanGameObject*)current;
		//	} break;
		//	
		//case GameObject::GOT_PARTICLE:
		//	{
		//		delete (ParticleGameObject*)current;
		//	} break;

		//default:
		//	{
				delete current;
		//	} break;
		//}
	END_LIST_ITERATOR(NULL)
	m_RootGameObject->GetChildren().clear();

	ClearScripts();
	
	ViewPortsManager::GetInstance().GetViewPort()->setBackgroundColour(Ogre::ColourValue::Black);

	m_SceneManager->setAmbientLight(Ogre::ColourValue::Black);

	//m_SceneManager->setShowDebugShadows(false);
	//m_SceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	//m_SceneManager->setShadowColour(Ogre::ColourValue::Black);
	//m_SceneManager->setShadowDirectionalLightExtrusionDistance(10000);
	//m_SceneManager->setShadowFarDistance(0);
	//m_SceneManager->setShadowTextureSize(512);
	//m_SceneManager->setShadowDirLightTextureOffset(0.6f);
	//m_SceneManager->setShadowTextureSelfShadow(false);
	//m_SceneManager->setShadowCasterRenderBackFaces(true);

	m_SkyBoxMaterial = "";
	m_SceneManager->setSkyBox(false, "");
	m_SkyDomeMaterial = "";
	m_SceneManager->setSkyDome(false, "");

	m_PostProcessEffects.clear();

	GameMap::GetInstance().Drop();
}


bool Scene::ConnectFirstCamera(void)
{
	CameraGameObject *camera = GetSuitableCameraToStart();

	if (camera)
	{
		ViewPortsManager::GetInstance().GetViewPort()->setCamera(camera->_GetCamera());
		return true;
	}

	return false;
}


void Scene::HandleViewportChange(void)
{
#ifndef EDIT_MODE
	Ogre::Viewport *vp = ViewPortsManager::GetInstance().GetViewPort();
	
	BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
		if (current->GetType() == GameObject::GOT_OCEAN)
			((OceanGameObject*)current)->OnViewportChanges(vp);
	END_LIST_ITERATOR(NULL)
	
	if (vp && vp->getCamera())
		m_DeferredShadingSystem->OnViewportChanges(vp);

	vp = NULL;

	m_AtmosphereManager->OnViewportChanges(ViewPortsManager::GetInstance().GetRenderWindow());
#endif
}


void Scene::InitializePostProcessEffects(void)
{
//	Ogre::CompositorManager::getSingleton().removeCompositorChain(
//#ifdef EDIT_MODE
//		ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()
//#else
//		ViewPortsManager::GetInstance().GetViewPort()
//#endif
//		);

	Ogre::Viewport *vp = 
#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()
#else
		ViewPortsManager::GetInstance().GetViewPort()
#endif
		;

	std::vector<Ogre::String> names;
	Ogre::CompositorChain::InstanceIterator it = Ogre::CompositorManager::getSingleton().getCompositorChain(vp)->getCompositors();
	Ogre::String name;
	while(it.hasMoreElements())
	{
		name = it.getNext()->getCompositor()->getName();

		if (!Ogre::StringUtil::startsWith(name, "_Hydrax", false) && 
			!Ogre::StringUtil::endsWith(name, "_DSS", false))
			names.push_back(name);
	}

	BEGIN_VECTOR_ITERATOR(names, Ogre::String)
		Ogre::CompositorManager::getSingleton().removeCompositor(vp, current);
	END_VECTOR_ITERATOR("")
	names.clear();

	if (vp->getCamera())
		BEGIN_VECTOR_ITERATOR(m_PostProcessEffects, Ogre::String)
			AddPostProcessEffectCore(current, true);
		END_VECTOR_ITERATOR("")
}


void Scene::SetPostProcessEffectEnabled(const Ogre::String &Name, bool Enabled)
{
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(
#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()
#else
		ViewPortsManager::GetInstance().GetViewPort()
#endif
		, Name, Enabled);
}


void Scene::RemovePostProcessEffectCore(const Ogre::String &Name)
{
	Ogre::CompositorManager::getSingleton().removeCompositor(
#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()
#else
		ViewPortsManager::GetInstance().GetViewPort()
#endif
		, Name);
}


Ogre::CompositorInstance *Scene::AddPostProcessEffectCore(const Ogre::String &Name)
{
	return AddPostProcessEffectCore(Name, false);
}


Ogre::CompositorInstance *Scene::AddPostProcessEffectCore(const Ogre::String &Name, bool Enabled)
{
	return AddPostProcessEffectCore(Name, Enabled, -1);
}


Ogre::CompositorInstance *Scene::AddPostProcessEffectCore(const Ogre::String &Name, int Index)
{
	return AddPostProcessEffectCore(Name, false, Index);
}


Ogre::CompositorInstance *Scene::AddPostProcessEffectCore(const Ogre::String &Name, bool Enabled, int Index)
{
	Ogre::CompositorInstance *com = Ogre::CompositorManager::getSingleton().addCompositor(
#ifdef EDIT_MODE
		ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()
#else
		ViewPortsManager::GetInstance().GetViewPort()
#endif
		, Name, Index);

	if (Enabled)
		com->setEnabled(true);

	return com;
}


void Scene::Load(const Ogre::String &FilePath)
{
	TreeParser *tp = new TreeParser(FilePath, TAG_LEVEL);

	BOOST_FOREACH(const TreeElement *elem, tp->GetRoot()->Children)
	{
		if (elem->Name == TAG_ATTRIBUTES)
		{
			Attributes *att = new Attributes;
			att->Read(elem);

			Deserialize(att);

			delete att;
			att = NULL;
		}
		else if (elem->Name == TAG_GAMEOBJECT)
		{
			ReadGameObject(elem, m_RootGameObject);
		}
	}

	delete tp;
}


void Scene::SetSkyMode(const SkyMode &Mode)
{
	m_SkyMode = Mode;

	SetSkyBoxEnabled(Mode == SM_SKY_BOX);
	SetSkyDomeEnabled(Mode == SM_SKY_DOME);
	m_AtmosphereManager->SetVisible(Mode == SM_ADVANCED_SKY);
}

void Scene::SetSunPosition(const Ogre::Vector3 &SunPosition)
{
	m_SunPosition = SunPosition;

	BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
		if (current->GetType() == GameObject::GOT_OCEAN)
			((OceanGameObject*)current)->SetSunPosition(m_SunPosition);
	END_LIST_ITERATOR(NULL)
}

void Scene::SetSunStrength(const float &SunStrength)
{
	m_SunStrength = SunStrength;

	BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
		if (current->GetType() == GameObject::GOT_OCEAN)
			((OceanGameObject*)current)->SetSunStrength(m_SunStrength);
	END_LIST_ITERATOR(NULL)
}

void Scene::SetSunArea(const float &SunArea)
{
	m_SunArea = SunArea;

	BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
		if (current->GetType() == GameObject::GOT_OCEAN)
			((OceanGameObject*)current)->SetSunArea(m_SunArea);
	END_LIST_ITERATOR(NULL)
}

void Scene::SetSunColor(const Ogre::ColourValue &SunColor)
{
	m_SunColor = SunColor;
	
	BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
		if (current->GetType() == GameObject::GOT_OCEAN)
			((OceanGameObject*)current)->SetSunColor(m_SunColor);
	END_LIST_ITERATOR(NULL)
}

void Scene::SetSunColor(const float &R, const float &G, const float &B)
{
	SetSunColor(Ogre::ColourValue(R, G, B));
}


void Scene::GetCamerasCore(std::list<CameraGameObject*> &List, GameObject *Object)
{
	BEGIN_LIST_ITERATOR(Object->GetChildren(), GameObject*)
	if (current->GetType() == GameObject::GOT_CAMERA)
		List.push_back((CameraGameObject*)current);
	END_LIST_ITERATOR(NULL)
}


CameraGameObject *Scene::GetSuitableCameraToStart(void)
{
	std::list<CameraGameObject*> list =	GetCameras();

	if (list.size())
	{
		bool found = false;

		BEGIN_LIST_ITERATOR(list, CameraGameObject*)
			if (current->GetIsMain())
				return current;
		END_LIST_ITERATOR(NULL)

		return (*list.begin());
	}

	return NULL;
}


void Scene::GetTerrainsCore(std::list<TerrainGameObject*> &List, GameObject *Object)
{
	BEGIN_LIST_ITERATOR(Object->GetChildren(), GameObject*)
	if (current->GetType() == GameObject::GOT_TERRAIN)
		List.push_back((TerrainGameObject*)current);
	END_LIST_ITERATOR(NULL)
}


void Scene::ReadGameObject(const TreeElement *Element, GameObject *Parent)
{
	Attributes *att = NULL;
	GameObject *obj = NULL;
	
	const std::string type = Element->GetAttributeValue("Type");
	const std::string name = Element->GetAttributeValue("Name");

	obj = m_GameObjectFactory->CreateGameObject(type, name, Parent);

	if (!obj)
	{
		Log::GetInstance().AddError("There's no GameObjectType as [" + type + "] in GameObjectFactory");

		return;
	}

	if (obj->GetType() == GameObject::GOT_EMPTY)
	{
		obj->GetParent()->RemoveChild(obj);
		obj->SetParent(Parent);
		Parent->AddChild(obj);
	}

	BOOST_FOREACH(const TreeElement *elem, Element->Children)
	{
		if (elem->Name == TAG_ATTRIBUTES)
		{
			att = new Attributes;
			att->Read(elem);
			obj->Deserialize(att);

			delete att;
			att = NULL;
		}
		else if (elem->Name == TAG_GAMEOBJECT)
		{
			ReadGameObject(elem, obj);
		}
	}
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(Scene)
	BEGIN_DEFINE_TYPE(Scene, "Scene")
		DEFINE_FUNCTION("ResetToInitializeState", &Scene::ResetToInitializeStateScriptUsage)

		DEFINE_FUNCTION("Resume", &Scene::Resume)
		DEFINE_FUNCTION("Destroy", &Scene::Destroy)

		DEFINE_FUNCTION("GetGameObject", (GameObject*(Scene::*)(const Ogre::String&))&Scene::GetGameObject)
		DEFINE_FUNCTION("GetGameObject", (GameObject*(Scene::*)(const Ogre::String&, GameObject *Parent))&Scene::GetGameObject)

		DEFINE_FUNCTION("GetSkyBoxEnabled", &Scene::GetSkyBoxEnabled)
		DEFINE_FUNCTION("SetSkyBoxEnabled", &Scene::SetSkyBoxEnabled)
		
		DEFINE_FUNCTION("GetSkyBoxMaterialName", &Scene::GetSkyBoxMaterialName)
		DEFINE_FUNCTION("SetSkyBoxMaterialName", &Scene::SetSkyBoxMaterialName)
		
		DEFINE_FUNCTION("GetSkyBoxDistance", &Scene::GetSkyBoxDistance)
		DEFINE_FUNCTION("SetSkyBoxDistance", &Scene::SetSkyBoxDistance)
		
		DEFINE_FUNCTION("GetSkyDomeEnabled", &Scene::GetSkyDomeEnabled)
		DEFINE_FUNCTION("SetSkyDomeEnabled", &Scene::SetSkyDomeEnabled)
		
		DEFINE_FUNCTION("GetSkyDomeMaterialName", &Scene::GetSkyDomeMaterialName)
		DEFINE_FUNCTION("SetSkyDomeMaterialName", &Scene::SetSkyDomeMaterialName)
		
		DEFINE_FUNCTION("GetSkyDomeCurvature", &Scene::GetSkyDomeCurvature)
		DEFINE_FUNCTION("SetSkyDomeCurvature", &Scene::SetSkyDomeCurvature)
		
		DEFINE_FUNCTION("GetSkyDomeTiling", &Scene::GetSkyDomeTiling)
		DEFINE_FUNCTION("SetSkyDomeTiling", &Scene::SetSkyDomeTiling)
		
		DEFINE_FUNCTION("GetSkyDomeDistance", &Scene::GetSkyDomeDistance)
		DEFINE_FUNCTION("SetSkyDomeDistance", &Scene::SetSkyDomeDistance)
		
		DEFINE_FUNCTION("SetPostProcessEffectEnabled", &Scene::SetPostProcessEffectEnabled)
		DEFINE_FUNCTION("RemovePostProcessEffect", &Scene::RemovePostProcessEffectCore)
		DEFINE_FUNCTION("AddPostProcessEffect", (Ogre::CompositorInstance*(Scene::*)(const Ogre::String&))&Scene::AddPostProcessEffectCore)
		DEFINE_FUNCTION("AddPostProcessEffect", (Ogre::CompositorInstance*(Scene::*)(const Ogre::String&, bool))&Scene::AddPostProcessEffectCore)
		DEFINE_FUNCTION("AddPostProcessEffect", (Ogre::CompositorInstance*(Scene::*)(const Ogre::String&, int))&Scene::AddPostProcessEffectCore)
		DEFINE_FUNCTION("AddPostProcessEffect", (Ogre::CompositorInstance*(Scene::*)(const Ogre::String&, bool, int))&Scene::AddPostProcessEffectCore)

		DEFINE_FUNCTION("GetName", &Scene::GetName)
		DEFINE_FUNCTION("GetRootGameObject", &Scene::GetRootGameObject)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE