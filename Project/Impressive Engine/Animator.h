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

#include "StdAfxOGRE.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class GameObject;

class Animator
{
public:
	Animator(GameObject *GameObject, const Ogre::String &Name, const float &Length);
	virtual ~Animator(void)
	{
		_Destroy();
	}

	void Update(void);

	const Ogre::String &GetName(void) const
	{
		return m_Animation->getName();
	}

	const float GetLength(void) const
	{
		return m_Animation->getLength();
	}
	
	void SetLength(const float &Length)
	{
		//
		// Maybe we have to user Ogre::AnimationState::setLength() and get, instead
		//
		return m_Animation->setLength(Length);
	}

	void SetInterpolationMode(const Ogre::Animation::InterpolationMode &Mode)
	{
		m_Animation->setInterpolationMode(Mode);
	}

	Ogre::Animation::InterpolationMode GetInterpolationMode(void) const
	{
		return m_Animation->getInterpolationMode();
	}

	void SetRotationInterpolationMode(const Ogre::Animation::RotationInterpolationMode &Mode)
	{
		m_Animation->setRotationInterpolationMode(Mode);
	}

	Ogre::Animation::RotationInterpolationMode GetRotationInterpolationMode(void) const
	{
		return m_Animation->getRotationInterpolationMode();
	}

	const bool HasEnded(void) const
	{
		return m_State->hasEnded();
	}

	const bool GetEnabled(void) const
	{
		return m_State->getEnabled();
	}

	void SetEnabled(const bool &Value)
	{
		m_State->setEnabled(Value);
	}

	void SetLoop(const bool &Value)
	{
		m_State->setLoop(Value);
	}

	const bool GetLoop(void) const 
	{
		return m_State->getLoop();
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	void _Destroy(void);

protected:
	GameObject *m_GameObject;
	Ogre::Animation *m_Animation;
	Ogre::AnimationState *m_State;
};


END_NAMESPACE