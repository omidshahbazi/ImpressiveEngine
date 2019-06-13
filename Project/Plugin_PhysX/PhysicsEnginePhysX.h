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

#include <IPlugin.h>
#include <IPhysicsEngine.h>
#include <OgreVector2.h>

using namespace IE;

class PhysicsEnginePhysX : public IPhysicsEngine, public IPlugin
{
public:
	PhysicsEnginePhysX(void);
	~PhysicsEnginePhysX(void);
	
	void Initialize(void);

	void Clear(void);

	void Update(void);

	IPhysicsScene *CreatePhysicsScene(void);
	
	void Install(void) { }

	const char *GetName(void) const
	{
		return "Plugin_PhysX";
	}
	
	const char *GetVersion(void) const
	{
		return "1.0.0.0";
	}
};