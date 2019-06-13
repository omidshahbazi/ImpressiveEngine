#pragma once

#include "CompositorLogicListener.h"

class SSAOLogic : public CompositorLogicListener
{
public:
	Ogre::CompositorInstance::Listener *CreateListener(Ogre::CompositorInstance *Instance);
};

