#pragma once

#include "PxSimulationEventCallback.h"

using namespace physx;

class CollisionCallback : public PxSimulationEventCallback
{
public:
	CollisionCallback(void);

	~CollisionCallback(void);

	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count);

	virtual void onWake(PxActor** actors, PxU32 count);

	virtual void onSleep(PxActor** actors, PxU32 count);

	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
};

