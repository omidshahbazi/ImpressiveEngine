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

#include "NodeBase.h"
#include "Scriptable.h"
#include "StdAfxOGRE.h"
#include "TrackAnimator.h"

BEGIN_NAMESPACE
class Scene;

class GameObject : public NodeBase, public Scriptable/*, public ISerializable*/
{
public:
	GameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent = NULL);
	virtual ~GameObject(void)
	{
		_Destroy();
	}

	void Start(void);
	bool Update(void);

#ifdef EDIT_MODE
	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);
	
	void SetInitializeState(void);
	void ResetToInitializeState(void);

#ifdef EDIT_MODE
	GameObject *Clone(const Ogre::String &Name, GameObject *Parent = NULL);
#endif

	void Remove(void);
	void Destroy(void);

	//void OnSetObject(Script *Script, const char *Name);

	void SetParent(GameObject *Parent);

	GameObject *CreateChild(const Ogre::String &Name);
	//void DestroyChild(GameObject *Object);
	
	void AddChild(GameObject *Object);
	void RemoveChild(GameObject *Object, bool InDestoryRemoving = false);

	TrackAnimator *CreateTrackAnimatorScriptUse(const Ogre::String &Name, const float &Length);
	TrackAnimator *CreateTrackAnimator(const Ogre::String &Name, const float &Length, bool ScriptUse = false);

	std::list<GameObject*> &GetChildren(void)
	{
		return m_Children;
	}
	
	GameObject *GetParent(void)
	{
		return m_Parent;
	}

	bool HasNode(Ogre::SceneNode *Node)
	{
		return (m_SceneNode == Node);
	}

	unsigned int GetID(void)
	{
		return m_ID;
	}

	const Ogre::String &GetName(void)
	{
		return m_Name;
	}

	void SetName(const Ogre::String &Name)
	{
		m_Name = Name;
	}

#ifdef EDIT_MODE
	Ogre::String GetEditorText(void)
	{
		return Ogre::StringConverter::toString(m_ID) + " - " + m_Name;
	}
#endif

	//Ogre::Node::ChildNodeIterator GetChilds(void)
	//{
	//	return m_SceneNode->getChildIterator();
	//}

	//std::list<GameObject*> GetChilds(void);
	//{
	//	std::list<GameObject*> list;

	//	Ogre::SceneNode::ChildNodeIterator it = m_SceneNode->getChildIterator();

	//	while (it.hasMoreElements())
	//	{
	//		list.push_back(Ogre::any_cast<GameObject*>(it.getNext()->getUserAny()));
	//	}

	//	return list;
	//}
	
	virtual const Ogre::Vector3 &GetPosition(void)
	{
		return m_SceneNode->getPosition();
	}

	virtual void SetPosition(const Ogre::Vector3 &Position)
	{
		m_SceneNode->setPosition(Position);
	}
	
	virtual void SetPosition(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_SceneNode->setPosition(X, Y, Z);
	}

	const Ogre::Vector3 &GetScale(void)
	{
		return m_SceneNode->getScale();
	}

	virtual void SetScale(const Ogre::Vector3 &Scale)
	{
		m_SceneNode->setScale(Scale);
	}
	
	virtual void SetScale(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_SceneNode->setScale(X, Y, Z);
	}

	virtual void Pitch(Ogre::Real Value)
	{
		m_SceneNode->pitch(Ogre::Degree(Value));
	}

	virtual void Yaw(Ogre::Real Value)
	{
		m_SceneNode->yaw(Ogre::Degree(Value));
	}

	virtual void Roll(Ogre::Real Value)
	{
		m_SceneNode->roll(Ogre::Degree(Value));
	}
	
	// Bug fixed : const Ogre::Vector3 instead const Ogre::Vector3 & (in scripting)
	virtual const Ogre::Vector3 GetRotation(void)
	{
		return Ogre::Vector3(GetOrientation().getPitch().valueDegrees(), GetOrientation().getYaw().valueDegrees(), GetOrientation().getRoll().valueDegrees());
	}
	
	virtual void SetRotation(Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{	
		m_SceneNode->resetOrientation();

		m_SceneNode->pitch(Ogre::Degree(X));
		m_SceneNode->yaw(Ogre::Degree(Y));
		m_SceneNode->roll(Ogre::Degree(Z));

		_ManualUpdate();
	}
	
	virtual void SetRotation(const Ogre::Vector3 &Rotation)
	{
		SetRotation(Rotation.x, Rotation.y, Rotation.z);
	}

	virtual const Ogre::Quaternion &GetOrientation(void)
	{
		return m_SceneNode->getOrientation();
	}
	
	virtual void SetOrientation(const Ogre::Quaternion &Quaternion)
	{
		m_SceneNode->setOrientation(Quaternion);
	}

	virtual void SetOrientation(Ogre::Real W, Ogre::Real X, Ogre::Real Y, Ogre::Real Z)
	{
		m_SceneNode->setOrientation(W, X, Y, Z);
	}

	const Ogre::Vector3 GetForward(void)
	{
		return GetOrientation() * Ogre::Vector3::UNIT_Z;
	}

	const Ogre::Vector3 GameObject::GetRight(void)
	{
		return GetOrientation() * Ogre::Vector3::UNIT_X;
	}

	const Ogre::Vector3 GameObject::GetUp(void)
	{
		return GetOrientation() * Ogre::Vector3::UNIT_Y;
	}

	virtual bool GetVisible(void)
	{
		return m_Visible;
	}

	virtual void SetVisible(bool Value);
	
	virtual const Ogre::AxisAlignedBox &GetWorldAABB(void) const
	{
		return m_SceneNode->_getWorldAABB();
	}

	Ogre::Vector3 ConvertWorldToLocalPosition(const Ogre::Vector3 &Position)
	{
		return m_SceneNode->convertWorldToLocalPosition(Position);
	}

	Ogre::Vector3 ConvertLocalToWorldPosition(const Ogre::Vector3 &Position)
	{
		return m_SceneNode->convertLocalToWorldPosition(Position);
	}

	Ogre::Quaternion ConvertWorldToLocalOrientation(const Ogre::Quaternion &Orientation)
	{
		return m_SceneNode->convertWorldToLocalOrientation(Orientation);
	}

	Ogre::Quaternion ConvertLocalToWorldOrientation(const Ogre::Quaternion &Orientation)
	{
		return m_SceneNode->convertLocalToWorldOrientation(Orientation);
	}

	void ShowBoundingBox(bool Value)
	{
		m_SceneNode->showBoundingBox(Value);
	}

	void _ManualUpdate(void)
	{
		m_SceneNode->_update(true, false);
	}

	Ogre::SceneNode *_GetSceneNode(void)
	{
		return m_SceneNode;
	}

	enum GameObjectType
	{
		GOT_EMPTY = 0,
		GOT_MESH,
		GOT_CAMERA,
		GOT_LIGHT,
		GOT_TERRAIN,
		GOT_OCEAN,
		GOT_PARTICLE
	};

	virtual GameObjectType GetType(void) const
	{
		return GOT_EMPTY;
	}

#ifdef EDIT_MODE
	void SetLocked(bool Value);

	bool GetLocked(void)
	{
		return m_Locked;
	}
#endif

	void SetAsRemoved(bool Value)
	{
		m_WasRemoved = Value;
	}

	bool GetWasRemove(void)
	{
		return m_WasRemoved;
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	void _Destroy(void);

protected:
	unsigned int m_ID;
	Ogre::String m_Name;
	Scene *m_Holder;
	Ogre::SceneNode *m_SceneNode;
	bool m_Visible;

#ifdef EDIT_MODE
	bool m_Locked;
#endif

	bool m_WasRemoved;

	//std::list<Ogre::String> m_Entities;
	GameObject *m_Parent;
	std::list<GameObject*> m_Children;

	std::list<Animator*> m_Animators;
	std::list<Animator*> m_ScriptAnimators;

private:
	bool m_Init_WasRemoved;

	bool m_Init_Visible;
	GameObject *m_Init_Parent;
	std::list<GameObject*> m_Init_Children;
};
END_NAMESPACE