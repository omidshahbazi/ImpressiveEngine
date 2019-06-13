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
#ifdef EDIT_MODE

#include "EditorScene.h"
#include "ScenesHolder.h"
#include "DeferredShadingSystem.h"
#include "EditorMaterialGenerator.h"
#include "Entity.h"
#include "MainWindow.h"
#include "HID.h"
#include "RenderControl.h"
#include "VirtualMachine.h"
#include "ViewPortsManager.h"
#include "ScriptControl.h"
#include "Editor.h"
#include "Attributes.h"
#include "Time.h"
#include "DebugDrawer.h"
#include "GameObjectFactory.h"
#include "CameraGameObject.h"
#include "TreeParser.h"
#include "Stats.h"
#include "Log.h"
#include "Exception.h"
#include "Utility.h"


BEGIN_NAMESPACE
EditorScene::EditorScene(const Ogre::String &Name) :
	Scene(Name),
	m_IsMouseDown(false),
	m_RaySceneQuery(NULL),
	m_SelectedGameObject(NULL)
{
	EditorMaterialGenerator::GetInstance().Initialize();

	m_RaySceneQuery = m_SceneManager->createRayQuery(Ogre::Ray());
	//m_RaySceneQuery->setSortByDistance(true);

	DebugDrawer::GetInstance().Initialize(this);
}


EditorScene::~EditorScene(void)
{
	delete DebugDrawer::GetInstancePointer();
}


void EditorScene::Start(void)
{
	if (!MainWindow::GetPointer()->IsInEditMode())
		Scene::Start();
}

	
bool EditorScene::Update(void)
{
	m_DeferredShadingSystem->Update();

	const HID &hid = HID::GetInstance();

	if (MainWindow::GetPointer()->IsInEditMode())
	{
		BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
			if (current->GetType() == GameObject::GOT_CAMERA || 
				current->GetType() == GameObject::GOT_LIGHT || 
				current->GetType() == GameObject::GOT_OCEAN)
				current->Update();
		END_LIST_ITERATOR(NULL)

		Editor::GetInstance().Update();

		if (hid.IsThisActive(ViewPortsManager::GetInstance().GetEditRenderControl()) && 
			hid.IsControlDown() && hid.WasMouseButtonPressed(HID::M_LEFT))
		{
			m_RaySceneQuery->setRay(ViewPortsManager::GetInstance().GetViewpPortRay());
			Ogre::RaySceneQueryResult &result = m_RaySceneQuery->execute();

			Ogre::RaySceneQueryResult::iterator it = result.begin();

			Ogre::MovableObject *object = NULL;
			Ogre::SceneNode *node = NULL;
			while (it != result.end())
			{
				object = it->movable;

				if (object->getQueryFlags() != Editor::GIZMO_FLAG &&
					object->getName() != m_AtmosphereManager->GetEntityName())
				{
					node = object->getParentSceneNode();

					if (node->getUserAny().getType() == typeid(void))
						m_SelectedGameObject = GetGameObject(node);
					else
						m_SelectedGameObject = (GameObject*)Ogre::any_cast<void*>(node->getUserAny());

					if (m_SelectedGameObject)
						SelectObject(m_SelectedGameObject);
					
					ScenesHolder::GetInstance().GetControl()->SelectItem(m_SelectedGameObject->GetEditorText());
					ScenesHolder::GetInstance().GetControl()->RefreshPropertyGrid();

					break;
				}

				it++;
			}
		}
	}
	else
		Scene::Update();

	if (m_Stats)
		m_Stats->Update();

	if (m_SkyMode == SM_ADVANCED_SKY)
		m_AtmosphereManager->Update();

	if (hid.IsAltDown() && hid.WasKeyPressed(HID::K_L) && m_SelectedGameObject)
	{
		m_SelectedGameObject->SetLocked(!m_SelectedGameObject->GetLocked());

		ScenesHolder::GetInstance().GetControl()->RefreshPropertyGrid();
	}

	return true;
}


bool EditorScene::Serialize(Attributes *Attributes)
{
	Scene::Serialize(Attributes);
	
	Attributes->AddColor("ClearColor", ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport()->getBackgroundColour());
	Attributes->AddInteger("RenderMode", m_DeferredShadingSystem->GetRenderMode());
	Attributes->AddColor("AmbientLight", m_SceneManager->getAmbientLight());

	//Attributes->AddBoolean("ShowDebugShadows", m_SceneManager->getShowDebugShadows());
	//Attributes->AddInteger("ShadowTechnique", (int)m_SceneManager->getShadowTechnique());
	//Attributes->AddColor("ShadowColor", m_SceneManager->getShadowColour());
	//Attributes->AddFloat("ShadowDirectionalLightExtrusionDistance", m_SceneManager->getShadowDirectionalLightExtrusionDistance());
	//Attributes->AddFloat("ShadowFarDistance", m_SceneManager->getShadowFarDistance());
	//Attributes->AddInteger("ShadowIndexBufferSize", m_SceneManager->getShadowIndexBufferSize());
	//Attributes->AddInteger("ShadowTextureSize", m_SceneManager->getShadowTextureConfigIterator().begin()->width);
	//Attributes->AddFloat("ShadowDirectionalLightTextureOffset", m_SceneManager->getShadowDirLightTextureOffset());
	//Attributes->AddBoolean("ShadowTextureSelfShadow", m_SceneManager->getShadowTextureSelfShadow());
	//Attributes->AddBoolean("ShadowCasterRenderBackFaces", m_SceneManager->getShadowCasterRenderBackFaces());

	Attributes->AddInteger("SkyMode", GetSkyMode());

	switch (m_SkyMode)
	{
	case SM_SKY_BOX:
		{
			if (m_SkyBoxMaterial.size())
			{
				Attributes->AddBoolean("SkyBoxEnabled", m_SceneManager->isSkyBoxEnabled());
				Attributes->AddString("SkyBoxMaterialName", m_SkyBoxMaterial);
				Attributes->AddFloat("SkyBoxDistance", m_SceneManager->getSkyBoxGenParameters().skyBoxDistance);
			}
		} break;

	case SM_SKY_DOME:
		{
			if (m_SkyDomeMaterial.size())
			{
				Attributes->AddBoolean("SkyDomeEnabled", m_SceneManager->isSkyDomeEnabled());
				Attributes->AddString("SkyDomeMaterialName", m_SkyDomeMaterial);
				Attributes->AddFloat("SkyDomeCurvature", m_SceneManager->getSkyDomeGenParameters().skyDomeCurvature);
				Attributes->AddFloat("SkyDomeTiling", m_SceneManager->getSkyDomeGenParameters().skyDomeTiling);
				Attributes->AddFloat("SkyDomeDistance", m_SceneManager->getSkyDomeGenParameters().skyDomeDistance);
			}
		} break;

	case SM_ADVANCED_SKY:
		{
			m_AtmosphereManager->Serialize(Attributes);
		} break;
	}

	Attributes->AddVector3("SunPosition", GetSunPosition());
	Attributes->AddFloat("SunStrength", GetSunStrength());
	Attributes->AddFloat("SunArea", GetSunArea());
	Attributes->AddColor("SunColor", GetSunColor());

	BEGIN_VECTOR_ITERATOR(m_PostProcessEffects, Ogre::String)
		Attributes->AddString(STRING_OGRE_TO_WX("PPE" + Ogre::StringConverter::toString(i)), STRING_OGRE_TO_WX(current));
	END_VECTOR_ITERATOR("")

	Attributes->AddString("NavGraphFileName", GetNavGraphFileName());

	return true;
}


void EditorScene::OnTranslate(const Ogre::Vector3 &Value)
{
	m_SelectedGameObject->SetPosition(Value);
}


void EditorScene::OnScale(const Ogre::Vector3 &Value)
{
}


void EditorScene::OnRotate(const Ogre::Vector3 &Value)
{
}


#ifdef DEBUG
void EditorScene::InitializeStats(void)
{
	m_Stats = new Stats(ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport(), m_SceneManager);
}
#endif

void EditorScene::ResetToInitializeStateScriptUsage(void)
{
	Log::GetInstance().AddInfo(Ogre::String("(") + GetName() + ")Scene::ResetToInitializeState(void) called by script");
}


void EditorScene::Resume(void)
{
	Log::GetInstance().AddInfo(Ogre::String("(") + GetName() + ")Scene::Resume(void) called by script");
}


void EditorScene::Destroy(void)
{
	Log::GetInstance().AddInfo(Ogre::String("(") + GetName() + ")Scene::Destroy(void) called by script");
}


void EditorScene::RemoveGameObject(GameObject *Object)
{
	if (Object)
	{
		if (MainWindow::GetPointer()->IsInEditMode())
		{
			//if (m_SelectedNode == Object->_GetSceneNode())
			//	m_SelectedNode = NULL;
			if (m_SelectedGameObject == Object)
				m_SelectedGameObject = NULL;

			Editor::GetInstance().AddRemoveGameObjectDo(Object);

			return;
		}

		Scene::RemoveGameObject(Object);

		RefreshSceneControlContent();
	}
}


void EditorScene::DestroyGameObject(GameObject *Object)
{
	RemoveGameObject(Object);
}


void EditorScene::Clear(void)
{
	Scene::Clear();

	ViewPortsManager::GetInstance().GetGameRenderControl()->SetCamera(NULL);
	
	ScenesHolder::GetInstance().GetControl()->ReloadContent();

	Editor::GetInstance().SeparateTranslateGizmo();
	
	ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()->setBackgroundColour(Ogre::ColourValue::Black);
}


bool EditorScene::ConnectFirstCamera(void)
{
	bool result = true;

	CameraGameObject *camera = GetSuitableCameraToStart();

	if (camera)
		ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()->setCamera(camera->_GetCamera());
	else
	{
		Log::GetInstance().AddError("There's no camera in the scene. At least add one camera");
		result = false;
	}
	
	ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport()->setBackgroundColour(
		ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport()->getBackgroundColour());

	return result;
}


void EditorScene::HandleViewportChange(void)
{
	Ogre::Viewport *vp = NULL;

	if (MainWindow::GetPointer()->IsInEditMode())
		vp = ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport();
	else
		vp = ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport();
	
	if (vp)
		m_DeferredShadingSystem->OnViewportChanges(vp);
	
	BEGIN_LIST_ITERATOR(m_RootGameObject->GetChildren(), GameObject*)
		if (current->GetType() == GameObject::GOT_OCEAN)
			((OceanGameObject*)current)->OnViewportChanges(vp);
	END_LIST_ITERATOR(NULL)

	vp = NULL;
	
	if (MainWindow::GetPointer()->IsInEditMode())
		m_AtmosphereManager->OnViewportChanges(ViewPortsManager::GetInstance().GetEditRenderControl()->GetRenderWindow());
	else
		m_AtmosphereManager->OnViewportChanges(ViewPortsManager::GetInstance().GetGameRenderControl()->GetRenderWindow());
}


void EditorScene::RemovePostProcessEffect(const Ogre::String &Name)
{
	std::vector<Ogre::String> oldList = m_PostProcessEffects;
	m_PostProcessEffects.clear();

	BEGIN_VECTOR_ITERATOR(oldList, Ogre::String)
		if (current != Name)
			m_PostProcessEffects.push_back(current);
	END_VECTOR_ITERATOR("")

	oldList.clear();
}


void EditorScene::AddPostProcessEffect(const Ogre::String &Name, int Index)
{
	if (Index != -1 && m_PostProcessEffects.size())
	{
		std::vector<Ogre::String> oldList = m_PostProcessEffects;
		m_PostProcessEffects.clear();

		BEGIN_VECTOR_ITERATOR(oldList, Ogre::String)
			if (i == Index)
				m_PostProcessEffects.push_back(Name);

			if (current != Name)
				m_PostProcessEffects.push_back(current);
		END_VECTOR_ITERATOR("")
		oldList.clear();
	}
	else
		m_PostProcessEffects.push_back(Name);
}


void EditorScene::Load(const Ogre::String &FilePath)
{
	Clear();

	int time = Time::GetInstance().GetRealTime();
	
	TreeParser *tp = new TreeParser(FilePath, Scene::TAG_SCENE);

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
		else if (elem->Name == TAG_CAMERA)
		{
			ReadCamera(elem);
		}
		else if (elem->Name == TAG_GAMEOBJECT)
		{
			ReadGameObject(elem, m_RootGameObject);
		}
	}

	delete tp;

	Log::GetInstance().AddInfo(Ogre::String("Scene [") + FilePath + "] loaded in " + Ogre::StringConverter::toString(Time::GetInstance().GetRealTime() - time) + "ms");
	
	RefreshSceneControlContent();

	Editor::GetInstance().ClearDoList();
}


void EditorScene::Save(const Ogre::String &FilePath, bool ForEditor)
{
	TreeParser tp = TreeParser();

	TreeElement *root = new TreeElement(tp.GetRoot());
	root->Name = ForEditor ? TAG_SCENE : TAG_LEVEL;

	Attributes *att = new Attributes;

	Serialize(att);
	
	att->Write(root);
	delete att;
	att = NULL;

	if (ForEditor)
	{
		TreeElement *elem = NULL;

		BEGIN_LIST_CONST_ITERATOR(ViewPortsManager::GetInstance().GetCameras(), EditorCamera)

			elem = new TreeElement(root);
			
			elem->Name = TAG_CAMERA;
			elem->Attributes.push_back(TreeElementAttribute("Type", Ogre::StringConverter::toString((int)current->GetType())));

			att = new Attributes;

			current->Serialize(att);

			att->Write(elem);

			delete att;
			att = NULL;

		END_LIST_CONST_ITERATOR
	}

	if (!m_RootGameObject)
		goto FinishUp;

	BEGIN_LIST_ITERATOR(GetChildren(), GameObject*)

		WriteGameObject(root, current);

	END_LIST_ITERATOR(NULL)

FinishUp:
	tp.WriteToFile(FilePath);
}


void EditorScene::PlayGame(void)
{
	delete m_Stats;
	m_Stats = new Stats(ViewPortsManager::GetInstance().GetGameRenderControl()->GetViewport(), m_SceneManager);
	
	if (ConnectFirstCamera())	
		HandleViewportChange();

	InitializePostProcessEffects();

	ScriptControl::GetPointer()->SaveAll();

	SetInitializeState();

	Start();
}


void EditorScene::StopGame(void)
{
	delete m_Stats;
	m_Stats = new Stats(ViewPortsManager::GetInstance().GetEditRenderControl()->GetViewport(), m_SceneManager);
	
	HandleViewportChange();

	ResetToInitializeState();

	RefreshSceneControlContent();
}


void EditorScene::SelectObject(GameObject *Object)
{
	if (m_SelectedGameObject)
	{
		m_SelectedGameObject->_GetSceneNode()->showBoundingBox(false);
	}

	m_SelectedGameObject = NULL;

	if (!Object)
		return;

	Editor::GetInstance().SetListener(this, Object);

	//if (!GetGameObject(Node) && GetGameObject(Node->getParentSceneNode())->GetType() != GameObject::GOT_MESH)
	////if (Node->getParentSceneNode() != m_SceneManager->getRootSceneNode())
	//{
	//	SelectNode(Node->getParentSceneNode());
	//	return;
	//}
	//else
		m_SelectedGameObject = Object;

	Object->_GetSceneNode()->showBoundingBox(true);
}


void EditorScene::SelectObject(unsigned int ObjectID)
{
	SelectObject(GetGameObject(ObjectID));
}


void EditorScene::RemoveSelectedGameObject(void)
{
	ScenesHolder::GetInstance().GetControl()->RemoveSelectedItem();
}


void EditorScene::RefreshSceneControlContent(void)
{
	ScenesHolder::GetInstance().GetControl()->ReloadContent();
}


void EditorScene::ReadCamera(const TreeElement *Element)
{
	Attributes *att = NULL;
	EditorCamera *camera = NULL;
	
	const std::string type = Element->GetAttributeValue("Type");

	camera = ViewPortsManager::GetInstance().GetCamera((EditorCamera::EditorCameraType)Ogre::StringConverter::parseInt(type));

	if (!camera)
	{
		Exception::GetInstance().Throw(Exception::ER_INVALID_PARAMETER, "Camera Type", Ogre::String("Camera with type number ") + type + " doesn't exists in EditorCameraType");

		return;
	}

	BOOST_FOREACH(const TreeElement *elem, Element->Children)
	{
		if (elem->Name == TAG_ATTRIBUTES)
		{
			att = new Attributes;
			att->Read(elem);
			camera->Deserialize(att);

			delete att;
			att = NULL;
		}
	}
}


void EditorScene::WriteGameObject(TreeElement *Element, GameObject *Object, bool ForExport)
{
	if (Object->GetWasRemove())
		return;

	TreeElement *gameObject = new TreeElement(Element);
	gameObject->Name = Scene::TAG_GAMEOBJECT;

	gameObject->Attributes.push_back(TreeElementAttribute("Name", Object->GetName()));
	gameObject->Attributes.push_back(TreeElementAttribute("Type", m_GameObjectFactory->GetStringOfType(Object->GetType())));

	TreeElement *elem = NULL;

	Attributes *att = new Attributes;

	Object->Serialize(att);

	att->Remove("Locked");

	att->Write(gameObject);

	att = NULL;
	delete att;

	BEGIN_LIST_ITERATOR(Object->GetChildren(), GameObject*)

		WriteGameObject(gameObject, current, ForExport);

	END_LIST_ITERATOR(NULL)
}


//void EditorScene::OnMouseClick(void)
//{
//	if (m_SelectedNode)
//	{
//		m_SelectedNode->showBoundingBox(false);
//	}
//
//	Ogre::MovableObject *object = GetSceneObject(OT_NEAREST_OBJECT);
//
//	if (object)
//	{
//		Ogre::SceneNode *node = (Ogre::SceneNode*)object->getParentNode();
//
//		GameObject *obj = GetGameObject(node);
//
//		if (!obj)
//			obj = GetGameObject(node->getParentSceneNode());
//		
//		if (obj)
//		{
//			ScenesHolder::GetInstance().GetControl()->SelectItem(obj->GetEditorText());
//
//			SelectNode(node);
//
//			obj = 0;
//		}
//		
//		node = NULL;
//	}
//	else
//	{
//		Editor::GetInstance().SeparateTranslateGizmo();
//
//		ScenesHolder::GetInstance().GetControl()->SelectItem("");
//	}
//
//	object = NULL;
//}


//Ogre::MovableObject *EditorScene::GetSceneObject(OBJECT_TYPE Type)
//{
//	m_RaySceneQuery->setRay(ViewPortsManager::GetInstance().GetViewpPortRay());
//
//	Ogre::RaySceneQueryResult &result = m_RaySceneQuery->execute();
//
//	Ogre::RaySceneQueryResult::iterator it = result.begin();
//
//	while (it != result.end())
//	{
//		if (/*it->movable && */it->movable->isVisible())
//		{
//			switch (Type)
//			{
//			case OT_NEAREST_OBJECT:
//				{
//					if (!Ogre::StringUtil::startsWith(it->movable->getName(), "gizmo", true))
//						return it->movable;
//				} break;
//				
//			case OT_GIZMO:
//				{
//					if (Ogre::StringUtil::startsWith(it->movable->getName(), "gizmo", true))
//						return it->movable;
//				} break;
//			}
//		}
//
//		it++;
//	}
//
//	return NULL;
//}
END_NAMESPACE

#endif