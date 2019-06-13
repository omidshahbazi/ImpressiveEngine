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
#include "Common.h"

BEGIN_NAMESPACE
struct AIMemoryRecord
{
	unsigned int TimeLastSensed;

	unsigned int TimeBecameVisible;

	unsigned int TimeLastVisible;

	Ogre::Vector3 LastSensedPosition;

	bool WithinFOV;

	bool Shootable;

	AIMemoryRecord(void) : TimeLastSensed(0),
						   TimeBecameVisible(0),
						   TimeLastVisible(0),
						   WithinFOV(false),
						   Shootable(false)
	{
	}
};


END_NAMESPACE