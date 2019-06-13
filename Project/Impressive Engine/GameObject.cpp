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
#include "GameObject.h"
#include "Scene.h"
//#include "Script.h"
#include "Utility.h"
#include "ScriptRegister.h"

#ifdef EDIT_MODE
#include "Editor.h"
#endif

BEGIN_NAMESPACE
GameObject::GameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent) :
	m_ID(0),
	m_Name(Name),
	m_Holder(Holder),
	m_SceneNode(NULL),
	m_Visible(true),
	m_Parent(NULL)
#ifdef EDIT_MODE
	,m_Locked(false)
#endif
	,m_WasRemoved(false)
{
	m_ID = m_Holder->GetNewGameObjectID();

	m_Children.clear();
	
	if (Parent)
	{
		m_Parent = m_Holder->GetRootGameObject();
		m_SceneNode = m_Holder->GetSceneManager()->getRootSceneNode()->createChildSceneNode(m_Holder->GetNewSceneNodeName(m_ID));

		//m_SceneNode->setUserAny(Ogre::Any(this));
		
		//SetParent(Parent);
		Parent->AddChild(this);
	}
	else
	{
		m_SceneNode = m_Holder->GetSceneManager()->getRootSceneNode();
	}

	Scriptable::SetAsGameObject();
}


//GameObject::~GameObject(void)
//{
//	for (unsigned int i = 0; i < GetAttachedObjectsCount(); i++)
//		m_Holder->GetSceneManager()->destroyEntity(GetAttachedObject(i));
//
//	if (m_SceneNode != m_Holder->GetSceneManager()->getRootSceneNode())
//	{
//		m_Holder->GetSceneManager()->destroySceneNode(m_SceneNode);
//		m_SceneNode = NULL;
//	}
//
//	m_Parent = NULL;
//
//	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
//		delete current;
//	END_LIST_ITERATOR(NULL)
//	m_Children.clear();
//
//	m_Init_Parent = NULL;
//	m_Init_Children.clear();
//	m_Init_MaterialNames.clear();
//}


void GameObject::Start(void)
{
	if (m_WasRemoved)
		return;

	CallFunction("Start");

	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
		current->Start();
	END_LIST_ITERATOR(NULL)
}
	

bool GameObject::Update(void)
{
	if (m_WasRemoved)
		return false;

	CallFunction("Update");

	BEGIN_LIST_ITERATOR(m_Animators, Animator*)
		current->Update();
	END_LIST_ITERATOR(NULL)

	BEGIN_LIST_ITERATOR(m_ScriptAnimators, Animator*)
		current->Update();
	END_LIST_ITERATOR(NULL)

	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
		current->Update();
	END_LIST_ITERATOR(NULL)

	return true;
}

#ifdef EDIT_MODE
bool GameObject::Serialize(Attributes *Attributes)
{
	Scriptable::Serialize(Attributes);

	Attributes->AddVector3("Position", GetPosition());
	Attributes->AddVector3("Scale", GetScale());
	Attributes->AddQuaternion("Orientation", GetOrientation());

	Attributes->AddBoolean("Visible", GetVisible());

#ifdef EDIT_MODE
	Attributes->AddBoolean("Locked", GetLocked());
#endif

	return true;
}
#endif

bool GameObject::Deserialize(Attributes *Attributes/*TreeReader *Reader*/)
{
	Scriptable::Deserialize(Attributes);
	
	SetPosition(Attributes->GetVector3("Position"));
	SetScale(Attributes->GetVector3("Scale"));
	SetOrientation(Attributes->GetQuaternion("Orientation"));
	
	SetVisible(Attributes->GetBoolean("Visible"));

#ifdef EDIT_MODE
	SetLocked(Attributes->GetBoolean("Locked"));
#endif

	return true;
}


//GameObject *GameObject::AddChild(const Ogre::String &Name)
//{
//	return new GameObject(Name, this);
//}


void GameObject::SetInitializeState(void)
{
	Scriptable::SetInitializeState();

	m_SceneNode->setInitialState();
	
#ifdef EDIT_MODE
	Editor::GetInstance().SeparateTranslateGizmo(m_SceneNode);
#endif

	m_Init_Visible = m_Visible;
	
#ifdef EDIT_MODE
	ShowBoundingBox(false);
#endif

	m_Init_WasRemoved = m_WasRemoved;

	m_Init_Parent = m_Parent;
	m_Init_Children = m_Children;

	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
		current->SetInitializeState();
	END_LIST_ITERATOR(NULL)
}


void GameObject::ResetToInitializeState(void)
{
	Scriptable::ResetToInitializeState();

	m_SceneNode->resetToInitialState();
	
	SetVisible(m_Init_Visible);

	m_WasRemoved = m_Init_WasRemoved;
	
	m_Parent = m_Init_Parent;
	m_Children = m_Init_Children;
	//m_Init_Children.clear();

	BEGIN_LIST_ITERATOR(m_ScriptAnimators, Animator*)
		delete current;
	END_LIST_ITERATOR(NULL)
	m_ScriptAnimators.clear();

	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
		current->ResetToInitializeState();
	END_LIST_ITERATOR(NULL)
}

#ifdef EDIT_MODE
GameObject *GameObject::Clone(const Ogre::String &Name, GameObject *Parent)
{
	if (!Parent)
		Parent = m_Holder->GetRootGameObject();

	GameObject *newObj = Parent->CreateChild(Name);

	newObj->SetOrientation(GetOrientation());
	newObj->SetPosition(GetPosition());
	newObj->SetScale(GetScale());

	//BEGIN_LIST_ITERATOR(m_Entities, Ogre::String)
	//	newObj->AddEntity(current);
	//END_LIST_ITERATOR("")
	//Ogre::SceneNode::ObjectIterator it = m_SceneNode->getAttachedObjectIterator();
	//while (it.hasMoreElements())
	//	newObj->AddEntity(Ogre::StringUtil::split(it.getNext()->getName(), "|")[0]);

	return newObj;
}
#endif


void GameObject::Remove(void)
{
	m_Holder->AddToGameObjectRemoveQueue(this);

	//BEGIN_LIST_ITERATOR(m_Children, GameObject*)
	//	current->Remove();
	//END_LIST_ITERATOR(NULL)
}


void GameObject::Destroy(void)
{
	m_Holder->AddToGameObjectDestroyQueue(this);
}


//void GameObject::OnSetObject(Script *Script, const char *Name)
//{
//	Script->GetTable()[Name] = (GameObject*)this;
//}


void GameObject::SetParent(GameObject *Parent)
{
	//m_Parent->RemoveChild(this);

	m_Parent = Parent;

	//m_Parent->AddChild(this);

	if (m_SceneNode)
	{
		m_SceneNode->setParent(NULL);
	}	
}


GameObject *GameObject::CreateChild(const Ogre::String &Name)
{
	GameObject *object = new GameObject(Name, m_Holder, this);

	AddChild(object);

#ifdef EDIT_MODE
	Editor::GetInstance().AddCreateGameObjectDo(object);
#endif

	return object;
}


//void GameObject::DestroyChild(GameObject *Object)
//{
//	RemoveChild(Object);
//
//	// Safe method
//	Object->Remove();
//}


void GameObject::AddChild(GameObject *Object)
{
	if (Object->GetParent())
		Object->GetParent()->RemoveChild(Object);

	m_Children.push_back(Object);

	Object->SetParent(this);
	
	if (m_SceneNode)
		m_SceneNode->addChild(Object->_GetSceneNode());

	_ManualUpdate();
}


void GameObject::RemoveChild(GameObject *Object, bool InDestoryRemoving)
{
	m_Children.remove(Object);

	if (InDestoryRemoving)
		m_Init_Children.remove(Object);

	Object->SetParent(NULL);

	if (m_SceneNode)
		m_SceneNode->removeChild(Object->_GetSceneNode());

	Object->SetParent(m_Holder->GetRootGameObject());

	_ManualUpdate();
}


TrackAnimator *GameObject::CreateTrackAnimatorScriptUse(const Ogre::String &Name, const float &Length)
{
	return CreateTrackAnimator(Name, Length, true);
}


TrackAnimator *GameObject::CreateTrackAnimator(const Ogre::String &Name, const float &Length, bool ScriptUse)
{
	TrackAnimator *ta = new TrackAnimator(this, Name, Length);

	if (ScriptUse)
		m_ScriptAnimators.push_back(ta);
	else
	m_Animators.push_back(ta);

	return ta;
}


//void GameObject::SetParent(GameObject *Parent)
//{
//	if (Parent)
//	{
//		//m_SceneNode->setParent(Parent->_GetSceneNode());
//
//		m_SceneNode->setParent(NULL);
//		Parent->_GetSceneNode()->addChild(m_SceneNode);
//	}
//	else
//	{
//		m_SceneNode->getParent()->removeChild(m_SceneNode);
//		m_SceneNode->setParent(m_Holder->GetSceneManager()->getRootSceneNode());
//	}
//}


//GameObject *GameObject::GetParent(void)
//{
//	if (m_SceneNode->getParent() == m_Holder->GetSceneManager()->getRootSceneNode())
//		return NULL;
//
//	return Ogre::any_cast<GameObject*>(m_SceneNode->getParent()->getUserAny());
//}


void GameObject::SetVisible(bool Value)
{
	if (m_Holder->GetRootGameObject() == this)
		return;

	//m_SceneNode->setVisible(Value);

	m_Visible = Value;

#ifdef EDIT_MODE
	m_SceneNode->showBoundingBox(false);

	Editor::GetInstance().SeparateTranslateGizmo(m_SceneNode);
#endif

	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
		current->SetVisible(Value);
	END_LIST_ITERATOR(NULL)
}

#ifdef EDIT_MODE
void GameObject::SetLocked(bool Value)
{
	m_Locked = Value;

	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
		current->SetLocked(Value);
	END_LIST_ITERATOR(NULL)
}
#endif

void GameObject::_Destroy(void)
{
	BEGIN_LIST_ITERATOR(m_Animators, Animator*)
		delete current;
	END_LIST_ITERATOR(NULL)
	m_Animators.clear();

	BEGIN_LIST_ITERATOR(m_Children, GameObject*)
		delete current;
	END_LIST_ITERATOR(NULL)
	m_Children.clear();
	
	if (m_SceneNode != m_Holder->GetSceneManager()->getRootSceneNode())
	{
		m_Holder->GetSceneManager()->destroySceneNode(m_SceneNode);
		m_SceneNode = NULL;
	}

	m_Parent = NULL;

	m_Init_Parent = NULL;
	m_Init_Children.clear();
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(GameObject)
	BEGIN_DEFINE_ENUMERATOR(GameObjectType, "GameObjectType")
		DEFINE_ENUMERATOR_VALUE("GOT_EMPTY", GameObject::GOT_EMPTY),
		DEFINE_ENUMERATOR_VALUE("GOT_MESH", GameObject::GOT_MESH),
		DEFINE_ENUMERATOR_VALUE("GOT_CAMERA", GameObject::GOT_CAMERA),
		DEFINE_ENUMERATOR_VALUE("GOT_LIGHT", GameObject::GOT_LIGHT),
		DEFINE_ENUMERATOR_VALUE("GOT_TERRAIN", GameObject::GOT_TERRAIN),
		DEFINE_ENUMERATOR_VALUE("GOT_OCEAN", GameObject::GOT_OCEAN)
	END_DEFINE_ENUMERATOR()

	BEGIN_DEFINE_TYPE_BASE(Scriptable, GameObject, "GameObject")
		DEFINE_FUNCTION("GetParent", &GameObject::GetParent)
		DEFINE_FUNCTION("SetParent", &GameObject::SetParent)

		DEFINE_FUNCTION("GetID", &GameObject::GetID)
		DEFINE_FUNCTION("GetName", &GameObject::GetName)

		DEFINE_FUNCTION("Remove", &GameObject::Remove)
		DEFINE_FUNCTION("Destroy", &GameObject::Destroy)
		//DEFINE_FUNCTION("AddEntity", &GameObject::AddEntity)

		DEFINE_FUNCTION("ResetToInitializeState", &GameObject::ResetToInitializeState)

		DEFINE_FUNCTION("CreateChild", &GameObject::CreateChild)
		//DEFINE_FUNCTION("DestroyChild", &GameObject::DestroyChild)
	
		DEFINE_FUNCTION("AddChild", &GameObject::AddChild)
		DEFINE_FUNCTION("RemoveChild", &GameObject::RemoveChild)

		DEFINE_FUNCTION("CreateTrackAnimator", &GameObject::CreateTrackAnimatorScriptUse)

		DEFINE_FUNCTION("GetPosition", &GameObject::GetPosition)
		DEFINE_FUNCTION("SetPosition", (void(GameObject::*)(const Ogre::Vector3&))&GameObject::SetPosition)
		DEFINE_FUNCTION("SetPosition", (void(GameObject::*)(float, float, float))&GameObject::SetPosition)

		DEFINE_FUNCTION("GetScale", &GameObject::GetScale)
		DEFINE_FUNCTION("SetScale", (void(GameObject::*)(const Ogre::Vector3&))&GameObject::SetScale)
		DEFINE_FUNCTION("SetScale", (void(GameObject::*)(float, float, float))&GameObject::SetScale)
		
		DEFINE_FUNCTION("Pitch", &GameObject::Pitch)
		DEFINE_FUNCTION("Yaw", &GameObject::Yaw)
		DEFINE_FUNCTION("Roll", &GameObject::Roll)

		DEFINE_FUNCTION("GetRotation", &GameObject::GetRotation)
		DEFINE_FUNCTION("SetRotation", (void(GameObject::*)(const Ogre::Vector3&))&GameObject::SetRotation)
		DEFINE_FUNCTION("SetRotation", (void(GameObject::*)(float, float, float))&GameObject::SetRotation)

		DEFINE_FUNCTION("GetForward", &GameObject::GetForward)
		DEFINE_FUNCTION("GetRight", &GameObject::GetRight)
		DEFINE_FUNCTION("GetUp", &GameObject::GetUp)

		DEFINE_FUNCTION("GetAABB", &GameObject::GetWorldAABB)

		//DEFINE_FUNCTION("GetOrientation", &GameObject::GetOrientation)
		//DEFINE_FUNCTION("SetOrientation", &GameObject::SetOrientation)

		DEFINE_FUNCTION("GetVisible", &GameObject::GetVisible)
		DEFINE_FUNCTION("SetVisible", &GameObject::SetVisible)

		DEFINE_FUNCTION("SetShowBoundingBox", &GameObject::ShowBoundingBox)
		
		DEFINE_FUNCTION("GetType", &GameObject::GetType)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE