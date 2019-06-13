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
class AINPCObject;
class CustomerStand;

class AISellerObject : public AIObject
{
public:
	AISellerObject(const unsigned int ID, GameObject* gameObject);
	~AISellerObject(void);

	void AddRoom(const Ogre::Vector3 Position);

	const CustomerStand* GetRoom(const unsigned int Index);

	void Announce(void);

	const unsigned int GetAnnounceInterval(void) const;

	const bool RoomAvailable(void) const;

	const bool Reserveed(AINPCObject* NPC) const;

	void ReserveRoom(AINPCObject* NPC, Ogre::Vector3& RoomPos);

	void FreeRoom(AINPCObject* NPC);

	DECLARE_SCRIPT_REGISTATION()

private:
	const float OFFSET_TO_CUSTOMER;

	typedef std::vector<CustomerStand*> CustomerStandVector;
	CustomerStandVector m_CustomerStands;

	unsigned int m_AnnounceInterval;
	unsigned int m_LastTimeAnnounced;

};


END_NAMESPACE