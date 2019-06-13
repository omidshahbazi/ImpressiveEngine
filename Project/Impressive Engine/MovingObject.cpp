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
#include "MovingObject.h"
#include "GameObject.h"
#include "Mathematics.h"


BEGIN_NAMESPACE
MovingObject::MovingObject(const unsigned int ID, GameObject* gameObject) : BaseObject(ID, gameObject),
																				m_Speed(1.0f),
																				m_Velocity(Ogre::Vector3::ZERO),
																				m_Force(1.0f)
{
}


const float MovingObject::GetSpeed() const
{
	return m_Speed / 10;
}


void MovingObject::SetSpeed(const float Speed)
{
	m_Speed = Speed;
}


const float &MovingObject::GetForce(void) const
{
	return m_Force;
}


void MovingObject::SetForce(const float Force)
{
	m_Force = Force;
}


const Ogre::Vector3 &MovingObject::GetVelocity() const
{
	return m_Velocity;
}


void MovingObject::SetVelocity(const Ogre::Vector3 &Velocity)
{
	m_Velocity = Velocity;
}


void MovingObject::Translate(const Ogre::Vector3 Translation)
{
	m_GameObject->SetPosition(m_GameObject->GetPosition() + Translation);
}


void MovingObject::Translate(const float X, const float Y, const float Z)
{
	Translate(Ogre::Vector3(X, Y, Z));
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(MovingObject)
	BEGIN_DEFINE_TYPE_BASE(BaseObject, MovingObject, "MovingObject")
		DEFINE_FUNCTION("Translate", (void(MovingObject::*)(const Ogre::Vector3))&MovingObject::Translate)
		DEFINE_FUNCTION("Translate", (void(MovingObject::*)(const float, const float, const float))&MovingObject::Translate)
		DEFINE_FUNCTION("SetSpeed", (void(MovingObject::*)(const float))&MovingObject::SetSpeed)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE