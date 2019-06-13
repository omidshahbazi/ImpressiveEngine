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

#include <map>
#include <vector>
#include "AIMemoryRecord.h"

BEGIN_NAMESPACE
class AIObject;

class AIMemorySensor
{
private:
	typedef std::map<AIObject*, AIMemoryRecord*> MemoryMap;

	AIObject* m_Owner;

	MemoryMap m_MemoryMap;

	unsigned int m_MemorySpan;

	void MakeNewRecordIfNotAlreadyPresent(AIObject* pOpponent);

public:
	AIMemorySensor(AIObject* pOwner, float MemorySpan);

	~AIMemorySensor(void);

	void UpdateWithSoundSource(AIObject* pOpponent);

	void UpdateVision(void);

	void RemoveOpponentFromMemory(AIObject* pOpponent);

	const bool IsOpponentShootable(AIObject* pOpponent);

	const bool IsOpponentWithinFOV(AIObject* pOpponent);

	const Ogre::Vector3 GetOpponentLastSensedPosition(AIObject* pOpponent);

	const unsigned int GetTimeSinceOpponentHasBeenVisible(AIObject* pOpponent);

	const unsigned int GetTimeOpponentHasBeenOutOfView(AIObject* pOpponent);

	const unsigned int GetTimeSinceOpponentHasLastSensed(AIObject* pOpponent);

	std::vector<AIObject*> GetRecentlySensedOpponents(void);

};


END_NAMESPACE