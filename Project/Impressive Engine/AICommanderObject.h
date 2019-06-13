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

#include "AIEnemyObject.h"

BEGIN_NAMESPACE
class AISoldierObject;

enum FireTeamType
{
	FTT_THREESOME,
	FTT_FOURSOME
};

class AICommanderObject : public AIEnemyObject
{
public:
	AICommanderObject(const unsigned int ID, GameObject* gameObject);

	~AICommanderObject(void);

	void AddSoldier(const unsigned ID);
	void AddSoldier(const Ogre::String Name);
	void AddSoldier(AISoldierObject* Soldier);

	AISoldierObject* GetSoldier(const unsigned int Index);

	const FireTeamType& GetFireTeamType(void) const;

	Ogre::Vector3 CalculateOffsetToSoldier(AISoldierObject* Soldier);

	void CalculateOffsetToSoldiers(void);

	const bool AreSoldiresAtPositions(void) const;

	void SoldiersPursuitOn(void);

	void SoldiersPursuitOff(void);

	void SoldiersLookAt(Ogre::Vector3& Target);

	void SoldiersRotateSmooth(const float& SmoothTime);

	DECLARE_SCRIPT_REGISTATION()

private:
	const bool SoldierExists(AISoldierObject* Soldier);

private:
	std::vector<AISoldierObject*> m_Soldiers;

	FireTeamType m_FireTeamType;
};


END_NAMESPACE