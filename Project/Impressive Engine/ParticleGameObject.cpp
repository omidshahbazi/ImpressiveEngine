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
#include "ParticleGameObject.h"
#include "Scene.h"
#include "Exception.h"


BEGIN_NAMESPACE
ParticleGameObject::ParticleGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent) :
	GameObject(Name, Holder, Parent),
	m_Particle(NULL)
{
}


ParticleGameObject::~ParticleGameObject(void)
{
	if (m_Particle)
	{
		m_SceneNode->detachObject(m_Particle);
		m_Holder->GetSceneManager()->destroyParticleSystem(m_Particle);
		m_Particle = NULL;

		m_TemplateName = "";
	}
}

#ifdef EDIT_MODE
bool ParticleGameObject::Serialize(Attributes *Attributes)
{
	GameObject::Serialize(Attributes);

	Attributes->AddString("TemplateName", m_TemplateName);

	return true;
}
#endif

bool ParticleGameObject::Deserialize(Attributes *Attributes)
{
	GameObject::Deserialize(Attributes);

	SetParticleTemplate(Attributes->GetString("TemplateName"));

	return true;
}
	

void ParticleGameObject::SetInitializeState(void)
{
	m_Init_TemplateName = m_TemplateName;
}


void ParticleGameObject::ResetToInitializeState(void)
{
	SetParticleTemplate(m_Init_TemplateName);
}


void ParticleGameObject::SetParticleTemplate(const Ogre::String &Name)
{
	m_TemplateName = Name;
	
	if (m_Particle)
	{
		m_SceneNode->detachObject(m_Particle);
		m_Holder->GetSceneManager()->destroyParticleSystem(m_Particle);
		m_Particle = NULL;
	}

	if (!m_TemplateName.size())
		return;

	try
	{
		m_Particle = m_Holder->GetSceneManager()->createParticleSystem(m_Holder->GetNewParticleName(), m_TemplateName);
		m_SceneNode->attachObject(m_Particle);
	}
	catch(Ogre::Exception &e)
	{
		Exception::GetInstance().Throw(e);

		m_TemplateName = "";
	}
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(ParticleGameObject)
	BEGIN_DEFINE_TYPE_BASE(GameObject, ParticleGameObject, "ParticleGameObject")
		DEFINE_FUNCTION("GetEnabled", &ParticleGameObject::GetEnabled)
		DEFINE_FUNCTION("SetEnabled", &ParticleGameObject::SetEnabled)
		
		DEFINE_FUNCTION("GetParticleTemplate", &ParticleGameObject::GetParticleTemplate)
		DEFINE_FUNCTION("SetParticleTemplate", &ParticleGameObject::SetParticleTemplate)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE