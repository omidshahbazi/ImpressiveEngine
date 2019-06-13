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

#include "AIObject.h"

BEGIN_NAMESPACE
enum WaypointType;

class AIEnemyObject : public AIObject
{
public:
	AIEnemyObject(const unsigned int ID, GameObject* gameObject);

	~AIEnemyObject(void);

	void PursueTarget();

	DECLARE_SCRIPT_REGISTATION()
};
END_NAMESPACE