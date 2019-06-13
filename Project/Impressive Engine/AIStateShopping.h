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

#include "AIDecisionMaker.h"

BEGIN_NAMESPACE
class AIObject;
class AISellerObject;

class AIStateShopping : public AIDecisionMaker<AIObject>
{
public:
	AIStateShopping(AIObject* pOwner, AISellerObject* pSeller, const Ogre::Vector3& OffsetToSeller);

	~AIStateShopping(void);

	void Activate(void);

	int Process(void);

	void Terminate(void);

	bool HandleMessage(const Message& Msg);
private:
	AISellerObject* m_Seller;

	Ogre::Vector3 m_Destination;

	bool m_bStartShopping;
};


END_NAMESPACE