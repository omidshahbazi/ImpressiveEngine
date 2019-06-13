#pragma once

#include "PxPhysicsAPI.h"
#include "Ogre.h"

using namespace physx;
using namespace Ogre;


struct IeTransform;


class IeConvert
{
public:
	static PxVec3 ToPxVec3(const Vector3 &vec);

	static PxQuat ToPxQuat(const Quaternion &quat);

	static Vector3 ToVector3(const PxVec3 &vec);

	static Quaternion ToQuaternion(const PxQuat &quat);

	static IeTransform ToIeTransform(const PxTransform &transform);

	static PxTransform ToPxTransform(const IeTransform &transform);
};

