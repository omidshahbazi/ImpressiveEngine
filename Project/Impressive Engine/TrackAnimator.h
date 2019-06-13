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

#include "Animator.h"


BEGIN_NAMESPACE
class TrackAnimator : public Animator
{
public:
	TrackAnimator(GameObject *GameObject, const Ogre::String &Name, const float &Length);
	~TrackAnimator(void);

	Ogre::TransformKeyFrame *CreateKeyFrame(const float &TimePosition)
	{
		return m_Track->createNodeKeyFrame(TimePosition);
	}

	DECLARE_SCRIPT_REGISTATION()

private:
	Ogre::NodeAnimationTrack *m_Track;
};


END_NAMESPACE