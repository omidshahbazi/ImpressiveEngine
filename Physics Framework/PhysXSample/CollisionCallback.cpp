#include "CollisionCallback.h"
#include "PxRigidActor.h"
#include "PseudoGameObject.h"
#include "PxShape.h"
#include "Collision.h"
#include <iostream>


std::ofstream output("C:/collision.txt");
std::ofstream outputTrigger("C:/Trigger.txt");


CollisionCallback::CollisionCallback(void)
{
}


CollisionCallback::~CollisionCallback(void)
{
}


void CollisionCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}


void CollisionCallback::onWake(PxActor** actors, PxU32 count)
{
}


void CollisionCallback::onSleep(PxActor** actors, PxU32 count)
{
}

static int triggerCounter = 0;

void CollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for(PxU32 i=0; i < count; i++)
	{
		// ignore pairs when shapes have been deleted
		if (pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& cp = pairs[i];

		String tag1 = ((PseudoGameObject*)cp.triggerShape->userData)->GetTag();
		String tag2 = ((PseudoGameObject*)cp.otherShape->userData)->GetTag();

		if(cp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			triggerCounter++;

			outputTrigger << triggerCounter << " - OnTriggerEnter: " << tag1 << 
					" VS " << tag2 << '\n';
		}
		else if(cp.status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)			// NOT TRUE
		{
			triggerCounter++;

			outputTrigger << triggerCounter << " - OnTriggerStay: " << tag1 << 
				" VS " << tag2 << '\n';
		}
		else if(cp.status == PxPairFlag::eNOTIFY_TOUCH_LOST)				// NOT TRUE
		{

			triggerCounter++;

			outputTrigger << triggerCounter << " - OnTriggerExit: " << tag1 << 
				" VS " << tag2 << '\n';
		}
	}
}


static int counter = 0;

void CollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	if (nbPairs == 0)
	{
		output << counter << "None " << '\n';
	}

	for(PxU32 i=0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];
		PxContactPairPoint* contacts = new PxContactPairPoint[cp.contactCount];

		String tag1 = ((PseudoGameObject*)pairHeader.actors[0]->userData)->GetTag();
		String tag2 = ((PseudoGameObject*)pairHeader.actors[1]->userData)->GetTag();

		if(cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
			pairs[i].extractContacts(contacts, sizeof(PxContactPairPoint));
			PxVec3 normal = contacts[0].normal;
			//output << "Pairs: " << i << ": (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << '\n';

			counter++;

			output << counter << " - OnCollisionEnter: " << tag1 << 
				" VS " << tag2 << '\n';
        }
		else if(cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)			// NOT TRUE
        {
			counter++;

			output << counter << " - OnCollisionStay: " << tag1 << 
				" VS " << tag2 << '\n';
		}
		else if(cp.events == PxPairFlag::eNOTIFY_TOUCH_LOST)				// NOT TRUE
        {

			counter++;

			output << counter << " - OnCollisionExit: " << tag1 << 
				" VS " << tag2 << '\n';
		}
    }
}
