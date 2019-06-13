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

BEGIN_NAMESPACE
class MeshGameObject : public GameObject
{
public:
	MeshGameObject(const Ogre::String &Name, Scene *Holder, const Ogre::String &MeshName = "", GameObject *Parent = NULL);
	~MeshGameObject(void);
	
	bool Update(void);

#ifdef EDIT_MODE
	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);
	
	void SetInitializeState(void);
	void ResetToInitializeState(void);

	void SetMaterialName(const Ogre::String &Name);
	void SetMaterialName(const unsigned int &SubMeshIndex, const Ogre::String &Name);
	
	const Ogre::String &GetMaterialName(void);
	const Ogre::String &GetMaterialName(const unsigned int &SubMeshIndex);

	const Ogre::Material *GetMaterial(void);
	const Ogre::Material *GetMaterial(const unsigned int &SubMeshIndex);

	bool GetVisible(void)
	{
		return m_Entity->getVisible();
	}

	void SetVisible(bool Value)
	{
		GameObject::SetVisible(Value);

		m_Entity->setVisible(Value);
	}

	void SetCastShadows(const bool &Value)
	{
		return m_Entity->setCastShadows(Value);
	}

	const bool GetCastShadows(void) const
	{
		return m_Entity->getCastShadows();
	}

	void SetRenderQueue(Ogre::RenderQueueGroupID Group)
	{
		m_Entity->setRenderQueueGroup(Group);
	}

	const Ogre::RenderQueueGroupID GetRenderQueue(void) const
	{
		return (Ogre::RenderQueueGroupID)m_Entity->getRenderQueueGroup();
	}
	
	Ogre::AnimationStateSet *GetAllAnimationStates(void) const
	{
		return m_Entity->getAllAnimationStates();
	}

	Ogre::AnimationState *GetAnimationState(const Ogre::String &Name) const
	{
		return m_Entity->getAnimationState(Name);
	}

	GameObjectType GetType(void) const
	{
		return GOT_MESH;
	}

private:
	void SetEntity(const Ogre::String &MeshName);

public:
	DECLARE_SCRIPT_REGISTATION()

private:
	Ogre::Entity *m_Entity;

	struct AnimationState
	{
		Ogre::AnimationState *Animation;
		float TimePosition;
		float Length;
		float Weight;
		bool Enabled;
		bool Loop;
	};
	
	typedef std::vector<AnimationState*> AnimationStateList;
	
	typedef std::pair<Ogre::SubEntity*, Ogre::String> MaterialNamePair;
	typedef std::vector<MaterialNamePair> MaterialNamePairList;

	MaterialNamePairList m_Init_MaterialNames;
	AnimationStateList m_Init_AnimationStates;

	Ogre::RenderQueueGroupID m_Init_RenderQueue;
};

END_NAMESPACE
