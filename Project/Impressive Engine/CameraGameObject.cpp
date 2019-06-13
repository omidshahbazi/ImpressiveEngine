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
#include "CameraGameObject.h"
#include "Scene.h"
#include "HID.h"
#include "Engine.h"
#include "ScriptRegister.h"
#include "EditorMaterialGenerator.h"

#ifdef EDIT_MODE
#include "MainWindow.h"
#include "ViewPortsManager.h"

const float BOX_SCALE = 2000.f;
#endif


BEGIN_NAMESPACE
CameraGameObject::CameraGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent) :
	GameObject(Name, Holder, Parent),
	m_IsMain(false)
{
	m_Camera = m_Holder->GetSceneManager()->createCamera(m_Holder->GetNewCameraName());

    m_Camera->setNearClipDistance(5);
	m_Camera->lookAt(0, 0, 0);

	m_SceneNode->attachObject(m_Camera);

#ifdef EDIT_MODE
	m_Box = m_SceneNode->createChildSceneNode(m_Camera->getName() + "|BoxNode");
	m_BoxEntity = m_Holder->GetSceneManager()->createEntity(m_Camera->getName() + "|Box", Ogre::SceneManager::PT_CUBE);
	m_BoxEntity->setCastShadows(false);
	m_BoxEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
	m_BoxEntity->setMaterialName(EditorMaterialGenerator::GetInstance().GetMaterial(EditorMaterialGenerator::EU_CAMERA)->getName());
	m_Box->attachObject(m_BoxEntity);
	//((Ogre::Entity*)m_Box->getAttachedObject(0))->setMaterialName("Camera");
	m_Box->setScale(0.1f, 0.1f, 0.1f);
	
	m_Box->setUserAny(Ogre::Any((void*)this));
#endif
}

CameraGameObject::~CameraGameObject(void)
{
	m_Holder->GetSceneManager()->destroyCamera(m_Camera);
	m_Camera = NULL;

#ifdef EDIT_MODE
	m_Holder->GetSceneManager()->destroyEntity(m_BoxEntity);
	m_BoxEntity = NULL;

	m_Holder->GetSceneManager()->destroySceneNode(m_Box);
	m_Box = NULL;
#endif
}

#ifdef EDIT_MODE
bool CameraGameObject::Update(void)
{
	m_Box->setScale(Ogre::Vector3::UNIT_SCALE * (ViewPortsManager::GetInstance().GetCurrentCamera()->GetPosition() - m_Box->getPosition()).length() / BOX_SCALE);

	if (MainWindow::GetPointer()->IsInEditMode())
		return true;

	return GameObject::Update();
}


bool CameraGameObject::Serialize(Attributes *Attributes)
{
	GameObject::Serialize(Attributes);
	
	Attributes->AddBoolean("IsMain", GetIsMain());
	Attributes->AddInteger("PolygonMode", (int)GetPolygonMode());
	Attributes->AddFloat("LODBias", GetLODBias());
	Attributes->AddFloat("FOVY", GetFOVY());
	Attributes->AddFloat("FarClipDistance", GetFarClipDistance());
	Attributes->AddFloat("NearClipDistance", GetNearClipDistance());
	Attributes->AddBoolean("AutoAspectRatio", GetAutoAspectRatio());
	Attributes->AddFloat("AspectRatio", GetAspectRatio());
	Attributes->AddInteger("ProjectionType", (int)GetProjectionType());

	return true;
}
#endif

bool CameraGameObject::Deserialize(Attributes *Attributes)
{
	GameObject::Deserialize(Attributes);
	
	SetIsMain(Attributes->GetBoolean("IsMain"));
	SetPolygonMode((Ogre::PolygonMode)Attributes->GetInteger("PolygonMode"));
	SetLODBias(Attributes->GetFloat("LODBias"));
	SetFOVY(Attributes->GetFloat("FOVY"));
	SetFarClipDistance(Attributes->GetFloat("FarClipDistance"));
	SetNearClipDistance(Attributes->GetFloat("NearClipDistance"));
	SetAutoAspectRatio(Attributes->GetBoolean("AutoAspectRatio"));
	if (!GetAutoAspectRatio())
		SetAspectRatio(Attributes->GetFloat("AspectRatio"));
	SetProjectionType((Ogre::ProjectionType)Attributes->GetInteger("ProjectionType"));

	return true;
}


void CameraGameObject::SetInitializeState(void)
{
	GameObject::SetInitializeState();

	m_Init_Orientation = GetOrientation();
	
#ifdef EDIT_MODE
	m_Box->setVisible(false);
#endif

	m_Init_PolygonMode = GetPolygonMode();
	m_Init_Position = m_Camera->getPosition();
	m_Init_Orientation = m_Camera->getOrientation();
	m_Init_LODBias = GetLODBias();
	m_Init_FOVY = GetFOVY();
	m_Init_FarClipDistance = GetFarClipDistance();
	m_Init_NearClipDistance = GetNearClipDistance();
	m_Init_AutoAspectRatio = GetAutoAspectRatio();
	m_Init_AspectRatio = GetAspectRatio();
	m_Init_ProjectionType = GetProjectionType();
	m_Init_OrthoWindow = GetOrthoWindow();
}


void CameraGameObject::ResetToInitializeState(void)
{
	GameObject::ResetToInitializeState();

	SetOrientation(m_Init_Orientation);
	
#ifdef EDIT_MODE
	m_Box->setVisible(true);
#endif
	
	SetPolygonMode(m_Init_PolygonMode);
	m_Camera->setPosition(m_Init_Position);
	m_Camera->setOrientation(m_Init_Orientation);
	SetLODBias(m_Init_LODBias);
	SetFOVY(m_Init_FOVY);
	SetFarClipDistance(m_Init_FarClipDistance);
	SetNearClipDistance(m_Init_NearClipDistance);
	SetAutoAspectRatio(m_Init_AutoAspectRatio);
	if (!GetAutoAspectRatio())
		SetAspectRatio(m_Init_AspectRatio);
	SetProjectionType(m_Init_ProjectionType);
	SetOrthoWindow(m_Init_OrthoWindow.x, m_Init_OrthoWindow.y);
}

#ifdef EDIT_MODE
void CameraGameObject::BeginChangeProperties(void)
{
	m_SetThisOnViewPort = false;

	if (ViewPortsManager::GetInstance().GetGameRenderControl()->GetOgreCamera() == m_Camera)
	{
		m_SetThisOnViewPort = true;
		ViewPortsManager::GetInstance().GetGameRenderControl()->SetCamera(NULL);
	}
}


void CameraGameObject::EndChangeProperties(void)
{
	if (m_SetThisOnViewPort)
		ViewPortsManager::GetInstance().GetGameRenderControl()->SetCamera(m_Camera);
}
#endif

bool CameraGameObject::SetFarClipDistance(Ogre::Real FarPlane)
{
	if (FarPlane != 0 || Engine::GetInstance().GetGraphic()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
		m_Camera->setFarClipDistance(FarPlane);
		return true;
	}

	return false;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(CameraGameObject)
	BEGIN_DEFINE_TYPE_BASE(GameObject, CameraGameObject, "CameraGameObject")
		DEFINE_FUNCTION("SetIsMain", &CameraGameObject::SetIsMain)
		DEFINE_FUNCTION("GetIsMain", &CameraGameObject::GetIsMain)

		DEFINE_FUNCTION("SetPolygonMode", &CameraGameObject::SetPolygonMode)
		DEFINE_FUNCTION("GetPolygonMode", &CameraGameObject::GetPolygonMode)
		
		DEFINE_FUNCTION("Move", &CameraGameObject::Move)
		DEFINE_FUNCTION("MoveRelative", &CameraGameObject::MoveRelative)
		
		DEFINE_FUNCTION("LookAt", (void(CameraGameObject::*)(const Ogre::Vector3&))&CameraGameObject::LookAt)
		DEFINE_FUNCTION("LookAt", (void(CameraGameObject::*)(float, float, float))&CameraGameObject::LookAt)
		
		DEFINE_FUNCTION("SetDirection", (void(CameraGameObject::*)(const Ogre::Vector3&))&CameraGameObject::SetDirection)
		DEFINE_FUNCTION("SetDirection", (void(CameraGameObject::*)(float, float, float))&CameraGameObject::SetDirection)
		DEFINE_FUNCTION("GetDirection", &CameraGameObject::GetDirection)
		
		DEFINE_FUNCTION("SetLODBias", &CameraGameObject::SetLODBias)
		DEFINE_FUNCTION("GetLODBias", &CameraGameObject::GetLODBias)

		DEFINE_FUNCTION("GetCameraToViewportRay", &CameraGameObject::GetCameraToViewportRay)

		DEFINE_FUNCTION("SetFOVY", &CameraGameObject::SetFOVY)
		DEFINE_FUNCTION("GetFOVY", &CameraGameObject::GetFOVY)

		DEFINE_FUNCTION("SetFarClipDistance", &CameraGameObject::SetFarClipDistance)
		DEFINE_FUNCTION("GetFarClipDistance", &CameraGameObject::GetFarClipDistance)

		DEFINE_FUNCTION("SetNearClipDistance", &CameraGameObject::SetNearClipDistance)
		DEFINE_FUNCTION("GetNearClipDistance", &CameraGameObject::GetNearClipDistance)

		DEFINE_FUNCTION("SetAspectRatio", &CameraGameObject::SetAspectRatio)
		DEFINE_FUNCTION("GetAspectRatio", &CameraGameObject::GetAspectRatio)
		
		DEFINE_FUNCTION("SetAutoTracking", (void(CameraGameObject::*)(GameObject*))&CameraGameObject::SetAutoTracking)
		DEFINE_FUNCTION("SetAutoTracking", (void(CameraGameObject::*)(GameObject*, const Ogre::Vector3&))&CameraGameObject::SetAutoTracking)

		DEFINE_FUNCTION("SetProjectionType", &CameraGameObject::SetProjectionType)
		DEFINE_FUNCTION("GetProjectionType", &CameraGameObject::GetProjectionType)

		DEFINE_FUNCTION("SetOrthoWindow", &CameraGameObject::SetOrthoWindow)
		DEFINE_FUNCTION("GetOrthoWindow", &CameraGameObject::GetOrthoWindow)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE