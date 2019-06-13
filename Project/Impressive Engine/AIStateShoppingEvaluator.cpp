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
#include "AIStateShoppingEvaluator.h"
#include "AINPCObject.h"
#include "AISellerObject.h"
#include "GameObject.h"
#include "AIBrain.h"
#include "Time.h"


BEGIN_NAMESPACE
AIStateShoppingEvaluator::AIStateShoppingEvaluator(float Bias) : AIStateEvaluator(Bias)
{
}


AIStateShoppingEvaluator::~AIStateShoppingEvaluator(void)
{
}


float AIStateShoppingEvaluator::CalculateDesirability(AIObject* aiObject)
{
	AINPCObject* Shopper = (AINPCObject*)aiObject;
	AISellerObject* Seller = Shopper->GetSeller();
	unsigned int delta = Time::GetInstance().GetRealTime() - ((AINPCObject*)aiObject)->GetLastTimeShoppingFinished();

	if(!Shopper->IsGoingShopping()) return 0.0f;

	if(Seller->Reserveed(Shopper)) return 1.0f;

	  if(!Shopper->GetSeller() ||
		delta <= Shopper->GetShoppingInterval() ||
		!Seller->RoomAvailable())
	{
		return 0.0f;
	}

	Ogre::Vector3 Offset = Ogre::Vector3::ZERO;

	Seller->ReserveRoom(Shopper, Offset);
	Shopper->SetOffsetToSeller(Offset);
	return 1.0f;
}


void AIStateShoppingEvaluator::SetGoal(AIObject* aiObject)
{
	aiObject->GetBrain()->AddStateShopping();
}

END_NAMESPACE