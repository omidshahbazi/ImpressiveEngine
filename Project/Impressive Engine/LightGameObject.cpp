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
#include "LightGameObject.h"
#include "Scene.h"
#include "ManualFunctionality.h"
#include "ScriptRegister.h"
#include "DeferredShadingSystem.h"
#include "EditorMaterialGenerator.h"

#ifdef EDIT_MODE
#include "ViewPortsManager.h"

const float BOX_SCALE = 2000.f;
#endif


BEGIN_NAMESPACE
LightGameObject::LightGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent) :
	GameObject(Name, Holder, Parent),
	m_Light(NULL)
	//m_BillboardSet(NULL)
{
	//m_Light = m_Holder->GetSceneManager()->createLight(m_Holder->GetNewLightName());

	m_Light = m_Holder->GetDeferredShadingSystem()->CreateLight();
		
	//m_Light->setAttenuation(1.0f, 0.001f, 0.004f);
	//m_Light->setDiffuseColour(0,1,1);
	//m_Light->setSpecularColour(0.25,1.0,1.0); // Cyan light has specular component

	m_SceneNode->attachObject(m_Light->_GetOgreLight());

#ifdef EDIT_MODE
	m_Box = m_SceneNode->createChildSceneNode(m_Light->_GetOgreLight()->getName() + "|BoxNode");
	//m_Box->setPosition(10000, 100, 100);
	m_BoxEntity = m_Holder->GetSceneManager()->createEntity(m_Light->_GetOgreLight()->getName() + "|Box", Ogre::SceneManager::PT_CUBE);
	m_BoxEntity->setCastShadows(false);
	m_BoxEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
	m_BoxEntity->setMaterialName(EditorMaterialGenerator::GetInstance().GetMaterial(EditorMaterialGenerator::EU_LIGHT)->getName());
	m_Box->attachObject(m_BoxEntity);
	//((Ogre::Entity*)m_Box->getAttachedObject(0))->setMaterialName("Light");
	m_Box->setScale(0.1f, 0.1f, 0.1f);

	m_Box->setUserAny(Ogre::Any((void*)this));

	//m_BillboardSet = m_Holder->GetSceneManager()->createBillboardSet(m_Light->getName() + "|Billboard", 1);
	//m_BillboardSet->setMaterialName("Light");
	//m_BillboardSet->setDefaultDimensions(10, 10);
	//m_BillboardSet->createBillboard(Ogre::Vector3::ZERO);
	//AttachObject(m_BillboardSet);
#endif
}


LightGameObject::~LightGameObject(void)
{
	//m_Holder->GetSceneManager()->destroyLight(m_Light);
	//if (m_Light)
	//	m_Holder->GetSceneManager()->destroyMovableObject(m_Light);

	m_Holder->GetDeferredShadingSystem()->DestroyLight(m_Light);

	m_Light = NULL;

#ifdef EDIT_MODE
	m_Holder->GetSceneManager()->destroyEntity(m_BoxEntity);
	m_BoxEntity = NULL;

	m_Holder->GetSceneManager()->destroySceneNode(m_Box);
	m_Box = NULL;

	//Ogre::BillboardSet *m_BillboardSet;
#endif
}

#ifdef EDIT_MODE
bool LightGameObject::Update(void)
{
	m_Box->setScale(Ogre::Vector3::UNIT_SCALE * (ViewPortsManager::GetInstance().GetCurrentCamera()->GetPosition() - m_Box->getPosition()).length() / BOX_SCALE);

	return GameObject::Update();
}


bool LightGameObject::Serialize(Attributes *Attributes)
{
	GameObject::Serialize(Attributes);

	Attributes->AddInteger("Type", (int)GetLightType());
	//Attributes->AddBoolean("CastShadows", GetCastShadows());

	Attributes->AddVector3("Direction", GetDirection());

	Attributes->AddFloat("InnerAngle", GetSpotlightInnerAngle());
	Attributes->AddFloat("OuterAngle", GetSpotlightOuterAngle());
	Attributes->AddFloat("Falloff", GetSpotlightFalloff());

	Attributes->AddColor("DiffuseColour", GetDiffuseColour());
	Attributes->AddColor("SpecularColour", GetSpecularColour());

	Attributes->AddFloat("AttenuationRange", GetAttenuationRange());
	Attributes->AddFloat("AttenuationConstant", GetAttenuationConstant());
	Attributes->AddFloat("AttenuationLinear", GetAttenuationLinear());
	Attributes->AddFloat("AttenuationQuadric", GetAttenuationQuadric());

	//Attributes->AddFloat("PowerScale", GetPowerScale());

	return true;
}
#endif

bool LightGameObject::Deserialize(Attributes *Attributes)
{
	GameObject::Deserialize(Attributes);

	SetLightType((Light::LightType)Attributes->GetInteger("Type"));
	//SetCastShadows(Attributes->GetBoolean("CastShadows"));

	SetDirection(Attributes->GetVector3("Direction"));

	SetSpotlightInnerAngle(Attributes->GetFloat("InnerAngle"));
	SetSpotlightOuterAngle(Attributes->GetFloat("OuterAngle"));
	SetSpotlightFalloff(Attributes->GetFloat("Falloff"));

	SetDiffuseColour(Attributes->GetColor("DiffuseColour"));
	SetSpecularColour(Attributes->GetColor("SpecularColour"));

	SetAttenuationRange(Attributes->GetFloat("AttenuationRange"));
	SetAttenuationConstant(Attributes->GetFloat("AttenuationConstant"));
	SetAttenuationLinear(Attributes->GetFloat("AttenuationLinear"));
	SetAttenuationQuadric(Attributes->GetFloat("AttenuationQuadric"));

	//SetPowerScale(Attributes->GetFloat("PowerScale"));

	return true;
}


void LightGameObject::SetInitializeState(void)
{
	GameObject::SetInitializeState();

	m_Init_Type = GetLightType();
	m_Init_Direction = GetDirection();
	m_Init_SpotlightInnerAngle = GetSpotlightInnerAngle();
	m_Init_SpotlightOuterAngle = GetSpotlightOuterAngle();
	m_Init_SpotlightFalloff = GetSpotlightFalloff();
	m_Init_DiffuseColour = GetDiffuseColour();
	m_Init_SpecularColour = GetSpecularColour();
	m_Init_AttenuationRange = GetAttenuationRange();
	m_Init_AttenuationConstant = GetAttenuationConstant();
	m_Init_AttenuationLinear = GetAttenuationLinear();
	m_Init_AttenuationQuadric = GetAttenuationQuadric();
	//m_Init_PowerScale = GetPowerScale();
	
#ifdef EDIT_MODE
	m_Box->setVisible(false);
#endif
}


void LightGameObject::ResetToInitializeState(void)
{
	GameObject::ResetToInitializeState();
	
	SetLightType(m_Init_Type);
	SetDirection(m_Init_Direction);
	SetSpotlightInnerAngle(m_Init_SpotlightInnerAngle);
	SetSpotlightOuterAngle(m_Init_SpotlightOuterAngle);
	SetSpotlightFalloff(m_Init_SpotlightFalloff);
	SetDiffuseColour(m_Init_DiffuseColour);
	SetSpecularColour(m_Init_SpecularColour);
	SetAttenuationRange(m_Init_AttenuationRange);
	SetAttenuationConstant(m_Init_AttenuationConstant);
	SetAttenuationLinear(m_Init_AttenuationLinear);
	SetAttenuationQuadric(m_Init_AttenuationQuadric);
	//SetPowerScale(m_Init_PowerScale);
	
#ifdef EDIT_MODE
	m_Box->setVisible(true);
#endif
}


const bool LightGameObject::GetEnabled(void)
{
	return m_Light->GetEnabled();
}


void LightGameObject::SetEnabled(const bool &Value)
{
	m_Light->SetEnabled(Value);
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(LightGameObject)
	BEGIN_DEFINE_TYPE_BASE(GameObject, LightGameObject, "LightGameObject")
		DEFINE_FUNCTION("GetEnabled", &LightGameObject::GetEnabled)
		DEFINE_FUNCTION("SetEnabled", &LightGameObject::SetEnabled)
		
		//DEFINE_FUNCTION("SetLightType", &LightGameObject::SetLightType)
		//DEFINE_FUNCTION("GetLightType", &LightGameObject::GetLightType)
		//
		//DEFINE_FUNCTION("SetDirection", (void(LightGameObject::*)(const Ogre::Vector3&))&LightGameObject::SetDirection)
		//DEFINE_FUNCTION("SetDirection", (void(LightGameObject::*)(float, float, float))&LightGameObject::SetDirection)
		//DEFINE_FUNCTION("GetDirection", &LightGameObject::GetDirection)
		//
		//DEFINE_FUNCTION("SetSpotlightInnerAngle", &LightGameObject::SetSpotlightInnerAngle)
		//DEFINE_FUNCTION("GetSpotlightInnerAngle", &LightGameObject::GetSpotlightInnerAngle)
		//
		//DEFINE_FUNCTION("SetSpotlightOuterAngle", &LightGameObject::SetSpotlightOuterAngle)
		//DEFINE_FUNCTION("GetSpotlightOuterAngle", &LightGameObject::GetSpotlightOuterAngle)
		//
		//DEFINE_FUNCTION("SetSpotlightFalloff", &LightGameObject::SetSpotlightFalloff)
		//DEFINE_FUNCTION("GetSpotlightFalloff", &LightGameObject::GetSpotlightFalloff)
		//
		//DEFINE_FUNCTION("SetDiffuseColour", &LightGameObject::SetDiffuseColour)
		//DEFINE_FUNCTION("GetDiffuseColour", &LightGameObject::GetDiffuseColour)
		//
		//DEFINE_FUNCTION("SetSpecularColour", &LightGameObject::SetSpecularColour)
		//DEFINE_FUNCTION("GetSpecularColour", &LightGameObject::GetSpecularColour)
		//
		//DEFINE_FUNCTION("SetAttenuationRange", &LightGameObject::SetAttenuationRange)
		//DEFINE_FUNCTION("GetAttenuationRange", &LightGameObject::GetAttenuationRange)
		//
		//DEFINE_FUNCTION("SetAttenuationConstant", &LightGameObject::SetAttenuationConstant)
		//DEFINE_FUNCTION("GetAttenuationConstant", &LightGameObject::GetAttenuationConstant)
		//
		//DEFINE_FUNCTION("SetAttenuationLinear", &LightGameObject::SetAttenuationLinear)
		//DEFINE_FUNCTION("GetAttenuationLinear", &LightGameObject::GetAttenuationLinear)
		//
		//DEFINE_FUNCTION("SetAttenuationQuadric", &LightGameObject::SetAttenuationQuadric)
		//DEFINE_FUNCTION("GetAttenuationQuadric", &LightGameObject::GetAttenuationQuadric)
		//
		//DEFINE_FUNCTION("SetPowerScale", &LightGameObject::SetPowerScale)
		//DEFINE_FUNCTION("GetPowerScale", &LightGameObject::GetPowerScale)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE