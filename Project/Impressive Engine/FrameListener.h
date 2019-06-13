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
#include "Singleton.h"

BEGIN_NAMESPACE
//class CAVIGenerator;

class FrameListener : public Singleton<FrameListener>, public Ogre::FrameListener
#ifdef EDIT_MODE
	,public Ogre::Camera::Listener/*, public Ogre::RenderQueueListener*/
#endif
{
public:
	//FrameListener(void);
	//~FrameListener(void);

	void Initialize(void);
	
#ifdef EDIT_MODE
	void initilizeRecorder(void);

	void cameraPostRenderScene(Ogre::Camera *Camera);
#endif

	bool frameStarted(const Ogre::FrameEvent &evt);
	//bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	bool frameEnded(const Ogre::FrameEvent &evt);

	//void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &skipThisInvocation);

#ifdef EDIT_MODE
private:
	void SetBitmapHeader(void);
#endif

private:
	HRESULT res;

	Ogre::TexturePtr m_RT;

public:
	//CAVIGenerator *m_Avi;
	Ogre::Image img;
	bool recording;
};
END_NAMESPACE