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

class IStaticRigidBody : public IRigidBody
{
public:
	virtual void SetFilterGroup(const int &GroupID) = 0;

	virtual void SetFilterMask(const int &Group) = 0;

	virtual void IsTrigger(const bool Enabled) = 0;

};