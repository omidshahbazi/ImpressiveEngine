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
#include "MeshGameObject.h"
#include "Scene.h"
#include "Time.h"

#ifdef EDIT_MODE
#include "Editor.h"
#endif


BEGIN_NAMESPACE
MeshGameObject::MeshGameObject(const Ogre::String &Name, Scene *Holder, const Ogre::String &MeshName, GameObject *Parent) :
	GameObject(Name, Holder, Parent),
	m_Entity(NULL)
{
	if (MeshName.size())
		SetEntity(MeshName);
}


MeshGameObject::~MeshGameObject(void)
{
	m_Holder->GetSceneManager()->destroyEntity(m_Entity);
	
	m_Init_MaterialNames.clear();
}

	
bool MeshGameObject::Update(void)
{
	if (GetWasRemove())
		return false;

	if (GetAllAnimationStates())
	{
		Ogre::AnimationState *state = NULL;
		Ogre::AnimationStateIterator it = GetAllAnimationStates()->getAnimationStateIterator();

		while (it.hasMoreElements())
		{
			state = it.getNext();

			if (state->getEnabled())
				state->addTime(Time::GetInstance().GetTimeSinceLastUpdate() * Time::GetInstance().GetTimeScale() / 1000.f);
		}
		state = NULL;
	}

	return GameObject::Update();
}
	
#ifdef EDIT_MODE
bool MeshGameObject::Serialize(Attributes *Attributes)
{
	GameObject::Serialize(Attributes);
	
	Attributes->AddString("MeshName", Ogre::StringUtil::split(m_Entity->getName(), "|")[0]);

	Attributes->AddBoolean("CastShadows", GetCastShadows());

	return true;
}
#endif

bool MeshGameObject::Deserialize(Attributes *Attributes)
{
	SetEntity(Attributes->GetString("MeshName"));

	SetCastShadows(Attributes->GetBoolean("CastShadows"));
	
	GameObject::Deserialize(Attributes);

	return true;
}

	
void MeshGameObject::SetInitializeState(void)
{
	GameObject::SetInitializeState();

	Ogre::SubEntity *subEntity = NULL;
	for (unsigned int j = 0; j < m_Entity->getNumSubEntities(); j++)
	{
		subEntity = m_Entity->getSubEntity(j);
		m_Init_MaterialNames.push_back(MaterialNamePair(subEntity, subEntity->getMaterialName()));
	}
	subEntity = NULL;

	Ogre::AnimationStateSet *set = m_Entity->getAllAnimationStates();
	Ogre::AnimationState *state = NULL;
	AnimationState *aState = NULL;
	if (set)
	{
		Ogre::AnimationStateIterator it = set->getAnimationStateIterator();

		while (it.hasMoreElements())
		{
			state = it.getNext();

			aState = new AnimationState();
			aState->Animation = state;
			aState->TimePosition = state->getTimePosition();
			aState->Length = state->getLength();
			aState->Weight = state->getWeight();
			aState->Enabled = state->getEnabled();
			aState->Loop = state->getLoop();

			m_Init_AnimationStates.push_back(aState);
		}
	}
	aState = NULL;
	state = NULL;
	set = NULL;

	m_Init_RenderQueue = GetRenderQueue();
}


void MeshGameObject::ResetToInitializeState(void)
{
	GameObject::ResetToInitializeState();

	for (unsigned int i = 0; i < m_Init_MaterialNames.size(); i++)
		m_Init_MaterialNames[i].first->setMaterialName(m_Init_MaterialNames[i].second);
	m_Init_MaterialNames.clear();

	if (m_Init_AnimationStates.size())
	{
		AnimationState *state = NULL;
		for (unsigned int i = 0; i < m_Init_AnimationStates.size(); i++)
		{
			state = m_Init_AnimationStates[i];

			state->Animation->setTimePosition(state->TimePosition);
			state->Animation->setLength(state->Length);
			state->Animation->setWeight(state->Weight);
			state->Animation->setEnabled(state->Enabled);
			state->Animation->setLoop(state->Loop);
		}
		state = NULL;
		m_Init_AnimationStates.clear();
	}

	SetRenderQueue(m_Init_RenderQueue);
}


void MeshGameObject::SetMaterialName(const Ogre::String &Name)
{
	m_Entity->setMaterialName(Name);
}


void MeshGameObject::SetMaterialName(const unsigned int &SubMeshIndex, const Ogre::String &Name)
{
	m_Entity->getSubEntity(SubMeshIndex)->setMaterialName(Name);
}


const Ogre::String &MeshGameObject::GetMaterialName(void)
{
	return GetMaterialName(0);
}


const Ogre::String &MeshGameObject::GetMaterialName(const unsigned int &SubMeshIndex)
{
	return m_Entity->getSubEntity(SubMeshIndex)->getMaterialName();
}


const Ogre::Material *MeshGameObject::GetMaterial(void)
{
	return GetMaterial(0);
}


const Ogre::Material *MeshGameObject::GetMaterial(const unsigned int &SubMeshIndex)
{
	return m_Entity->getSubEntity(SubMeshIndex)->getMaterial().getPointer();
}


void MeshGameObject::SetEntity(const Ogre::String &MeshName)
{
	m_Entity = m_Holder->AddEntity(MeshName);

	//m_Entity->getMesh()->buildEdgeList();

	//int qid = m_Entity->getRenderQueueGroup();
	
	//m_Entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_3);

	m_SceneNode->attachObject(m_Entity);

	//
	// Requierd for NormalBump mapping
	//
	unsigned short src, dest;
	if (!m_Entity->getMesh()->suggestTangentVectorBuildParams(Ogre::VES_NORMAL, src, dest))
	{
		m_Entity->getMesh()->buildTangentVectors(Ogre::VES_NORMAL, src, dest);
	}

	if (!m_Entity->getMesh()->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
	{
		m_Entity->getMesh()->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
	}

	if (!m_Entity->getMesh()->suggestTangentVectorBuildParams(Ogre::VES_BINORMAL, src, dest))
	{
		m_Entity->getMesh()->buildTangentVectors(Ogre::VES_BINORMAL, src, dest);
	}
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(MeshGameObject)
	BEGIN_DEFINE_TYPE_BASE(GameObject, MeshGameObject, "MeshGameObject")
		DEFINE_FUNCTION("SetMaterialName", (void(MeshGameObject::*)(const Ogre::String&))&MeshGameObject::SetMaterialName)
		DEFINE_FUNCTION("SetMaterialName", (void(MeshGameObject::*)(const unsigned int&, const Ogre::String&))&MeshGameObject::SetMaterialName)
		
		DEFINE_FUNCTION("GetMaterialName", (const Ogre::String&(MeshGameObject::*)(void))&MeshGameObject::GetMaterialName)
		DEFINE_FUNCTION("GetMaterialName", (const Ogre::String&(MeshGameObject::*)(const unsigned int&))&MeshGameObject::GetMaterialName)
		
		DEFINE_FUNCTION("GetMaterialName", (const Ogre::Material*(MeshGameObject::*)(void))&MeshGameObject::GetMaterial)
		DEFINE_FUNCTION("GetMaterialName", (const Ogre::Material*(MeshGameObject::*)(const unsigned int&))&MeshGameObject::GetMaterial)

		DEFINE_FUNCTION("SetRenderQueue", &MeshGameObject::SetRenderQueue)
		
		DEFINE_FUNCTION("GetAnimationState", &MeshGameObject::GetAnimationState)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE