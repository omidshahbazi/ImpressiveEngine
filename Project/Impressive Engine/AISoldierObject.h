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
enum SoldierPositionType
{
	SPT_RIGHT,
	SPT_BEHIND,
	SPT_LEFT
};

class AICommanderObject;

class AISoldierObject : public AIEnemyObject
{
public:
	AISoldierObject(const unsigned int ID, GameObject* gameObject);

	~AISoldierObject(void);

	void SetCommander(AICommanderObject* Commander);

	AICommanderObject* GetCommander(void);

	void SetOffsetToCommander(Ogre::Vector3 Offset);

	const Ogre::Vector3& GetOffsetToCommander(void) const;

	void SetPositionType(SoldierPositionType Type);

	const SoldierPositionType GetPositionType(void) const;

	void GoToPosition(void);

	DECLARE_SCRIPT_REGISTATION()

private:
	AICommanderObject* m_Commander;

	Ogre::Vector3 m_OffsetToCommander;

	SoldierPositionType m_PositionType;

};


END_NAMESPACE