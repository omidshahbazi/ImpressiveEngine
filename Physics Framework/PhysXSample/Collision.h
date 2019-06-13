#pragma once

#include "PxSimulationEventCallback.h"

using namespace physx;

class Collision
{
public:
	Collision(void);
	~Collision(void);

	PxContactPairPoint *ContactPoints;
};

