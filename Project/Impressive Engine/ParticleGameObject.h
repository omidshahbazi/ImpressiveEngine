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
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class ParticleGameObject : public GameObject
{
public:
	ParticleGameObject(const Ogre::String &Name, Scene *Holder, GameObject *Parent = NULL);
	~ParticleGameObject(void);

#ifdef EDIT_MODE
	bool Serialize(Attributes *Attributes);
#endif
	bool Deserialize(Attributes *Attributes);
	
	void SetInitializeState(void);
	void ResetToInitializeState(void);

	void SetParticleTemplate(const Ogre::String &Name);

	const Ogre::String &GetParticleTemplate(void)
	{
		return m_TemplateName;
	}

	bool GetEnabled(void) const
	{
		if (m_Particle)
			return m_Particle->getEmitter(0)->getEnabled();

		return false;
	}

	void SetEnabled(bool Value) const
	{
		if (m_Particle)
			return m_Particle->getEmitter(0)->setEnabled(Value);
	}
	
	GameObjectType GetType(void) const
	{
		return GOT_PARTICLE;
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	Ogre::ParticleSystem *m_Particle;
	Ogre::String m_TemplateName;

	Ogre::String m_Init_TemplateName;
};


END_NAMESPACE