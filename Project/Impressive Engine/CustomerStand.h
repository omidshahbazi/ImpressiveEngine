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

#include "StdAfxOGRE.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class CustomerStand
{
public:
	CustomerStand(Ogre::Vector3 Position);

	const int& GetCustomerID(void) const;

	const Ogre::Vector3& GetPosition(void) const;

	const bool &IsTaken(void) const;

	void Reserve(unsigned int CustomerID);

	void Realease(void);

	DECLARE_SCRIPT_REGISTATION()

private:
	bool m_bTaken;

	Ogre::Vector3 m_Position;

	int m_CustomerID;

};
END_NAMESPACE

