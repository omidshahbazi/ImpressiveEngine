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

#include "IRigidBody.h"

class IDynamicRigidBody : public IRigidBody
{
public:
	virtual void SetDensity(const float &Density) = 0;

	virtual const float GetDensity(void) const = 0;

	virtual void SetLinearDamping(const float &Damping) = 0;

	virtual float const GetLinearDamping(void) const = 0;

	virtual void SetAngularDamping(const float &Damping) = 0;

	virtual const float GetAngularDamping(void) const = 0;

	virtual void SetLinearVelocity(const float &X, const float &Y, const float &Z) = 0;
	virtual void SetLinearVelocity(const Ogre::Vector3 &Velocity) = 0;

	virtual const Ogre::Vector3 &GetLinearVelocity(void) const = 0;

	virtual void SetAngularVelocity(const float &X, const float &Y, const float &Z) = 0;
	virtual void SetAngularVelocity(const Ogre::Vector3 &Velocity) = 0;

	virtual const Ogre::Vector3 &GetAngularVelocity(void) const = 0;

	virtual void AddForce(const float &X, const float &Y, const float &Z) = 0;
	virtual void AddForce(const Ogre::Vector3 &Velocity) = 0;

	virtual void UseGravity(const bool Enabled) = 0;

	virtual void IsKinematic(const bool Enabled) = 0;

	virtual void IsTrigger(const bool Enabled) = 0;

	virtual void UseCCD(const bool Enabled) = 0;

	virtual void SetFilterGroup(const int &GroupID) = 0;

	virtual void SetFilterMask(const int &Group) = 0;

	virtual void IsTrigger(const bool Enabled) = 0;

};