/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
///
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
#include "Common.h"

BEGIN_NAMESPACE
class IRigidBody;
class IDynamicRigidBody;
class IStaticRigidBody;
class GameObject;

class IPhysicsScene
{
public:
	virtual void Clear(void) = 0;

	virtual void Update(void) = 0;

	virtual IDynamicRigidBody *CreateDynamicRigidBody(const GameObject *gameObject) = 0;

	virtual IStaticRigidBody *CreateStaticRigidBody(const GameObject *gameObject) = 0;
};
END_NAMESPACE