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
#include "AIMemorySensor.h"
#include "AIObject.h"
#include "Time.h"


BEGIN_NAMESPACE
AIMemorySensor::AIMemorySensor(AIObject* pOwner, float MemorySpan)
{
}


AIMemorySensor::~AIMemorySensor(void)
{
}


void AIMemorySensor::UpdateWithSoundSource(AIObject* pOpponent)
{
}


void AIMemorySensor::UpdateVision(void)
{
}


void AIMemorySensor::RemoveOpponentFromMemory(AIObject* pOpponent)
{
}


const bool AIMemorySensor::IsOpponentShootable(AIObject* pOpponent)
{
	MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);

	if(it == m_MemoryMap.end()) return false;

	return it->second->Shootable;
}

const bool AIMemorySensor::IsOpponentWithinFOV(AIObject* pOpponent)
{
	MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);

	if(it == m_MemoryMap.end()) return false;

	return it->second->WithinFOV;
}


const Ogre::Vector3 AIMemorySensor::GetOpponentLastSensedPosition(AIObject* pOpponent)
{
	MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);

	if(it == m_MemoryMap.end()) 
	{
		throw std::runtime_error("AIMemorySensor::GetOpponentLastSensedPosition: Attemping to get position of unrecorded opponent.");
	}

	return it->second->LastSensedPosition;
}


const unsigned int AIMemorySensor::GetTimeSinceOpponentHasBeenVisible(AIObject* pOpponent)
{
	MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);

	if(it == m_MemoryMap.end()) return 0;

	return Time::GetInstance().GetRealTime() - it->second->TimeBecameVisible;
}


const unsigned int AIMemorySensor::GetTimeOpponentHasBeenOutOfView(AIObject* pOpponent)
{
	MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);

	if(it == m_MemoryMap.end()) return 0;

	return Time::GetInstance().GetRealTime() - it->second->TimeLastVisible;
}		


const unsigned int AIMemorySensor::GetTimeSinceOpponentHasLastSensed(AIObject* pOpponent)
{
	MemoryMap::const_iterator it = m_MemoryMap.find(pOpponent);

	if(it == m_MemoryMap.end()) return 0;

	return Time::GetInstance().GetRealTime() - it->second->TimeLastSensed;
}


std::vector<AIObject*> AIMemorySensor::GetRecentlySensedOpponents(void)
{
	std::vector<AIObject*> OpponentsVector;

	unsigned int CurrentTime = Time::GetInstance().GetRealTime();

	MemoryMap::const_iterator CurrentRecord = m_MemoryMap.begin();
	for(CurrentRecord; CurrentRecord != m_MemoryMap.end(); CurrentRecord++)
	{
		if(CurrentTime - CurrentRecord->second->TimeLastSensed <= m_MemorySpan)
		{
			OpponentsVector.push_back(CurrentRecord->first);
		}
	}

	return OpponentsVector;
}

END_NAMESPACE