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

#include "Common.h"
#include "ScriptRegister.h"

BEGIN_NAMESPACE
class IPhysicsScene;

class IPhysicsEngine
{
public:
	virtual void Initialize(void) = 0;

	virtual void Clear(void) = 0;

	virtual void Update(void) = 0;

	virtual IPhysicsScene *CreatePhysicsScene(void) = 0;
};
