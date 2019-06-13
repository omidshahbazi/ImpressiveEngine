#include "CompositorLogicListener.h"


void CompositorLogicListener::compositorInstanceCreated(Ogre::CompositorInstance *Instance)
{
	Ogre::CompositorInstance::Listener *listener = CreateListener(Instance);

	Instance->addListener(listener);

	m_Listeners[Instance] = listener;
}


void CompositorLogicListener::compositorInstanceDestroyed(Ogre::CompositorInstance *Instance)
{
	delete m_Listeners[Instance];

	m_Listeners.erase(Instance);
}


