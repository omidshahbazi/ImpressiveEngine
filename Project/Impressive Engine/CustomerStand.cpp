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
#include "CustomerStand.h"


BEGIN_NAMESPACE
CustomerStand::CustomerStand(Ogre::Vector3 Position) : m_Position(Position), m_bTaken(false), m_CustomerID(-1)
{
}


const int& CustomerStand::GetCustomerID(void) const
{
	return m_CustomerID;
}


const Ogre::Vector3& CustomerStand::GetPosition(void) const
{
	return m_Position;
}


const bool &CustomerStand::IsTaken(void) const
{
	return m_bTaken;
}


void CustomerStand::Reserve(unsigned int CustomerID)
{
	m_CustomerID = CustomerID;
	m_bTaken = true;
}


void CustomerStand::Realease(void)
{
	m_CustomerID = -1;
	m_bTaken = false;
}


BEGIN_DEFINE_SCRIPT_REGISTRATION(CustomerStand)
	BEGIN_DEFINE_TYPE(CustomerStand, "CustomerStand")
		DEFINE_FUNCTION("GetPosition", &CustomerStand::GetPosition)
	END_DEFINE_TYPE()
END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE