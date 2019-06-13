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

#include "BaseObject.h"

BEGIN_NAMESPACE
class MovingObject : public BaseObject
{
public:
	MovingObject(const unsigned int ID, GameObject* gameObject);

	const float GetSpeed(void) const;

	void SetSpeed(const float Speed);

	const float &GetForce(void) const;

	void SetForce(const float Force);

	const Ogre::Vector3 &GetVelocity(void) const;

	void SetVelocity(const Ogre::Vector3 &Velocity);

	void Translate(const Ogre::Vector3 Translation);
	void Translate(const float X, const float Y, const float Z);

	DECLARE_SCRIPT_REGISTATION()

protected:
	float m_Speed;

	float m_Force;

	Ogre::Vector3 m_Velocity;
};


END_NAMESPACE