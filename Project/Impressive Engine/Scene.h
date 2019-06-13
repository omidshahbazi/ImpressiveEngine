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

#include "StdAfxOGRE.h"
#include "NodeBase.h"
#include "Scriptable.h"
#include "ScriptRegister.h"
#include "AtmosphereManager.h"
#include "ShadowManager.h"


BEGIN_NAMESPACE
#ifdef EDIT_MODE
#define VIRTUAL_MARK virtual
#else
#define VIRTUAL_MARK
#endif

#ifdef EDIT_MODE
#define ACCESS_MARK protected:
#else
#define ACCESS_MARK private:
#endif


#ifdef DEBUG
class Stats;
#endif


class DeferredShadingSystem;
class GameObjectFactory;
class GameObject;
class MeshGameObject;
class CameraGameObject;
class LightGameObject;
class TerrainGameObject;
class OceanGameObject;
class ParticleGameObject;
struct TreeElement;

class Scene : public NodeBase, public Scriptable/*, Ogre::RenderSystem::Listener*/
{
public:
	enum SkyMode
	{
		SM_SKY_BOX = 0,
		SM_SKY_DOME,
		SM_ADVANCED_SKY
	};

public:
	Scene(const Ogre::String &Name);

	~Scene(void);
	
	void Start(void);
	bool Update(void);

	bool Deserialize(Attributes *Attributes);

#ifdef DEBUG
	VIRTUAL_MARK void InitializeStats(void);
#endif
	
	//void eventOccurred(const Ogre::String &EventName, const Ogre::NameValuePairList *Parameters = 0);
	
	void SetInitializeState(void);
	void ResetToInitializeState(void);
	VIRTUAL_MARK void ResetToInitializeStateScriptUsage(void);

	VIRTUAL_MARK void Resume(void);

	VIRTUAL_MARK void Destroy(void);
	
	Ogre::Entity *AddEntity(const Ogre::String MeshName);
	GameObject *CreateEmptyGameObject(const Ogre::String &Name, const Ogre::Vector3 &Translate = Ogre::Vector3::ZERO, const Ogre::Quaternion &Rotate = Ogre::Quaternion::IDENTITY/*, Ogre::Entity *Entity = NULL*/);
	MeshGameObject *CreateMeshGameObject(const Ogre::String &Name, const Ogre::String &MeshName = "", GameObject *Parent = NULL);
	CameraGameObject *CreateCameraGameObject(const Ogre::String &Name, GameObject *Parent = NULL);
	LightGameObject *CreateLightGameObject(const Ogre::String &Name, GameObject *Parent = NULL);
	TerrainGameObject *CreateTerrainGameObject(const Ogre::String &Name, GameObject *Parent = NULL);
	OceanGameObject *CreateOceanGameObject(const Ogre::String &Name, GameObject *Parent = NULL);
	ParticleGameObject *CreateParticleGameObject(const Ogre::String &Name, GameObject *Parent = NULL);
	
	//GameObject *AddGameObject(const Ogre::String &Name, const Ogre::Vector3 &Translate = Ogre::Vector3::ZERO, const Ogre::Quaternion &Rotate = Ogre::Quaternion::IDENTITY, Ogre::Entity *Entity = NULL);
	//void AttachEntityToGameObject(GameObject *Object, Ogre::MovableObject *Entity);
	
	void AddToGameObjectRemoveQueue(GameObject *Object);

	VIRTUAL_MARK void RemoveGameObject(GameObject *Object);
	void RemoveGameObject(unsigned int ID/*const Ogre::String &Name*/);

	void AddToGameObjectDestroyQueue(GameObject *Object);

	VIRTUAL_MARK void DestroyGameObject(GameObject *Object);
	void DestroyGameObject(unsigned int ID/*const Ogre::String &Name*/);

	//void AddGameObjectToList(GameObject *Object);
	//void RemoveGameObjectFromList(GameObject *Object);
	
	GameObject *GetGameObject(Ogre::SceneNode *Node);
	GameObject *GetGameObject(Ogre::SceneNode *Node, GameObject *Parent);
	GameObject *GetGameObject(unsigned int ID/*const Ogre::String &Name*/);
	GameObject *GetGameObject(unsigned int ID/*const Ogre::String &Name*/, GameObject *Parent);
	GameObject *GetGameObject(const Ogre::String &Name);
	GameObject *GetGameObject(const Ogre::String &Name, GameObject *Parent);

	void GetGameObjectsByName(const Ogre::String &Name, std::list<GameObject*> &List);

	std::list<GameObject*> &GetChildren(void);
	
	CameraGameObject *GetCamera(Ogre::Camera *Camera);
	std::list<CameraGameObject*> GetCameras(void);
	std::list<TerrainGameObject*> GetTerrains(void);
	
	unsigned int GetNewGameObjectID(void);
	Ogre::String GetNewEntityName(const Ogre::String &MeshName);
	Ogre::String GetNewCameraName(void);
	Ogre::String GetNewLightName(void);
	Ogre::String GetNewParticleName(void);
	Ogre::String GetNewSceneNodeName(unsigned int ObjectID);
	
	bool GetSkyBoxEnabled(void);
	void SetSkyBoxEnabled(bool Enabled);

	const Ogre::String &GetSkyBoxMaterialName(void) const;
	void SetSkyBoxMaterialName(Ogre::String MaterialName);

	float GetSkyBoxDistance(void);
	void SetSkyBoxDistance(float Value);
	
	bool GetSkyDomeEnabled(void);
	void SetSkyDomeEnabled(bool Enabled);

	const Ogre::String &GetSkyDomeMaterialName(void) const;
	void SetSkyDomeMaterialName(Ogre::String MaterialName);

	float GetSkyDomeCurvature(void);
	void SetSkyDomeCurvature(float Value);

	float GetSkyDomeTiling(void);
	void SetSkyDomeTiling(float Value);

	float GetSkyDomeDistance(void);
	void SetSkyDomeDistance(float Value);

	VIRTUAL_MARK void Clear(void);

	VIRTUAL_MARK bool ConnectFirstCamera(void);
	VIRTUAL_MARK void HandleViewportChange(void);

	void InitializePostProcessEffects(void);
	void SetPostProcessEffectEnabled(const Ogre::String &Name, bool Enabled);
	void RemovePostProcessEffectCore(const Ogre::String &Name);
	Ogre::CompositorInstance *AddPostProcessEffectCore(const Ogre::String &Name);
	Ogre::CompositorInstance *AddPostProcessEffectCore(const Ogre::String &Name, bool Enabled);
	Ogre::CompositorInstance *AddPostProcessEffectCore(const Ogre::String &Name, int Index);
	Ogre::CompositorInstance *AddPostProcessEffectCore(const Ogre::String &Name, bool Enabled, int Index);
	
	VIRTUAL_MARK void Load(const Ogre::String &FilePath);

	const SkyMode &GetSkyMode(void)
	{
		return m_SkyMode;
	}
	
	void SetSkyMode(const SkyMode &Mode);
	
	AtmosphereManager *GetAtmosphereManager(void)
	{
		return m_AtmosphereManager;
	}
	
	ShadowManager *GetShadowManager(void)
	{
		return m_ShadowManager;
	}

	const Ogre::Vector3 &GetSunPosition(void)
	{
		return m_SunPosition;
	}

	void SetSunPosition(const Ogre::Vector3 &SunPosition);

	const float &GetSunStrength(void)
	{
		return m_SunStrength;
	}

	void SetSunStrength(const float &SunStrength);

	const float &GetSunArea(void)
	{
		return m_SunArea;
	}

	void SetSunArea(const float &SunArea);

	const Ogre::ColourValue GetSunColor(void)
	{
		return m_SunColor;
	}

	void SetSunColor(const Ogre::ColourValue &SunColor);
	void SetSunColor(const float &R, const float &G, const float &B);

	const Ogre::String &GetName(void)
	{
		return m_Name;
	}

	void SetName(Ogre::String Name)
	{
		m_Name = Name;
	}

	Ogre::SceneManager *GetSceneManager(void)
	{
		return m_SceneManager;
	}

	DeferredShadingSystem *GetDeferredShadingSystem(void) const
	{
		return m_DeferredShadingSystem;
	}

	const std::vector<Ogre::String> &GetPostProcessEffects(void) const
	{
		return m_PostProcessEffects;
	}

	GameObject *GetRootGameObject(void)
	{
		return m_RootGameObject;
	}

	const Ogre::String &GetNavGraphFileName(void) const
	{
		return m_NavGraphFileName;
	}

	void SetNavGraphFileName(const Ogre::String &FileName)
	{
		m_NavGraphFileName = FileName;
	}

	DECLARE_SCRIPT_REGISTATION()

ACCESS_MARK
	void GetCamerasCore(std::list<CameraGameObject*> &List, GameObject *Object);
	CameraGameObject *GetSuitableCameraToStart(void);
	void GetTerrainsCore(std::list<TerrainGameObject*> &List, GameObject *Object);

	void ReadGameObject(const TreeElement *Element, GameObject *Parent);
	
public:
	static const char *TAG_SCENE;
	static const char *TAG_LEVEL;
	static const char *TAG_ATTRIBUTES;
	static const char *TAG_CAMERA;
	static const char *TAG_GAMEOBJECT;

ACCESS_MARK
#ifdef DEBUG
	Stats *m_Stats;
#endif

	Ogre::String m_Name;

	Ogre::SceneManager *m_SceneManager;

	DeferredShadingSystem *m_DeferredShadingSystem;

	GameObjectFactory *m_GameObjectFactory;
	
	SkyMode m_SkyMode;

	AtmosphereManager *m_AtmosphereManager;
	ShadowManager *m_ShadowManager;

	Ogre::Vector3 m_SunPosition;
	float m_SunStrength;
	float m_SunArea;
	Ogre::ColourValue m_SunColor;

	Ogre::String m_SkyBoxMaterial;
	Ogre::String m_SkyDomeMaterial;

	std::vector<Ogre::String> m_PostProcessEffects;

	unsigned int m_EntityCounter;
	unsigned int m_CameraCounter;
	unsigned int m_LightCounter;
	unsigned int m_ParticleCounter;
	unsigned int m_GameObjectCounter;
	GameObject *m_RootGameObject;

	std::vector<GameObject*> m_GameObjectRemoveQueue;
	std::vector<GameObject*> m_GameObjectDestroyQueue;

	//bool m_FirstUpdate;

	Ogre::String m_NavGraphFileName;
};
END_NAMESPACE