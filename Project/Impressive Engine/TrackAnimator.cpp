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
#include "TrackAnimator.h"
#include "GameObject.h"


BEGIN_NAMESPACE
TrackAnimator::TrackAnimator(GameObject *GameObject, const Ogre::String &Name, const float &Length) :
	Animator(GameObject, Name, Length)
{
	m_Track = m_Animation->createNodeTrack(0, m_GameObject->_GetSceneNode());
}


TrackAnimator::~TrackAnimator(void)
{
	m_Animation->destroyAllNodeTracks();
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(TrackAnimator)
	BEGIN_DEFINE_TYPE_BASE(Animator, TrackAnimator, "TrackAnimator")
		DEFINE_FUNCTION("CreateKeyFrame", &TrackAnimator::CreateKeyFrame)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE