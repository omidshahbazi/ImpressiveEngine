#include "IeConvert.h"


struct IeTransform
{
	Vector3 p;
	Quaternion q;

	IeTransform(Vector3 P, Quaternion Q) : p(P), q(Q)
	{
	}

};


PxVec3 IeConvert::ToPxVec3(const Ogre::Vector3 &vec)
{
	return PxVec3(vec.x, vec.y, vec.z);
}


PxQuat IeConvert::ToPxQuat(const Quaternion &quat)
{
	return PxQuat(quat.w, quat.x, quat.y, quat.z);
}


Vector3 IeConvert::ToVector3(const PxVec3 &vec)
{
	return Vector3(vec.x, vec.y, vec.z);
}


Quaternion IeConvert::ToQuaternion(const PxQuat &quat)
{
	return Quaternion(quat.w, quat.x, quat.y, quat.z);
}


IeTransform IeConvert::ToIeTransform(const PxTransform &transform)
{
	return IeTransform(ToVector3(transform.p), ToQuaternion(transform.q));
}


PxTransform IeConvert::ToPxTransform(const IeTransform &transform)
{
	return PxTransform(ToPxVec3(transform.p), ToPxQuat(transform.q));
}
