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
class AISellerObject;

class AINPCObject : public AIObject
{
public:
	AINPCObject(const unsigned int ID, GameObject* gameObject, bool& RandomInitPosition);

	~AINPCObject(void);

	const bool &IsGoingShopping(void) const;

	void GoShopping(AISellerObject* pSeller);

	void LeaveShopping(void);

	const unsigned int &GetLastTimeShoppingFinished(void) const;

	const unsigned int &GetShoppingInterval(void) const;

	AISellerObject* GetSeller(void);

	const Ogre::Vector3& GetOffsetToSeller(void) const;

	void SetOffsetToSeller(const Ogre::Vector3 Offset);

	void ExploreFreely(void);

	DECLARE_SCRIPT_REGISTATION()

public:
	const unsigned int STOPT_TIME_AT_SELLER;

private:
	bool m_bGoShopping;

	unsigned int m_LastTimeShoppingFinished;

	const unsigned int MIN_SHOPPINT_TIME;
	const unsigned int MAX_SHOPPINT_TIME;

	unsigned int m_ShoppingInterval;

	Ogre::Vector3 m_SpawnPoint;

	AISellerObject* m_Seller;

	Ogre::Vector3 m_OffsetToSeller;

};


END_NAMESPACE