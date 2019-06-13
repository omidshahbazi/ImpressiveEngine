#pragma once

#include <OgreCompositorInstance.h>
#include <OgreCompositorLogic.h>
#include <map>

class CompositorLogicListener : public Ogre::CompositorLogic
{
public:
	void compositorInstanceCreated(Ogre::CompositorInstance *Instance);
	void compositorInstanceDestroyed(Ogre::CompositorInstance *Instance);

protected:
	virtual Ogre::CompositorInstance::Listener *CreateListener(Ogre::CompositorInstance *Instance) = 0;
	
private:
	typedef std::map<Ogre::CompositorInstance*, Ogre::CompositorInstance::Listener*> ListenerMap;

	ListenerMap m_Listeners;
};

