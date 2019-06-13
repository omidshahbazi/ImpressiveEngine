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

#include "Editor.h"
#include "Engine.h"
#include "ManualFunctionality.h"
#include "ScenesHolder.h"
#include "GameObject.h"
#include "Utility.h"
#include "ViewPortsManager.h"
#include "DoClasses.h"
#include "SceneControl.h"
#include "RenderControl.h"
#include "Log.h"
#include "HID.h"


BEGIN_NAMESPACE
Editor *Singleton<Editor>::m_Instance = NULL;

const unsigned int Editor::GIZMO_FLAG = 1000;


const char *AXIS_X_MATERIAL("GizmoRed");
const char *AXIS_Y_MATERIAL("GizmoGreen");
const char *AXIS_Z_MATERIAL("GizmoBlue");
const char *AXIS_SELECTED_MATERIAL("GizmoSelected");

const float GIZMO_SCALE = 200.f;


Editor::Editor(void) :
	m_EditorListener(NULL),
	m_TranslateType(TT_LOCAL),

	m_CurrentGizmoNode(NULL),
	m_CurrentGizmoNodeBase(NULL),

	m_LinkedNodeForTrans(NULL),

	m_CurrentDoItem(0)
{
}


//Editor::~Editor(void)
//{
//}


//bool Editor::queryResult(Ogre::MovableObject* obj, float distance)
//{
//	if (obj->getQueryFlags() == GIZMO_FLAG && obj->isVisible())
//	{
//		Log::GetInstance().AddInfo(obj->getName() + " - " + Ogre::StringConverter::toString(distance));
//	}
//
//	return true;
//}


void Editor::Initialize(void)
{
	Scene *scene = ScenesHolder::GetInstance().GetCurrentScene();
	//scene->AddResourceLocation("Data/Editor/Texture", "EditorTexture");
	//scene->AddResourceLocation("Data/Editor/Material", "EditorMaterial");
	//scene->AddResourceLocation("Data/Editor/Model", "EditorModel");

	//http://www.ogre3d.org/forums/viewtopic.php?f=2&t=56162

	const float fullValue = 1.f;

	const Ogre::ColourValue AXIS_X_COLOR(Ogre::ColourValue::Red);
	const Ogre::ColourValue AXIS_Y_COLOR(Ogre::ColourValue::Green);
	const Ogre::ColourValue AXIS_Z_COLOR(Ogre::ColourValue::Blue);
	const Ogre::ColourValue AXIS_SELECTED_COLOR(Ogre::ColourValue(1, 1, 0));

	ManualFunctionality::CreateColorMaterial(AXIS_X_COLOR, AXIS_X_MATERIAL, true);
	ManualFunctionality::CreateColorMaterial(AXIS_Y_COLOR, AXIS_Y_MATERIAL, true);
	ManualFunctionality::CreateColorMaterial(AXIS_Z_COLOR, AXIS_Z_MATERIAL, true);
	ManualFunctionality::CreateColorMaterial(AXIS_SELECTED_COLOR, AXIS_SELECTED_MATERIAL, true);

	const unsigned int segments = 10;

	Ogre::ManualObject *object = NULL;


	//
	// Translate Gizmo
	//

	const float arrowLength = 40.0f;
	const float arrowDiameter = 0.5f;
	const float arrowLengthHead = 8.0f;
	const float arrowDiameterHead = 2.0f;

	
	m_GizmoTranslateNode = scene->GetSceneManager()->getRootSceneNode()->createChildSceneNode("GizmoTranslateNode");

	m_GizmoTranslateNodeX = m_GizmoTranslateNode->createChildSceneNode("GizmoTranslateNodeX");
	object = ManualFunctionality::CreateManualObject(m_GizmoTranslateNodeX, "GizmoTranslateX", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddArrow(object, arrowLength, arrowDiameter, arrowLengthHead, arrowDiameterHead, segments, Ogre::Vector3(1, 0, 0), AXIS_X_MATERIAL, AXIS_X_COLOR);
	//m_GizmoTranslateNodeX->setVisible(false);

	m_GizmoTranslateNodeY = m_GizmoTranslateNode->createChildSceneNode("GizmoTranslateNodeY");
	object = ManualFunctionality::CreateManualObject(m_GizmoTranslateNodeY, "GizmoTranslateY", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddArrow(object, arrowLength, arrowDiameter, arrowLengthHead, arrowDiameterHead, segments, Ogre::Vector3(0, 1, 0), AXIS_Y_MATERIAL, AXIS_Y_COLOR);
	//m_GizmoTranslateNodeY->setVisible(false);

	m_GizmoTranslateNodeZ = m_GizmoTranslateNode->createChildSceneNode("GizmoTranslateNodeZ");
	object = ManualFunctionality::CreateManualObject(m_GizmoTranslateNodeZ, "GizmoTranslateZ", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddArrow(object, arrowLength, arrowDiameter, arrowLengthHead, arrowDiameterHead, segments, Ogre::Vector3(0, 0, 1), AXIS_Z_MATERIAL, AXIS_Z_COLOR);
	//m_GizmoTranslateNodeZ->setVisible(false);

	m_GizmoTranslateNode->setVisible(false);


	//
	// Rotate Gizmo
	//

	const float circleRadius = 40.0f;
	const float circleWidth = 0.4f;
	const unsigned int circleSegments = 100;
	
	m_GizmoRotateNode = scene->GetSceneManager()->getRootSceneNode()->createChildSceneNode("GizmoRotateNode");

	m_GizmoRotateNodeX = m_GizmoRotateNode->createChildSceneNode("GizmoRotateNodeX");
	object = ManualFunctionality::CreateManualObject(m_GizmoRotateNodeX, "GizmoRotateX", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddCircle(object, circleRadius, circleWidth, circleSegments, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_X, AXIS_X_MATERIAL, AXIS_X_COLOR);
	//m_GizmoRotateNodeX->setVisible(false);

	m_GizmoRotateNodeY = m_GizmoRotateNode->createChildSceneNode("GizmoRotateNodeY");
	object = ManualFunctionality::CreateManualObject(m_GizmoRotateNodeY, "GizmoRotateY", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddCircle(object, circleRadius, circleWidth, circleSegments, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_Y, AXIS_Y_MATERIAL, AXIS_Y_COLOR);
	//m_GizmoRotateNodeY->setVisible(false);

	m_GizmoRotateNodeZ = m_GizmoRotateNode->createChildSceneNode("GizmoRotateNodeZ");
	object = ManualFunctionality::CreateManualObject(m_GizmoRotateNodeZ, "GizmoRotateZ", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddCircle(object, circleRadius, circleWidth, circleSegments, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_Z, AXIS_Z_MATERIAL, AXIS_Z_COLOR);
	//m_GizmoRotateNodeZ->setVisible(false);

	m_GizmoRotateNode->setVisible(false);


	//
	// Scale Gizmo
	//

	const float scaleLength = 40.0f;
	const float scaleDiameter = 0.2f;
	const float scaleSizeHead = 3.0f;
	
	m_GizmoScaleNode = scene->GetSceneManager()->getRootSceneNode()->createChildSceneNode("GizmoScaleNode");

	m_GizmoScaleNodeX = m_GizmoScaleNode->createChildSceneNode("GizmoScaleNodeX");
	object = ManualFunctionality::CreateManualObject(m_GizmoScaleNodeX, "GizmoScaleX", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddLineBox(object, scaleLength, scaleDiameter, segments, scaleSizeHead, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_X, AXIS_X_MATERIAL, AXIS_X_COLOR);
	//m_GizmoScaleNodeX->setVisible(false);

	m_GizmoScaleNodeY = m_GizmoScaleNode->createChildSceneNode("GizmoScaleNodeY");
	object = ManualFunctionality::CreateManualObject(m_GizmoScaleNodeY, "GizmoScaleY", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddLineBox(object, scaleLength, scaleDiameter, segments, scaleSizeHead, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_Y, AXIS_Y_MATERIAL, AXIS_Y_COLOR);
	//m_GizmoScaleNodeY->setVisible(false);

	m_GizmoScaleNodeZ = m_GizmoScaleNode->createChildSceneNode("GizmoScaleNodeZ");
	object = ManualFunctionality::CreateManualObject(m_GizmoScaleNodeZ, "GizmoScaleZ", Ogre::RENDER_QUEUE_9);
	object->setQueryFlags(GIZMO_FLAG);
	ManualFunctionality::AddLineBox(object, scaleLength, scaleDiameter, segments, scaleSizeHead, Ogre::Vector3::ZERO, Ogre::Vector3::UNIT_Z, AXIS_Z_MATERIAL, AXIS_Z_COLOR);
	//m_GizmoScaleNodeZ->setVisible(false);

	m_GizmoScaleNode->setVisible(false);


	SetTransformType(TT_NONE);

	m_RaySceneQuery = scene->GetSceneManager()->createRayQuery(Ogre::Ray());
	//m_RaySceneQuery->setSortByDistance(true);
}


void Editor::Update(void)
{
	const Ogre::Ray &ray = ViewPortsManager::GetInstance().GetViewpPortRay();

	//Log::GetInstance().Clear();

	const HID &hid = HID::GetInstance();

	if (!hid.IsControlDown())
	{
		if (m_CurrentGizmoNode)
		{
			if (!hid.IsMouseDown(HID::M_LEFT))
			{
				SetCurrentAxisMaterialToIdle();

				m_CurrentGizmoNode = NULL;
				//Log::GetInstance().AddInfo("Released");
			}
			else
			{
				//Log::GetInstance().AddInfo("In Move");

				Ogre::Plane plane;
				Ogre::Node *nodeToApply = m_CurrentGizmoNode->getParent();

				if (m_CurrentGizmoNode == m_GizmoTranslateNodeX) 
				{
					plane.normal = Ogre::Vector3::UNIT_Z;
					plane.d = nodeToApply->getPosition().z;
				}
				else if (m_CurrentGizmoNode == m_GizmoTranslateNodeY)
				{
					plane.normal = Ogre::Vector3::UNIT_Z;
					plane.d = nodeToApply->getPosition().z;
				}
				else if (m_CurrentGizmoNode == m_GizmoTranslateNodeZ)
				{
					plane.normal = Ogre::Vector3::UNIT_Y;
					plane.d = nodeToApply->getPosition().y;
				}


				std::pair<bool, float> result = ray.intersects(plane);

				if (result.first)
				{
					//Log::GetInstance().AddInfo(Ogre::StringConverter::toString(m_GizmoSelectOffset));
					Ogre::Vector3 point = ray.getPoint(result.second) + m_GizmoSelectOffset;
					//m_GizmoSelectOffset = Ogre::Vector3::ZERO;

					if (m_CurrentGizmoNode == m_GizmoTranslateNodeX)
					{
						point.y = nodeToApply->getPosition().y;
						point.z = nodeToApply->getPosition().z;
					}
					else if (m_CurrentGizmoNode == m_GizmoTranslateNodeY)
					{
						point.x = nodeToApply->getPosition().x;
						point.z = nodeToApply->getPosition().z;
					}
					else if (m_CurrentGizmoNode == m_GizmoTranslateNodeZ)
					{
						point.x = nodeToApply->getPosition().x;
						point.y = nodeToApply->getPosition().y;
					}

					nodeToApply->setPosition(point);
				
					//Log::GetInstance().AddInfo(Ogre::StringConverter::toString(m_GizmoSelectOffset));
					m_EditorListener->OnTranslate(point);
				}
			}
		}
		else if (hid.IsMouseDown(HID::M_LEFT))
		{
			m_RaySceneQuery->setRay(ray);
			//m_RaySceneQuery->execute(this);
			Ogre::RaySceneQueryResult &result = m_RaySceneQuery->execute();

			Ogre::RaySceneQueryResult::iterator it = result.begin();

			while (it != result.end())
			{
				if (it->movable->getQueryFlags() == GIZMO_FLAG && it->movable->isVisible())
				{
					//Ogre::String str = it->movable->getName() + " ----- " + Ogre::StringConverter::toString(it->distance);

					m_CurrentGizmoNode = it->movable->getParentSceneNode();

					SetCurrentAxisMaterialToSelected();

					const Ogre::Vector3 &pos = m_CurrentGizmoNode->getParent()->getPosition();
					
					m_GizmoSelectOffset = pos - ray.getPoint(it->distance);

					//if (pos.y < 0)
						//m_GizmoSelectOffset.y *= -1;

					//m_GizmoSelectOffset = Ogre::Vector3(0, 30, 0);
					Log::GetInstance().AddInfo(Ogre::StringConverter::toString(m_GizmoSelectOffset));

					break;
				}

				it++;
			}
		}
	}

	if (m_CurrentGizmoNodeBase)
		m_CurrentGizmoNodeBase->setScale(Ogre::Vector3::UNIT_SCALE * (ViewPortsManager::GetInstance().GetCurrentCamera()->GetPosition() - m_CurrentGizmoNodeBase->getPosition()).length() / GIZMO_SCALE);
}


bool Editor::ExportGame(const Ogre::String &CurrentScene, const std::vector<Ogre::String> &Scenes)
{
	std::vector<Ogre::String> files = GetFiles(Engine::PATH_LEVEL);
	BEGIN_VECTOR_ITERATOR(files, Ogre::String)
		DeleteFile(current.c_str());
	END_VECTOR_ITERATOR("")
	files.clear();

	Ogre::String fileName = "";
	BEGIN_VECTOR_ITERATOR(Scenes, Ogre::String)
		ScenesHolder::GetInstance().GetCurrentScene()->Load(current);

		Ogre::StringUtil::splitFullFilename(current, fileName, current, current);

		ScenesHolder::GetInstance().GetCurrentScene()->Save(Engine::PATH_LEVEL + fileName + ".level", false);
	END_VECTOR_ITERATOR("")

	if (CurrentScene.size())
		ScenesHolder::GetInstance().GetCurrentScene()->Load(CurrentScene);

	return true;
}


void Editor::ClearDoList(void)
{
	m_DoList.Clear();

	m_CurrentDoItem = 0;
}


void Editor::DoRedo(void)
{
	m_CurrentDoItem++;

	if (m_CurrentDoItem == m_DoList.Count())
		m_CurrentDoItem = m_DoList.Count() - 1;

	if (m_CurrentDoItem < m_DoList.Count())
		((DoBase*)m_DoList.Item(m_CurrentDoItem))->Do();
}


void Editor::DoUndo(void)
{
	if (m_CurrentDoItem < m_DoList.Count())
		((DoBase*)m_DoList.Item(m_CurrentDoItem))->Undo();

	m_CurrentDoItem--;

	if (m_CurrentDoItem < 0)
		m_CurrentDoItem = 0;
}


void Editor::FocusGameObject()
{
	//GameObject* sgo = SceneControl::GetInstance().GetSelectedObject();

	//ScenesHolder::GetInstance().GetCurrentScene()->GetCamera(Scene::Get);
	if (ScenesHolder::GetInstance().GetCurrentScene()->GetSelectedObject())
	{
		ViewPortsManager::GetInstance().GetEditRenderControl()->FocusOn(ScenesHolder::GetInstance().GetCurrentScene()->GetSelectedObject());
	}
	else
	{
		Log::GetInstance().AddError("Please select a Gameobject first");
	}
	//ViewPortsManager::GetInstance().GetCurrentCamera()->SetPosition(ScenesHolder::GetInstance().GetCurrentScene()->GetSelectedObject()->GetPosition() - 30);

}


void Editor::AddRemoveGameObjectDo(GameObject *Object)
{
	AddDoBase(new RemoveGameObjectDo(Object));
}


void Editor::AddCreateGameObjectDo(GameObject *Object)
{
	AddDoBase(new RemoveGameObjectDo(Object, true));
}


void Editor::LinkTranslateGizmoTo(Ogre::SceneNode *Node)
{
	if (Node)
	{
		m_LinkedNodeForTrans = Node;

		//m_CurrentGizmoNode->setVisible(true);
	}
}


void Editor::SeparateTranslateGizmo(Ogre::SceneNode *Node)
{
	if (Node)
	{
		if (Node == m_LinkedNodeForTrans)
			m_LinkedNodeForTrans = NULL;
	}
	else
	{
		m_LinkedNodeForTrans = NULL;
	}

	//m_CurrentGizmoNode->setVisible(false);
}


DoBase *Editor::GetLastDo(void)
{
	return (DoBase*)m_DoList.Last();
}


void Editor::AddDoBase(DoBase *Do, bool DoIt)
{
	if (m_DoList.Count() > 0 && m_CurrentDoItem < m_DoList.Count() - 1)
		m_DoList.RemoveAt(m_CurrentDoItem, m_DoList.Count() - m_CurrentDoItem);

	m_DoList.push_back(Do);

	if (DoIt)
		GetLastDo()->Do();

	m_CurrentDoItem = m_DoList.Count() - 1;
}


void Editor::SetListener(EditorListener *Listener, GameObject *GameObject)
{
	m_EditorListener = Listener;

	m_GizmoTranslateNode->setPosition(GameObject->GetPosition());
	m_GizmoRotateNode->setPosition(GameObject->GetPosition());
	m_GizmoScaleNode->setPosition(GameObject->GetPosition());

	SetTransformType(m_TransformType);
}


void Editor::SetTransformType(TRANSFORM_TYPE Type)
{
	m_TransformType = Type;

	if (m_CurrentGizmoNodeBase)
		m_CurrentGizmoNodeBase->setVisible(false);

	if (!m_EditorListener)
		return;

	switch (m_TransformType)
	{
	case TT_NONE:
		{
			m_CurrentGizmoNodeBase = NULL;
		} break;

	case TT_TRANSLATE:
		{
			m_CurrentGizmoNodeBase = m_GizmoTranslateNode;
		} break;

	case TT_ROTATE:
		{
			m_CurrentGizmoNodeBase = m_GizmoRotateNode;
		} break;

	case TT_SCALE:
		{
			m_CurrentGizmoNodeBase = m_GizmoScaleNode;
		} break;
	}
	
	if (m_CurrentGizmoNodeBase)
		m_CurrentGizmoNodeBase->setVisible(true);
}


void Editor::SetCurrentAxisMaterialToIdle(void)
{
	Ogre::ManualObject *object = (Ogre::ManualObject*)m_CurrentGizmoNode->getAttachedObject(0);

	const char *materialName = "";

	if (m_CurrentGizmoNode == m_GizmoTranslateNodeX) materialName = AXIS_X_MATERIAL;
	else if (m_CurrentGizmoNode == m_GizmoTranslateNodeY) materialName = AXIS_Y_MATERIAL;
	else if (m_CurrentGizmoNode == m_GizmoTranslateNodeZ) materialName = AXIS_Z_MATERIAL;
	
	else if (m_CurrentGizmoNode == m_GizmoRotateNodeX) materialName = AXIS_X_MATERIAL;
	else if (m_CurrentGizmoNode == m_GizmoRotateNodeY) materialName = AXIS_Y_MATERIAL;
	else if (m_CurrentGizmoNode == m_GizmoRotateNodeZ) materialName = AXIS_Z_MATERIAL;
	
	else if (m_CurrentGizmoNode == m_GizmoScaleNodeX) materialName = AXIS_X_MATERIAL;
	else if (m_CurrentGizmoNode == m_GizmoScaleNodeY) materialName = AXIS_Y_MATERIAL;
	else if (m_CurrentGizmoNode == m_GizmoScaleNodeZ) materialName = AXIS_Z_MATERIAL;

	for (unsigned int i = 0; i < object->getNumSections(); i++)
		object->setMaterialName(i, materialName);
}


void Editor::SetCurrentAxisMaterialToSelected(void)
{
	Ogre::ManualObject *object = (Ogre::ManualObject*)m_CurrentGizmoNode->getAttachedObject(0);

	for (unsigned int i = 0; i < object->getNumSections(); i++)
		object->setMaterialName(i, AXIS_SELECTED_MATERIAL);
}

END_NAMESPACE

#endif