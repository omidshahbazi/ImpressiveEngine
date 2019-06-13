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
#include "Animator.h"
#include "GameObject.h"
#include "Time.h"


BEGIN_NAMESPACE
Animator::Animator(GameObject *GameObject, const Ogre::String &Name, const float &Length) :
	m_GameObject(GameObject)
{
	m_Animation = m_GameObject->_GetSceneNode()->getCreator()->createAnimation(Name, Length);
	
	m_State = m_GameObject->_GetSceneNode()->getCreator()->createAnimationState(Name);
	
	SetLoop(false);
}


void Animator::Update(void)
{
	m_State->addTime(Time::GetInstance().GetTimeSinceLastUpdate() * Time::GetInstance().GetTimeScale() / 1000.f);
}


void Animator::_Destroy(void)
{
	m_GameObject->_GetSceneNode()->getCreator()->destroyAnimation(GetName());
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(Animator)
	BEGIN_DEFINE_TYPE(Animator, "Animator")
		DEFINE_FUNCTION("GetName", &Animator::GetName)

		DEFINE_FUNCTION("GetLength", &Animator::GetLength)
		DEFINE_FUNCTION("SetLength", &Animator::SetLength)

		DEFINE_FUNCTION("SetInterpolationMode", &Animator::SetInterpolationMode)
		DEFINE_FUNCTION("GetInterpolationMode", &Animator::GetInterpolationMode)

		DEFINE_FUNCTION("SetRotationInterpolationMode", &Animator::SetRotationInterpolationMode)
		DEFINE_FUNCTION("GetRotationInterpolationMode", &Animator::GetRotationInterpolationMode)

		DEFINE_FUNCTION("HasEnded", &Animator::HasEnded)

		DEFINE_FUNCTION("GetEnabled", &Animator::GetEnabled)
		DEFINE_FUNCTION("SetEnabled", &Animator::SetEnabled)

		DEFINE_FUNCTION("SetLoop", &Animator::SetLoop)
		DEFINE_FUNCTION("GetLoop", &Animator::GetLoop)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE